// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Components/PlayerAttackComponent.h"
#include "GameFramework/Character.h"
#include "Utils/AnimBoneTransformDataAsset.h"
#include "Engine/StaticMeshSocket.h"
#include "Combat/Interfaces/HitReactionInterface.h"

void UPlayerAttackComponent::ExecuteAttack(FName AttackName, float Playrate)
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


void UPlayerAttackComponent::PlayAnimation(FPlayerAttackInfo AttackInfo, int32 index, float Playrate)
{
	UE_LOG(LogTemp, Warning, TEXT("Playrate = %f"), Playrate);
	AnimInstance->Montage_Play(AttackInfo.Anim, Playrate);
	AnimInstance->Montage_JumpToSection(AttackInfo.AttackDetail[index].SectionName);
	//AnimInstance->Montage_SetPlayRate(AttackInfo.Anim, Playrate);

	FOnMontageEnded MontageEndDelegate;
	MontageEndDelegate.BindUObject(this, &UPlayerAttackComponent::OnMontageEnded);
	AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, AttackInfo.Anim);

	//AttackInfo.Anim->anim
}

void UPlayerAttackComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Warning, TEXT("공격상태 초기화"));
	if (!bInterrupted)
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

void UPlayerAttackComponent::DetectAttackTarget(UStaticMeshComponent* WeaponMesh, FWeaponPartInfo WeaponInfo, float StartTime, float EndTime)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	UAnimBoneTransformDataAsset* TraceTargetBoneTransformData = CurAttackInfo.AttackDetail[ComboIndex].TargetBoneTransformDataAsset.LoadSynchronous();
	
	if (!DetectTracePrevTime.Key)
	{
		DetectTracePrevTime.Key = true;
		DetectTracePrevTime.Value = StartTime;
	}

	float CurTime = AnimInstance->GetCurveValue(FName("AnimTimeCheckCurve")) - AnimInstance->GetCurveValue(FName("AnimSectionTime"));;
	float BaseTime = CurTime;
	int32 TraceCorrectionCount = (CurTime - DetectTracePrevTime.Value) / 0.001f;
	
	FTransform CurrentRootWorldTransform = Character->GetMesh()->GetBoneTransform(0);

	FVector CurrentHandLocationAtData = TraceTargetBoneTransformData->GetTransformAtTime(CurTime).GetLocation();
	FVector CurrentHandLocationAtDataWorldTransform = CurrentRootWorldTransform.TransformPosition(CurrentHandLocationAtData);


	FTransform TargetWeaponOffset = WeaponInfo.WeaponTransform;

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

	FTransform CurrentTargetWeaponTransform(CurrentTargetWeaponRotation, CurrentTargetWeaponLocation, WeaponInfo.WeaponTransform.GetScale3D());

	FVector CurrentStartSocketWorldLocation = CurrentTargetWeaponTransform.TransformPosition(StartSocketRelativeLocation);
	FVector CurrentEndSocketWorldLocation = CurrentTargetWeaponTransform.TransformPosition(EndSocketRelativeLocation);


	FName StartSocketName = FName("Start");
	FName EndSocketName = FName("End");

	FVector StartSocketLocation = WeaponMesh->GetSocketLocation(StartSocketName);
	FVector EndSocketLocation = WeaponMesh->GetSocketLocation(EndSocketName);

	//DrawDebugSphere(GetWorld(), StartSocketLocation, 3.0f, 12, FColor::Red, false, 3.0f);
	//DrawDebugSphere(GetWorld(), EndSocketLocation, 3.0f, 12, FColor::Red, false, 3.0f);
	//DrawDebugLine(GetWorld(), StartSocketLocation, EndSocketLocation, FColor::Black, false, 3.0f);

	FVector StartLoc = StartSocketLocation;
	FVector EndLoc = EndSocketLocation;
	float Radius = WeaponInfo.HitBoxRadius;

	float CurWeaponLength = FVector::Distance(StartLoc, EndLoc);
	float CurHalfHeight = (CurWeaponLength * 0.5f);

	FVector CurCapsuleCenter = (StartLoc + EndLoc) * 0.5f;
	FVector CurCapsuleAxis = (EndLoc - StartLoc).GetSafeNormal();
	FQuat CurCapsuleRotation = FRotationMatrix::MakeFromZ(CurCapsuleAxis).ToQuat();

	DrawDebugCapsule(GetWorld(), CurCapsuleCenter, CurHalfHeight, Radius, CurCapsuleRotation, FColor::Green, false, 1.0f);

	FVector HandLocation = Character->GetMesh()->GetSocketLocation(FName("Hand_R"));
	FVector WeaponLocation = Character->GetMesh()->GetSocketLocation(FName("S_Sword"));
	FVector TestLocation = Character->GetMesh()->GetSocketLocation(FName("WeaponEnd"));

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

		FTransform PrevTargetWeaponTransform(PrevTargetWeaponRotation, PrevTargetWeaponLocation, WeaponInfo.WeaponTransform.GetScale3D());

		FVector PrevStartSocketWorldLocation = PrevTargetWeaponTransform.TransformPosition(StartSocketRelativeLocation);
		FVector PrevEndSocketWorldLocation = PrevTargetWeaponTransform.TransformPosition(EndSocketRelativeLocation);

		FVector StartSocketLocationDifference = CurrentStartSocketWorldLocation - PrevStartSocketWorldLocation;
		FVector EndSocketLocationDifference = CurrentEndSocketWorldLocation - PrevEndSocketWorldLocation;

		FVector StartTraceLocation = (StartSocketLocation - StartSocketLocationDifference);
		FVector EndTraceLocation = (EndSocketLocation - EndSocketLocationDifference);

		//DrawDebugSphere(GetWorld(), StartTraceLocation, 3.0f, 12, FColor::Blue, false, 3.0f);
		//DrawDebugSphere(GetWorld(), EndTraceLocation, 3.0f, 12, FColor::Blue, false, 3.0f);
		//DrawDebugLine(GetWorld(), StartTraceLocation, EndTraceLocation, FColor::Black, false, 3.0f);

		StartLoc = StartTraceLocation;
		EndLoc = EndTraceLocation;

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
			//float OutDamage = 
			//FAttackRequest OutAttackData();
			for (const FHitResult& Result : HitResults)
			{
				AActor* HitActor = Result.GetActor();
				if (HitActor && !HitActorListCurrentAttack.Contains(HitActor))
				{
					HitActorListCurrentAttack.Add(HitActor);

					if (HitActor->Implements<UHitReactionInterface>())
					{
						//IHitReactionInterface::Execute_OnHit(HitActor);

					// 피격에 필요한 데이터 전달(인터페이스)
						UE_LOG(LogTemp, Warning, TEXT("Hit Bone = %s"), *Result.BoneName.ToString());

					}
					else if (HitActorListCurrentAttack.Contains(HitActor))
					{
						UE_LOG(LogTemp, Warning, TEXT("Already Hit Actor"));
					}
				}
			}
		}

		DrawDebugCapsule(GetWorld(), CurCapsuleCenter, CurHalfHeight, Radius, CurCapsuleRotation, FColor::Red, false, 1.0f);
	}

	DetectTracePrevTime.Value = CurTime;
}
