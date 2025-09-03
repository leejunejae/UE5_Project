// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Components/PlayerAttackComponent.h"
#include "GameFramework/Character.h"
#include "Utils/AnimBoneTransformDataAsset.h"
#include "Engine/StaticMeshSocket.h"
#include "Combat/Interfaces/HitReactionInterface.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

void UPlayerAttackComponent::ExecuteAttack(FName AttackName, float Playrate)
{
	bool CanPlayAttack = false;

	//if(CurAttackInfo.IsValid())
		//AnimInstance->OnMontageEnded.RemoveDynamic(this, &UPlayerAttackComponent::OnMontageEnded);

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

void UPlayerAttackComponent::PlayAnimation(FPlayerAttackInfo AttackInfo, int32 index, float Playrate)
{
	AnimInstance->Montage_Play(AttackInfo.Anim, Playrate);
	AnimInstance->Montage_JumpToSection(AttackInfo.AttackDetail[index].BaseAttackData.SectionName, AttackInfo.Anim);

	FOnMontageEnded MontageEndDelegate;
	MontageEndDelegate.BindUObject(this, &UPlayerAttackComponent::OnMontageEnded);
	AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, AttackInfo.Anim);
}

void UPlayerAttackComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
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

	AnimInstance->OnMontageEnded.RemoveDynamic(this, &UPlayerAttackComponent::OnMontageEnded);
}

void UPlayerAttackComponent::SetCurAttackType(EWeaponType WeaponType)
{
	const UPlayerAttackDataAsset* TypedAsset = Cast<UPlayerAttackDataAsset>(AttackListDA);

	if (!TypedAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackListDA 캐스팅 실패"));
		return;
	}

	// 현재 
	CurAttackType = TypedAsset->FindPlayerAttackInfo(WeaponType);
}

void UPlayerAttackComponent::DetectAttackTarget(UStaticMeshComponent* WeaponMesh, FWeaponSetsInfo WeaponInfo, float StartTime, float EndTime, bool IsDetectEnd, bool IsSubWeaponAttack)
{
	UE_LOG(LogTemp, Warning, TEXT("DetectTarget"));

	ACharacter* Character = Cast<ACharacter>(GetOwner());

	UAnimBoneTransformDataAsset* TraceTargetBoneTransformData = CurAttackInfo.AttackDetail[ComboIndex].BaseAttackData.TargetBoneTransformDataAsset.LoadSynchronous();

	FWeaponPartInfo CurrentWeaponPart = IsSubWeaponAttack ? WeaponInfo.SubWeapon : WeaponInfo.MainWeapon;
	
	if (!DetectTracePrevTime.Key)
	{
		DetectTracePrevTime.Key = true;
		DetectTracePrevTime.Value = StartTime;
	}

	float CurTime = !IsDetectEnd
		? AnimInstance->GetCurveValue(FName("AnimTimeCheckCurve")) - AnimInstance->GetCurveValue(FName("AnimSectionTime"))
		: EndTime;

	int32 TraceCorrectionCount = (CurTime - DetectTracePrevTime.Value) / 0.001f;
	
	FTransform CurrentRootWorldTransform = Character->GetMesh()->GetBoneTransform(0);

	FVector CurrentHandLocationAtData = TraceTargetBoneTransformData->GetTransformAtTime(CurTime).GetLocation();
	FVector CurrentHandLocationAtDataWorldTransform = CurrentRootWorldTransform.TransformPosition(CurrentHandLocationAtData);


	FTransform TargetWeaponOffset = CurrentWeaponPart.WeaponTransform;

	const UStaticMeshSocket* StartSocket = WeaponMesh->GetStaticMesh()->FindSocket("Start");
	const UStaticMeshSocket* EndSocket = WeaponMesh->GetStaticMesh()->FindSocket("End");

	if (!StartSocket || !EndSocket) return;

	const FVector StartSocketRelativeLocation = StartSocket->RelativeLocation;
	const FVector EndSocketRelativeLocation = EndSocket->RelativeLocation;


	FTransform CurrentTargetBoneTransformData = TraceTargetBoneTransformData->GetTransformAtTime(CurTime);

	FVector CurrentHandLocationAtDataWorldLocation = CurrentRootWorldTransform.TransformPosition(CurrentTargetBoneTransformData.GetLocation());
	FQuat CurrentHandQuatAtDataWorldRotator = CurrentRootWorldTransform.TransformRotation(CurrentTargetBoneTransformData.GetRotation());
	FRotator CurrentHandRotatorAtQuat = CurrentHandQuatAtDataWorldRotator.Rotator();

	FTransform CurrentHandTransform(CurrentHandRotatorAtQuat, CurrentHandLocationAtDataWorldLocation, CurrentTargetBoneTransformData.GetScale3D());

	FVector CurrentTargetWeaponLocation = CurrentHandTransform.TransformPosition(TargetWeaponOffset.GetLocation());
	FQuat CurrentTargetWeaponQuat = CurrentHandTransform.TransformRotation(TargetWeaponOffset.GetRotation());
	FRotator CurrentTargetWeaponRotation = CurrentTargetWeaponQuat.Rotator();

	FTransform CurrentTargetWeaponTransform(CurrentTargetWeaponRotation, CurrentTargetWeaponLocation, CurrentWeaponPart.WeaponTransform.GetScale3D());

	FVector CurrentStartSocketWorldLocation = CurrentTargetWeaponTransform.TransformPosition(StartSocketRelativeLocation);
	FVector CurrentEndSocketWorldLocation = CurrentTargetWeaponTransform.TransformPosition(EndSocketRelativeLocation);

	FName StartSocketName = FName("Start");
	FName EndSocketName = FName("End");

	FVector StartLoc = CurrentStartSocketWorldLocation;
	FVector EndLoc = CurrentEndSocketWorldLocation;

	float Radius = CurrentWeaponPart.HitBoxRadius;

	float CurWeaponLength = FVector::Distance(StartLoc, EndLoc);

	float CurHalfHeight = (CurWeaponLength * 0.5f);

	FVector CurCapsuleCenter = (StartLoc + EndLoc) * 0.5f;
	FVector CurCapsuleAxis = (EndLoc - StartLoc).GetSafeNormal();

	FQuat CurCapsuleRotation = FRotationMatrix::MakeFromZ(CurCapsuleAxis).ToQuat();

	DrawDebugCapsule(GetWorld(), CurCapsuleCenter, CurHalfHeight, Radius, CurCapsuleRotation, FColor::Green, false, 1.0f);

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

		StartLoc = PrevStartSocketWorldLocation;
		EndLoc = PrevEndSocketWorldLocation;

		CurWeaponLength = FVector::Distance(StartLoc, EndLoc);
		CurHalfHeight = (CurWeaponLength * 0.5f);

		CurCapsuleCenter = (StartLoc + EndLoc) * 0.5f;
		CurCapsuleAxis = (EndLoc - StartLoc).GetSafeNormal();
		CurCapsuleRotation = FRotationMatrix::MakeFromZ(CurCapsuleAxis).ToQuat();

		TArray<FHitResult> HitResults;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());

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

		DrawDebugCapsule(GetWorld(), CurCapsuleCenter, CurHalfHeight, Radius, CurCapsuleRotation, FColor::Red, false, 1.0f);
	}
	DetectTracePrevTime.Value = CurTime;
}
