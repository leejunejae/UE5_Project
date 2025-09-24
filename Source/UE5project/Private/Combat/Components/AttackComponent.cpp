// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Components/AttackComponent.h"
#include "GameFramework/Character.h"
#include "Combat/Interfaces/HitReactionInterface.h"
#include "Combat/Interfaces/CombatInterface.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Engine/StaticMeshSocket.h"
#include "Utils/AnimBoneTransformDataAsset.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> TrailNS(TEXT("/Game/99_TestTool/NS_Trail_10.NS_Trail_10"));
	if (TrailNS.Succeeded())
	{
		TrailSystem = TrailNS.Object;
	}
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedCharacter = Cast<ACharacter>(GetOwner());

	if (!CheckOwnerExist())
		return;

	for (UActorComponent* Comp : CachedCharacter->GetComponents())
	{
		if (Comp->GetClass()->ImplementsInterface(UEquipmentDataInterface::StaticClass()))
		{
			CachedEquipment = TScriptInterface<IEquipmentDataInterface>(Comp);
			break;
		}
	}

	// ...
	AnimInstance = GetOwner()->GetComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UAttackComponent::OnMontageNotifyBegin);
		AnimInstance->OnPlayMontageNotifyEnd.AddDynamic(this, &UAttackComponent::OnMontageNotifyEnd);
	}

	if (TrailSystem)
	{
		TrailComponent = NewObject<UNiagaraComponent>(GetOwner());
		TrailComponent->SetAsset(TrailSystem);
		TrailComponent->AttachToComponent(GetOwner()->GetRootComponent(),
			FAttachmentTransformRules::KeepRelativeTransform);
		TrailComponent->RegisterComponent();
		TrailComponent->SetForceSolo(true);
		TrailComponent->SetAutoActivate(false); // 필요할 때 Activate()
	}
}

bool UAttackComponent::ReCastOwner()
{
	CachedCharacter = Cast<ACharacter>(GetOwner());

	return CachedCharacter.IsValid() ? true : false;
}

bool UAttackComponent::CheckOwnerExist()
{
	bool IsExist = true;

	if (!CachedCharacter.IsValid())
	{
		IsExist = ReCastOwner();
	}

	return IsExist;
}

ACharacter* UAttackComponent::GetMyCharacter()
{
	if (!CachedCharacter.IsValid())
	{
		return CachedCharacter.Get();
	}

	return nullptr;
}

void UAttackComponent::ExecuteAttack(FName AttackName, float Playrate)
{
	bool CanPlayAttack = false;

	// 실행중인 공격이 있으면 실행중인 공격과 입력이 같은지 확인	
	if (CurAttackInfo.AttackName == AttackName)
	{
		CanPlayAttack = true;
		// 다음 콤보가 있으면 ComboIndex증가 아니면 첫콤보로 초기화
		ComboIndex = CurAttackInfo.AttackDetail.IsValidIndex(ComboIndex + 1) ? ComboIndex + 1 : 0;
	}
	else
	{
		if (CurAttackType.AttackInfoList.IsEmpty()) return;
		FPlayerAttackInfo DataForFind;
		DataForFind.AttackName = AttackName;
		const FPlayerAttackInfo* FoundData = CurAttackType.AttackInfoList.Find(DataForFind);
		CurAttackInfo = FoundData ? *FoundData : FPlayerAttackInfo{};

		if (CurAttackInfo.Anim && !CurAttackInfo.AttackDetail.IsEmpty())
		{
			CanPlayAttack = true;
			ComboIndex = 0;
		}
	}

	if (CanPlayAttack)
	{
		PlayAnimation(CurAttackInfo, ComboIndex, Playrate);
	}
}

void UAttackComponent::PlayAnimation(FPlayerAttackInfo AttackInfo, int32 index, float Playrate)
{
	AnimInstance->Montage_Play(AttackInfo.Anim, Playrate);
	AnimInstance->Montage_JumpToSection(AttackInfo.AttackDetail[index].BaseAttackData.SectionName, AttackInfo.Anim);

	FOnMontageEnded MontageEndDelegate;
	MontageEndDelegate.BindUObject(this, &UAttackComponent::OnMontageEnded);
	AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, AttackInfo.Anim);
}

void UAttackComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	FString EndMontage = Montage ? Montage->GetName() : TEXT("None");
	UAnimMontage* CurrentMontage = AnimInstance ? AnimInstance->GetCurrentActiveMontage() : nullptr;
	FString CurMontage = CurrentMontage ? CurrentMontage->GetName() : TEXT("None");

	//UE_LOG(LogTemp, Log, TEXT("Montage Ended : %s "), *EndMontage);
	//UE_LOG(LogTemp, Log, TEXT("Montage Current : %s "), *CurMontage);

	if (!(EndMontage.Equals(CurMontage)))
	{
		ComboIndex = 0;
		CurAttackInfo = FPlayerAttackInfo();
	}

	AnimInstance->OnMontageEnded.RemoveDynamic(this, &UAttackComponent::OnMontageEnded);
}

void UAttackComponent::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (BranchingPointPayload.SequenceAsset)
	{
		if (NotifyName == FName("WeaponAttack"))
		{
			if (CachedEquipment.GetInterface() && CachedEquipment.GetObject())
			{
				UStaticMeshComponent* WeaponMeshComponent = IEquipmentDataInterface::Execute_GetMainWeaponMeshComponent(CachedEquipment.GetObject());
				FWeaponSetsInfo WeaponInfo = IEquipmentDataInterface::Execute_GetWeaponSetsData(CachedEquipment.GetObject());

				if (WeaponMeshComponent != nullptr)
				{
					float AnimSectionTime = AnimInstance->GetCurveValue(FName("AnimSectionTime"));
					float StartTime = BranchingPointPayload.NotifyEvent->GetTriggerTime() - AnimSectionTime;
					float EndTime = BranchingPointPayload.NotifyEvent->GetEndTriggerTime() - AnimSectionTime;
					UE_LOG(LogTemp, Log, TEXT("NotifyState: %s, Start: %f, End: %f"),
						*BranchingPointPayload.NotifyEvent->NotifyName.ToString(),
						StartTime,
						EndTime);

					FTimerDelegate AttackTimerDelegate;
					AttackTimerDelegate.BindUObject(this, &UAttackComponent::DetectAttackTarget, WeaponMeshComponent, WeaponInfo, StartTime, EndTime, false, false);
					GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, AttackTimerDelegate, 0.01f, true);
				}
			}
		}
		else if (NotifyName == FName("WeaponTrail"))
		{
			if (CachedEquipment.GetInterface() && CachedEquipment.GetObject())
			{
				FWeaponSetsInfo WeaponInfo = IEquipmentDataInterface::Execute_GetWeaponSetsData(CachedEquipment.GetObject());

				float AnimSectionTime = AnimInstance->GetCurveValue(FName("AnimSectionTime"));
				float StartTime = BranchingPointPayload.NotifyEvent->GetTriggerTime() - AnimSectionTime;
				float EndTime = BranchingPointPayload.NotifyEvent->GetEndTriggerTime() - AnimSectionTime;
				UE_LOG(LogTemp, Log, TEXT("NotifyState: %s, Start: %f, End: %f"),
					*BranchingPointPayload.NotifyEvent->NotifyName.ToString(),
					StartTime,
					EndTime);

				FTimerDelegate TrailTimerDelegate;
				TrailTimerDelegate.BindUObject(this, &UAttackComponent::ExecuteWeaponTrail, WeaponInfo, StartTime, EndTime, false, false);
				GetWorld()->GetTimerManager().SetTimer(TrailTimerHandle, TrailTimerDelegate, 0.01f, true);
			}
		}
	}
}

void UAttackComponent::OnMontageNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName("WeaponAttack"))
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(AttackTimerHandle))
			GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);

		UStaticMeshComponent* WeaponMeshComponent = IEquipmentDataInterface::Execute_GetMainWeaponMeshComponent(CachedEquipment.GetObject());
		FWeaponSetsInfo WeaponInfo = IEquipmentDataInterface::Execute_GetWeaponSetsData(CachedEquipment.GetObject());
		float AnimSectionTime = AnimInstance->GetCurveValue(FName("AnimSectionTime"));
		float StartTime = BranchingPointPayload.NotifyEvent->GetTriggerTime() - AnimSectionTime;
		float EndTime = BranchingPointPayload.NotifyEvent->GetEndTriggerTime() - AnimSectionTime;
		DetectAttackTarget(WeaponMeshComponent, WeaponInfo, StartTime, EndTime, true, false);
		UE_LOG(LogTemp, Warning, TEXT("Reset Detect"));
		TrailComponent->Deactivate();
		DetectTracePrevTime.Key = false;
		HitActorListCurrentAttack.Empty();
	}
	else if (NotifyName == FName("WeaponTrail"))
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(TrailTimerHandle))
			GetWorld()->GetTimerManager().ClearTimer(TrailTimerHandle);

		if (CachedEquipment.GetInterface() && CachedEquipment.GetObject())
		{
			FWeaponSetsInfo WeaponInfo = IEquipmentDataInterface::Execute_GetWeaponSetsData(CachedEquipment.GetObject());

			float AnimSectionTime = AnimInstance->GetCurveValue(FName("AnimSectionTime"));
			float StartTime = BranchingPointPayload.NotifyEvent->GetTriggerTime() - AnimSectionTime;
			float EndTime = BranchingPointPayload.NotifyEvent->GetEndTriggerTime() - AnimSectionTime;

			ExecuteWeaponTrail(WeaponInfo, StartTime, EndTime, true, false);
			TrailPrevTime.Key = false;
			TrailComponent->Deactivate();
		}
	}
}

void UAttackComponent::DetectAttackTarget(UStaticMeshComponent* WeaponMesh, FWeaponSetsInfo WeaponInfo, float StartTime, float EndTime,bool IsDetectEnd, bool IsSubWeaponAttack)
{
	UE_LOG(LogTemp, Warning, TEXT("DetectTarget"));

	ACharacter* Character = Cast<ACharacter>(GetOwner());

	// 실행하는 공격 애니메이션의 본 트랜스폼 데이터 가져오기
	UAnimBoneTransformDataAsset* TraceTargetBoneTransformData = CurAttackInfo.AttackDetail[ComboIndex].BaseAttackData.TargetBoneTransformDataAsset.Get();

	// 무기 데이터
	FWeaponPartInfo CurrentWeaponPart = IsSubWeaponAttack ? WeaponInfo.SubWeapon : WeaponInfo.MainWeapon;

	// 처음 실행되었을때 초기값 설정
	if (!DetectTracePrevTime.Key)
	{
		DetectTracePrevTime.Key = true;
		DetectTracePrevTime.Value = StartTime; // 노티파이 시작 시간으로 초기값 설정
		TrailComponent->DeactivateImmediate();
		TrailComponent->ResetSystem();            // 히스토리 초기화
		TrailComponent->Activate(true);
	}

	// 현재 시간(몽타주 전체에서 현재 시간 - 현재 섹션의 시작 시간), 노티파이가 끝날때 호출 되면 노티파이의 끝 시간
	float CurTime = !IsDetectEnd
		? AnimInstance->GetCurveValue(FName("AnimTimeCheckCurve")) - AnimInstance->GetCurveValue(FName("AnimSectionTime"))
		: EndTime;

	// 이전프레임과 현재프레임 사이를 0.001초 간격으로 나눔
	int32 TraceCorrectionCount = (CurTime - DetectTracePrevTime.Value) / 0.001f;

	// 현재 루트본의 위치
	FTransform CurrentRootWorldTransform = Character->GetMesh()->GetBoneTransform(0);

	FTransform TargetWeaponOffset = CurrentWeaponPart.WeaponTransform;

	const UStaticMeshSocket* StartSocket = WeaponMesh->GetStaticMesh()->FindSocket("Start");
	const UStaticMeshSocket* EndSocket = WeaponMesh->GetStaticMesh()->FindSocket("End");

	if (!StartSocket || !EndSocket) return;

	const FVector StartSocketRelativeLocation = StartSocket->RelativeLocation;
	const FVector EndSocketRelativeLocation = EndSocket->RelativeLocation;

	const float subDT = (CurTime - DetectTracePrevTime.Value) / TraceCorrectionCount;

	for (int32 i = 1; i <= TraceCorrectionCount; ++i)
	{
		float PrevTime = DetectTracePrevTime.Value + (i * 0.001f);
		FTransform PrevTargetBoneTransformData = TraceTargetBoneTransformData->GetTransformAtTime(PrevTime);

		FVector PrevHandLocationAtDataWorldLocation = CurrentRootWorldTransform.TransformPosition(PrevTargetBoneTransformData.GetLocation());
		FQuat PrevHandQuatAtDataWorldRotator = CurrentRootWorldTransform.TransformRotation(PrevTargetBoneTransformData.GetRotation());
		FRotator PrevHandRotatorAtQuat = PrevHandQuatAtDataWorldRotator.Rotator();

		FTransform PrevHandTransform(PrevHandRotatorAtQuat, PrevHandLocationAtDataWorldLocation, PrevTargetBoneTransformData.GetScale3D());


		FVector PrevTargetWeaponLocation = PrevHandTransform.TransformPosition(TargetWeaponOffset.GetLocation());
		FQuat PrevTargetWeaponQuat = PrevHandTransform.TransformRotation(TargetWeaponOffset.GetRotation());
		FRotator PrevTargetWeaponRotation = PrevTargetWeaponQuat.Rotator();

		FTransform PrevTargetWeaponTransform(PrevTargetWeaponRotation, PrevTargetWeaponLocation, CurrentWeaponPart.WeaponTransform.GetScale3D());

		FVector PrevStartSocketWorldLocation = PrevTargetWeaponTransform.TransformPosition(StartSocketRelativeLocation);
		FVector PrevEndSocketWorldLocation = PrevTargetWeaponTransform.TransformPosition(EndSocketRelativeLocation);

		FVector StartLoc = PrevStartSocketWorldLocation;
		FVector EndLoc = PrevEndSocketWorldLocation;

		float CurWeaponLength = FVector::Distance(StartLoc, EndLoc);
		float CurHalfHeight = (CurWeaponLength * 0.5f);

		FVector CurCapsuleCenter = (StartLoc + EndLoc) * 0.5f;
		FVector CurCapsuleAxis = (EndLoc - StartLoc).GetSafeNormal();
		FQuat CurCapsuleRotation = FRotationMatrix::MakeFromZ(CurCapsuleAxis).ToQuat();

		TArray<FHitResult> HitResults;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());

		float Radius = CurrentWeaponPart.HitBoxRadius;

		TrailComponent->SetVectorParameter(FName("TrailStart"), StartLoc);
		TrailComponent->SetVectorParameter(FName("TrailEnd"), EndLoc);
		TrailComponent->AdvanceSimulation(1, subDT);

		FCollisionShape DetectShape = FCollisionShape::MakeCapsule(Radius, CurHalfHeight);

		bool bHit = GetWorld()->SweepMultiByChannel(
			HitResults,
			StartLoc,
			EndLoc,
			FQuat::Identity,
			ECC_GameTraceChannel3,
			DetectShape,
			CollisionParams
		);

		if (bHit)
		{
			for (const FHitResult& Result : HitResults)
			{
				AActor* HitActor = Result.GetActor();
				if (HitActor && !HitActorListCurrentAttack.Contains(HitActor))
				{
					HitActorListCurrentAttack.Add(HitActor);

					if (HitActor->Implements<UHitReactionInterface>())
					{
						float OutDamage = WeaponInfo.AttackPower * CurAttackInfo.AttackDetail[ComboIndex].DamageMultiplier;
						float OutPoiseDamage = WeaponInfo.PoisePower * CurAttackInfo.AttackDetail[ComboIndex].PoiseDamageMultiplier;
						float OutStanceDamage = WeaponInfo.StancePower * CurAttackInfo.AttackDetail[ComboIndex].StanceDamageMultiplier;
						EHitResponse OutResponse = CurAttackInfo.AttackDetail[ComboIndex].BaseAttackData.Response;
						EAttackType OutAttackType = CurAttackInfo.AttackDetail[ComboIndex].BaseAttackData.AttackType;
						FVector OutHitPoint = Result.ImpactPoint;
						FString OutHitPointName = Result.PhysMaterial.IsValid() ? Result.PhysMaterial->GetName() : FString();
						bool OutCanBlocked = CurAttackInfo.AttackDetail[ComboIndex].BaseAttackData.CanBlocked;
						bool OutCanParried = CurAttackInfo.AttackDetail[ComboIndex].BaseAttackData.CanParried;
						bool OutCanAvoid = CurAttackInfo.AttackDetail[ComboIndex].BaseAttackData.CanAvoid;
						TArray<FStatusEffect> OutStatusEffect = CurAttackInfo.AttackDetail[ComboIndex].BaseAttackData.StatusEffectList;

						FAttackRequest OutAttackData(
							OutDamage,
							OutStanceDamage,
							OutPoiseDamage,
							OutResponse,
							OutHitPoint,
							OutHitPointName,
							OutCanBlocked,
							OutCanParried,
							OutCanAvoid,
							OutStatusEffect
						);

						IHitReactionInterface::Execute_OnHit(HitActor, OutAttackData);

						// 피격에 필요한 데이터 전달(인터페이스)
						UE_LOG(LogTemp, Warning, TEXT("Hit Bone = %s"), *Result.BoneName.ToString());

					}
				}
				else if (HitActorListCurrentAttack.Contains(HitActor))
				{
					UE_LOG(LogTemp, Warning, TEXT("Already Hit Actor"));
				}
			}
		}

		//DrawDebugCapsule(GetWorld(), CurCapsuleCenter, CurHalfHeight, Radius, CurCapsuleRotation, FColor::Red, false, 1.0f);
	}
	DetectTracePrevTime.Value = CurTime;
}

void UAttackComponent::ExecuteWeaponTrail(FWeaponSetsInfo WeaponInfo, float StartTime, float EndTime, bool IsTrailEnd, bool IsSubWeaponTrail)
{
	UAnimBoneTransformDataAsset* TraceTargetBoneTransformData = CurAttackInfo.AttackDetail[ComboIndex].BaseAttackData.TargetBoneTransformDataAsset.Get();

	FWeaponPartInfo CurrentWeaponPart = IsSubWeaponTrail ? WeaponInfo.SubWeapon : WeaponInfo.MainWeapon;

	if (!TrailPrevTime.Key)
	{
		TrailPrevTime.Key = true;
		TrailPrevTime.Value = StartTime; // 노티파이 시작 시간으로 초기값 설정
		TrailComponent->DeactivateImmediate();
		TrailComponent->ResetSystem();            // 히스토리 초기화
		TrailComponent->Activate(true);
	}

	float CurTime = !IsTrailEnd
		? AnimInstance->GetCurveValue(FName("AnimTimeCheckCurve")) - AnimInstance->GetCurveValue(FName("AnimSectionTime"))
		: EndTime;

	int32 TraceCorrectionCount = (CurTime - TrailPrevTime.Value) / 0.001f;

	const float subDT = (CurTime - TrailPrevTime.Value) / TraceCorrectionCount;

	for (int32 i = 1; i <= TraceCorrectionCount; ++i)
	{
		float PrevTime = TrailPrevTime.Value + (i * 0.001f);

		FWeaponSegment CurrentWeaponSeg;
		bool bSamplingWeaponLocation = SampleWeaponSegmentAtTime(CurTime, TraceTargetBoneTransformData, CurrentWeaponPart, CurrentWeaponSeg);

		if (bSamplingWeaponLocation)
		{
			UE_LOG(LogTemp, Warning, TEXT("TrailStart [X : %f, Y : %f, Z : %f]"), CurrentWeaponSeg.StartWS.X, CurrentWeaponSeg.StartWS.Y, CurrentWeaponSeg.StartWS.Z);

			TrailComponent->SetVectorParameter(FName("TrailStart"), CurrentWeaponSeg.StartWS);
			TrailComponent->SetVectorParameter(FName("TrailEnd"), CurrentWeaponSeg.EndWS);
			TrailComponent->AdvanceSimulation(1, subDT);
		}
	}

	TrailPrevTime.Value = CurTime;
}

bool UAttackComponent::SampleWeaponSegmentAtTime(float Time, const UAnimBoneTransformDataAsset* BoneData, const FWeaponPartInfo& WeaponPart, FWeaponSegment& OutSeg) const
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	FTransform PrevTargetBoneTransformData = BoneData->GetTransformAtTime(Time);

	FTransform CurrentRootWorldTransform = Character->GetMesh()->GetBoneTransform(0);
	FTransform TargetWeaponOffset = WeaponPart.WeaponTransform;

	FVector PrevHandLocationAtDataWorldLocation = CurrentRootWorldTransform.TransformPosition(PrevTargetBoneTransformData.GetLocation());
	FQuat PrevHandQuatAtDataWorldRotator = CurrentRootWorldTransform.TransformRotation(PrevTargetBoneTransformData.GetRotation());
	FRotator PrevHandRotatorAtQuat = PrevHandQuatAtDataWorldRotator.Rotator();

	FTransform PrevHandTransform(PrevHandRotatorAtQuat, PrevHandLocationAtDataWorldLocation, PrevTargetBoneTransformData.GetScale3D());


	FVector PrevTargetWeaponLocation = PrevHandTransform.TransformPosition(TargetWeaponOffset.GetLocation());
	FQuat PrevTargetWeaponQuat = PrevHandTransform.TransformRotation(TargetWeaponOffset.GetRotation());
	FRotator PrevTargetWeaponRotation = PrevTargetWeaponQuat.Rotator();

	FTransform PrevTargetWeaponTransform(PrevTargetWeaponRotation, PrevTargetWeaponLocation, WeaponPart.WeaponTransform.GetScale3D());

	UWeaponDataAsset* WeaponData = WeaponPart.WeaponInstance.Get();

	if (!WeaponData) return false;

	const UStaticMeshSocket* StartSocket = WeaponData->WeaponMesh->FindSocket("Start");
	const UStaticMeshSocket* EndSocket = WeaponData->WeaponMesh->FindSocket("End");

	if (!StartSocket || !EndSocket) return false;

	const FVector StartSocketRelativeLocation = StartSocket->RelativeLocation;
	const FVector EndSocketRelativeLocation = EndSocket->RelativeLocation;

	FVector PrevStartSocketWorldLocation = PrevTargetWeaponTransform.TransformPosition(StartSocketRelativeLocation);
	FVector PrevEndSocketWorldLocation = PrevTargetWeaponTransform.TransformPosition(EndSocketRelativeLocation);

	OutSeg = FWeaponSegment(PrevStartSocketWorldLocation, PrevEndSocketWorldLocation);

	return true;
}