// Fill out your copyright notice in the Description page of Project Settings.

// 엔진 헤더
#include "Characters/Player/CharacterBaseAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/Player/Components/PlayerStatusComponent.h"

// 참조할 액터
#include "Characters/Player/Warrior/FallenKnight.h" // CharacterBase로 변경예정

// Kismet 유틸리티
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// 컴포넌트
#include "Interaction/Climb/Components/ClimbComponent.h"

UCharacterBaseAnimInstance::UCharacterBaseAnimInstance()
{
	IsInAir = false;

	BoneNameToBodyType.Add("Hand_L", EBodyType::Hand_L);
	BoneNameToBodyType.Add("Hand_R", EBodyType::Hand_R);
	BoneNameToBodyType.Add("Foot_L", EBodyType::Foot_L);
	BoneNameToBodyType.Add("Foot_R", EBodyType::Foot_R);
}

void UCharacterBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<AFallenKnight>(TryGetPawnOwner());

	if (Character)
	{
		
		Character->GetCharacterStatusComponent()->OnDeath.AddUObject(this, &UCharacterBaseAnimInstance::HandleDeath);
	}
}

void UCharacterBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Character)
	{
		//IKWeights.
		IsMovementInput = Character->GetIsMovementInput();
		CurrentState = Character->GetCharacterStatusComponent()->GetCharacterState_Native();
		CurStance = Character->GetStance();
		IsAccelerating = Character->GetCharacterMovement()->GetCurrentAcceleration() != FVector::ZeroVector && Speed > 3.0f ? true : false;
		IsInAir = Character->GetMovementComponent()->IsFalling();
		IsAttack = Character->IsAttacking();
		IsRoll = Character->IsRolling();
		ComboCount = Character->CheckCombo();
		CharacterGroundState = Character->GetCharacterStatusComponent()->GetGroundStance_Native();

		switch (CurrentState)
		{
		case ECharacterState::Ground:
		{
			CurGroundStance = Character->GetCurGroundStance();

			RightFootIKAlpha = GetCurveValue(FName("EnableRightLegIK"));
			LeftFootIKAlpha = GetCurveValue(FName("EnableLeftLegIK"));

			Speed = Character->GetVelocity().Length();
			Direction = GetAnimDirection(DeltaSeconds);
				//CalculateDirection(Character->GetVelocity(), Character->GetActorRotation());
			float LocomotionAnimCurrentTime = GetCurveValue(FName("LocomotionTimeCurve"));
			float LocomotionAnimLength = GetCurveValue(FName("LocomotionAnimEntireLength"));
			LocomotionAnimTime = LocomotionAnimCurrentTime / LocomotionAnimLength;

			if (GetCurveValue(FName("TurnLock")) > 0.0f || Speed < 200.0f)
			{
				bQuickTurn = false;
			}
			else
			{
				FVector MovementDirection = Character->GetVelocity().GetSafeNormal();
				FVector AccelDirection = Character->GetLastMovementInputVector().GetSafeNormal();
				float AngleDotProduct = FVector::DotProduct(MovementDirection, AccelDirection);

				bQuickTurn = AngleDotProduct < -0.5f ? true : false;
			}

			//Character->GetCharacterMovement()->RotationRate = FRotator(0.0f, FMath::Lerp(540.0f, 150.0f, GetCurveValue(FName("TurnRate"))), 0.0f);

			if (IsAccelerating)
			{
				LastDirection = Direction;
				LastSpeed = Speed;
			}

			float BlendAlpha = CharacterGroundState == EGroundStance::Block ? 0.99f : 0.01f;
			BlockBlend = FMath::FInterpTo(BlockBlend, BlendAlpha, DeltaSeconds, 10.0f);

			TTuple<FVector, float> TraceLeftFoot = FootTrace("Foot_L");
			TTuple<FVector, float> TraceRightFoot = FootTrace("Foot_R");

			TraceLeftFoot.Value *= LeftFootIKAlpha;
			TraceRightFoot.Value *= RightFootIKAlpha;

			FootRotation(DeltaSeconds, TraceLeftFoot.Key, &LeftFootRotator, 20.0f);
			FootRotation(DeltaSeconds, TraceRightFoot.Key, &RightFootRotator, 20.0f);

			PelvisOffset = FMath::FInterpTo(PelvisOffset, UKismetMathLibrary::Min(TraceLeftFoot.Value, TraceRightFoot.Value), DeltaSeconds, 5.0f);
			LeftFootGroundOffset.Z = FMath::FInterpTo(LeftFootGroundOffset.Z, TraceLeftFoot.Value - PelvisOffset, DeltaSeconds, 5.0f);
			RightFootGroundOffset.Z = FMath::FInterpTo(RightFootGroundOffset.Z, TraceRightFoot.Value - PelvisOffset, DeltaSeconds, 5.0f);
			
			if (Speed > 100.0f)
			{
				LeftFootGroundOffset.X = FMath::FInterpTo(LeftFootGroundOffset.X, 5.0f, DeltaSeconds, 5.0f);
				RightFootGroundOffset.X = FMath::FInterpTo(RightFootGroundOffset.X, -5.0f, DeltaSeconds, 5.0f);
			}
			else
			{
				LeftFootGroundOffset.X = FMath::FInterpTo(LeftFootGroundOffset.X, 0.0f, DeltaSeconds, 5.0f);
				RightFootGroundOffset.X = FMath::FInterpTo(RightFootGroundOffset.X, 0.0f, DeltaSeconds, 5.0f);
			}
			break;
		}
		case ECharacterState::Ladder:
		{			
			CurLadderStance = Character->GetClimbComponent()->GetLadderStance_Native();

			Character->SetActorLocation(Character->GetClimbComponent()->GetLimbIKTarget(ELimbList::Body));

			LeftHandLadderOffset = Character->GetClimbComponent()->GetLimbIKTarget(ELimbList::HandL);
			FVector Hand_L_Location = Character->GetMesh()->GetSocketLocation(FName("Hand_L_Offset"));
			FVector Palm_L_Location = Character->GetMesh()->GetSocketLocation(FName("Palm_L"));
			LeftHandLadderOffset -= Palm_L_Location - Hand_L_Location;

			RightFootLadderOffset = Character->GetClimbComponent()->GetLimbIKTarget(ELimbList::FootR);
			FVector Foot_R_Location = Character->GetMesh()->GetSocketLocation(FName("Foot_R_Offset"));
			FVector Sole_R_Location = Character->GetMesh()->GetSocketLocation(FName("Sole_R"));
			RightFootLadderOffset -= Sole_R_Location - Foot_R_Location;

			RightHandLadderOffset = Character->GetClimbComponent()->GetLimbIKTarget(ELimbList::HandR);
			FVector Hand_R_Location = Character->GetMesh()->GetSocketLocation(FName("Hand_R_Offset"));
			FVector Palm_R_Location = Character->GetMesh()->GetSocketLocation(FName("Palm_R"));
			RightHandLadderOffset -= Palm_R_Location - Hand_R_Location;

			LeftFootLadderOffset = Character->GetClimbComponent()->GetLimbIKTarget(ELimbList::FootL);
			FVector Foot_L_Location = Character->GetMesh()->GetSocketLocation(FName("Foot_L_Offset"));
			FVector Sole_L_Location = Character->GetMesh()->GetSocketLocation(FName("Sole_L"));
			LeftFootLadderOffset -= Sole_L_Location - Foot_L_Location;

			UE_LOG(LogTemp, Warning, TEXT("HandLOffset = [X : %f, Y : %f, Z : %f]"),
				LeftHandLadderOffset.X,
				LeftHandLadderOffset.Y,
				LeftHandLadderOffset.Z
			);

			UE_LOG(LogTemp, Warning, TEXT("HandLLocation = [X : %f, Y : %f, Z : %f]"),
				Character->GetMesh()->GetSocketLocation("Hand_L").X,
				Character->GetMesh()->GetSocketLocation("Hand_L").Y,
				Character->GetMesh()->GetSocketLocation("Hand_L").Z
			);

			UE_LOG(LogTemp, Warning, TEXT("/////////////////////////////////////////////////////////////////////"))
			break;
		}
		case ECharacterState::Ride:
		{
			CurRideStance = Character->GetCurRideStance();

			switch (CurRideStance)
			{
			case ERideStance::Mount:
			{
				break;
			}
			case ERideStance::Riding:
			{
				Speed = Character->GetRideSpeed();
				Direction = Character->GetRideDirection();

				float LocomotionAnimCurrentTime = GetCurveValue(FName("LocomotionTimeCurve"));
				float LocomotionAnimLength = GetCurveValue(FName("LocomotionAnimEntireLength"));
				LocomotionAnimTime = LocomotionAnimCurrentTime / LocomotionAnimLength;
				break;
			}
			case ERideStance::DisMount:
			{
				break;
			}
			}
			break;
		}
		}
	}
}

TTuple<FVector, float> UCharacterBaseAnimInstance::FootTrace(FName SocketName)
{
	FVector FootLoc = Character->GetMesh()->GetSocketLocation(SocketName);
	float TraceDistance = 50.0f;
	FVector Start = FVector(FootLoc.X, FootLoc.Y, Character->GetActorLocation().Z);
	FVector End = Start - FVector(0.0f, 0.0f, Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + TraceDistance);

	FHitResult DistHitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.bTraceComplex = true;
	enum ECollisionChannel TraceChannel = Speed > 50.0f ? ECC_Visibility : ECC_GameTraceChannel8;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		DistHitResult,
		Start,
		End,
		TraceChannel,
		CollisionParams
	);

	//FColor DrawColor = bHit ? FColor::Green : FColor::Red;
	//DrawDebugLine(GetWorld(), Start, End, DrawColor, false, 1.0f);

	if (bHit)
	{
		float Offset = ((DistHitResult.ImpactPoint - DistHitResult.TraceEnd).Length()) - (TraceDistance - 3.0f);
		return MakeTuple(DistHitResult.ImpactNormal, Offset);
	}
	else
	{
		return MakeTuple(FVector::ZeroVector, 0.0f);
	}
}

void UCharacterBaseAnimInstance::FootRotation(float DeltaTime, FVector TargetNormal, FRotator *FootRotator, float fInterpSpeed)
{
	float AtanX = -1.0f * UKismetMathLibrary::DegAtan2(TargetNormal.X, TargetNormal.Z);
	float AtanY = UKismetMathLibrary::DegAtan2(TargetNormal.Y, TargetNormal.Z);
	FRotator TargetRotator = FRotator(AtanX, 0.0f, AtanY);

	*FootRotator = UKismetMathLibrary::RInterpTo(*FootRotator, TargetRotator, DeltaTime, fInterpSpeed);
}


///////// Ladder Placement /////////////
 
/* 삭제
void UCharacterBaseAnimInstance::SetLadderIK(const FName& BoneName, const FName& MiddleBoneName, FVectorCurveNameSet CurveNameList, FVector& BoneTarget, float LimbYDistance, float DeltaSeconds, float Offset, bool IsDebug)
{
	if (!BoneNameToBodyType.Contains(BoneName) || !Character->GetMesh()->DoesSocketExist(BoneName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Bone Type or Bone is not valid"));
		return;
	}

	const TOptional<TTuple<FVector, FVector>> GripLoc = Character->GetLadderIKTargetLoc(BoneNameToBodyType[BoneName]);

	// 본 설정이 잘못되거나 손잡이에 대한 정보가 없는 경우
	// EX) 사다리에 올라타지 않은 상태, 본 이름이 잘못 입력된 상태
	if (!GripLoc.IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Grip Not Set"));
		return;
	}

	FVector TargetLoc;
	FVector ForwardVector = Character->GetActorForwardVector();
	FVector RightVector = Character->GetActorRightVector();

	TArray<FName> ActiveCurveNames;
	// 현재 애니메이션에서 활성화된 Float 커브 목록 가져오기
	GetActiveCurveNames(EAnimCurveType::AttributeCurve, ActiveCurveNames);

	if (ActiveCurveNames.Contains(CurveNameList.CurveNameZ))
	{
		TargetLoc = FMath::Lerp(GripLoc.GetValue().Get<1>(), GripLoc.GetValue().Get<0>(), GetCurveValue(CurveNameList.CurveNameZ));
	}
	else
	{
		TargetLoc = GripLoc.GetValue().Get<0>();
	}

	FVector BoneLoc = Character->GetMesh()->GetSocketLocation(BoneName);
	FVector AdjustBoneLoc = FVector::ZeroVector;

	if (Character->GetMesh()->DoesSocketExist(MiddleBoneName))
	{
		FVector MiddleBoneLoc = Character->GetMesh()->GetSocketLocation(MiddleBoneName);
		AdjustBoneLoc = (MiddleBoneLoc - BoneLoc) * Offset;
	} 

	TargetLoc -= AdjustBoneLoc;

	FVector ForwardOffset = (ForwardVector * LimbYDistance) * GetCurveValue(CurveNameList.CurveNameY);

	BoneTarget = TargetLoc + ((RightVector * GetCurveValue(CurveNameList.CurveNameX)) + ((ForwardVector * LimbYDistance) * GetCurveValue(CurveNameList.CurveNameY)));

	if (IsDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurveValue : X = %f, Y = %f, Z = %f"), GetCurveValue(CurveNameList.CurveNameX), GetCurveValue(CurveNameList.CurveNameY), GetCurveValue(CurveNameList.CurveNameZ));
		UE_LOG(LogTemp, Warning, TEXT("TargetLoc : X = %f, Y = %f, Z = %f"), TargetLoc.X, TargetLoc.Y, TargetLoc.Z);
		UE_LOG(LogTemp, Warning, TEXT("Bone Target : X = %f, Y = %f, Z = %f"), BoneTarget.X, BoneTarget.Y, BoneTarget.Z);
		UE_LOG(LogTemp, Warning, TEXT("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ"));
	}
}
*/

void UCharacterBaseAnimInstance::CheckIKValid(FName CurveName, float& AlphaValue, float DeltaSeconds)
{
	AlphaValue = 1.0f - GetCurveValue(CurveName);
}

void UCharacterBaseAnimInstance::SetHitAir(bool HitState)
{
	bIsHitAir = HitState;
}

void UCharacterBaseAnimInstance::ResetHitAir_Implementation()
{
	SetHitAir(false);
}

void UCharacterBaseAnimInstance::SetPitch()
{
	FRotator PawnRotation = Character->GetActorRotation();
	FRotator AimRotation = Character->GetBaseAimRotation();
	Pitch = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, PawnRotation).Pitch;
}

void UCharacterBaseAnimInstance::SetRootYawOffset()
{
	if (Speed > 0.f || IsAnyMontagePlaying())
	{
		IsMove = true;
		RootYawOffset = 0.0f;
		return;
	}

	YawLastTick = Yaw;
	Yaw = Character->GetActorRotation().Yaw;
	if (IsMove)
	{
		YawLastTick = Yaw;
		IsMove = false;
	}
	YawChangeOverFrame = YawLastTick - Yaw;

	RootYawOffset = UKismetMathLibrary::NormalizeAxis(YawChangeOverFrame + RootYawOffset);

	if (GetCurveValue(Turning) > 0.0f)
	{
		DistanceCurveValueLastFrame = DistanceCurveValue;
		DistanceCurveValue = GetCurveValue(DistanceCurve);

		(RootYawOffset > 0.0f) ? TurnDirection = -1.0f : TurnDirection = 1.0f;
		DistanceCurveDifference = DistanceCurveValueLastFrame - DistanceCurveValue;

		RootYawOffset = RootYawOffset - (DistanceCurveDifference * TurnDirection);
		ABSRootYawOffset = UKismetMathLibrary::Abs(RootYawOffset);
		if (ABSRootYawOffset > MaxTurnAngle)
		{
			YawToSubtract = ABSRootYawOffset - MaxTurnAngle;
			YawMultiplier = 0.0f;
			(RootYawOffset > 0.0f) ? YawMultiplier = 1.0f : YawMultiplier = -1.0f;
			YawToSubtract = YawToSubtract * YawMultiplier;
			RootYawOffset = RootYawOffset - YawToSubtract;
		}
	}

	return;
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_ParryEnd()
{
	OnParryEnd.Broadcast();
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_NextAttack()
{
	OnNextAttackCheck.Broadcast();
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_SetAttackDirection()
{
	OnSetAttackDirection.Broadcast();
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_EndAttack()
{
	OnEndAttack.Broadcast();
}

void UCharacterBaseAnimInstance::ResetTurn_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Disable Root Motion"));
	SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_EnableRootLock()
{
	SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
	UE_LOG(LogTemp, Warning, TEXT("Enable Root Motion"));
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_DisableRootLock()
{
	SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_TurnStart()
{
	Character->GetController()->SetIgnoreMoveInput(true);
	UE_LOG(LogTemp, Warning, TEXT("TurnStart"));
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_TurnEnd()
{
	//SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
	
	UE_LOG(LogTemp, Warning, TEXT("TurnEnd"));
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_CheckLadderStance()
{
	//CurLadderStance = Character->GetCurLadderStance();
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_EnableInputLock()
{
	//Character->EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	Character->SetCanMovementInput(true);
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_ClimbEnd()
{
	OnClimbEnd.Broadcast();
	bIsClimb = false;
	//CurLadderStance = Character->GetCurLadderStance();
	UE_LOG(LogTemp, Warning, TEXT("ClimbEnd"));
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_ClimbStart()
{
	CurveValue_Root_Z = 0.0f;
	CurveValue_Root_Y = 0.0f;
	Hand_L_Y_Distance = FMath::RandRange(-15.0f, -30.0f);
	Hand_R_Y_Distance = FMath::RandRange(-15.0f, -30.0f);
	Foot_L_Y_Distance = FMath::RandRange(-15.0f, -30.0f);
	Foot_R_Y_Distance = FMath::RandRange(-15.0f, -30.0f);
	bIsClimb = true;
	UE_LOG(LogTemp, Warning, TEXT("ClimbStart"));
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_ResetClimbState()
{
	UE_LOG(LogTemp, Warning, TEXT("ResetClimbState"));
	Character->GetClimbComponent()->ResetClimbState();
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_MountEnd()
{
	OnMountEnd.Broadcast();
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_DisMountEnd()
{
	OnDisMountEnd.Broadcast();
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_ResetHurt()
{
	//UE_LOG(LogTemp, Warning, TEXT("ResetHurt"));
	OnResetHurt.Broadcast();
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_EnterLocomotion()
{
	OnEnterLocomotion.ExecuteIfBound();
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_LeftLocomotion()
{
	//UE_LOG(LogTemp, Warning, TEXT("ResetHurt"));
	OnLeftLocomotion.ExecuteIfBound();
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_AttackStart()
{
	OnAttackStart.Broadcast();
}

FName UCharacterBaseAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Light%d"), Section));
}

void UCharacterBaseAnimInstance::SetIKWeight_Implementation(EIKContext IKContext, ELimbList Limb, float Weight)
{
	switch (IKContext)
	{
	case EIKContext::Ground:
	{
		GroundIKWeights.Set(Limb, Weight);
		break;
	}
	case EIKContext::Ladder:
	{
		LadderIKWeights.Set(Limb, Weight);
		break;
	}
	default:
		return;
	}
}

float UCharacterBaseAnimInstance::GetIKWeight_Implementation(EIKContext IKContext, ELimbList Limb)
{
	switch (IKContext)
	{
	case EIKContext::Ground:
	{
		return GroundIKWeights.Get(Limb);
	}
	case EIKContext::Ladder:
	{
		return LadderIKWeights.Get(Limb);
	}
	default:
		return 0.0f;
	}
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_EnterWalkState()
{
	OnEnterWalkState.Broadcast();
	SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
}


float UCharacterBaseAnimInstance::GetAnimDirection(float DeltaSeconds)
{
	if (Speed > 200.0f)
	{
		float PrevCharacterYaw = CharacterYaw;
		CharacterYaw = Character->GetActorRotation().Yaw;

		float DeltaCharacterYaw = CharacterYaw - PrevCharacterYaw;
		float LeanAngle = FMath::Clamp((UKismetMathLibrary::SafeDivide(DeltaCharacterYaw, DeltaSeconds) / 200.0f), -1.0f, 1.0f);


		return LeanAngle;
	}
	else
	{
		return 0.0f;
	}
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_EnterIdleState()
{
	//CurLadderStance = Character->GetCurLadderStance();
	CanMovementInput = true;
	//UE_LOG(LogTemp, Warning, TEXT("EnterIdleState"));
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_LeftIdleState()
{
	CanMovementInput = false;
	//UE_LOG(LogTemp, Warning, TEXT("LeftIdleState"));
}