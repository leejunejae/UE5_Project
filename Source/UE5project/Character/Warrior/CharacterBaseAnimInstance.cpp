// Fill out your copyright notice in the Description page of Project Settings.

// 엔진 헤더
#include "CharacterBaseAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../Function/CharacterStatusComponent.h"

// 참조할 액터
#include "FallenKnight.h" // CharacterBase로 변경예정

// Kismet 유틸리티
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// 컴포넌트
#include "../../Function/Interact/ClimbComponent.h"

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

	SeedSwitch = true;
}

void UCharacterBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Character)
	{
		IsMovementInput = Character->GetIsMovementInput();
		CurrentState = Character->GetCurrentState();
		CurStance = Character->GetStance();
		IsAccelerating = Character->GetCharacterMovement()->GetCurrentAcceleration() != FVector::ZeroVector && Speed > 3.0f ? true : false;
		IsInAir = Character->GetMovementComponent()->IsFalling();
		IsAttack = Character->IsAttacking();
		IsAttackInput = Character->IsAttackInput();
		IsBlock = Character->IsBlocking();
		IsParry = Character->IsParrying();
		IsRoll = Character->IsRolling();
		Response = Character->GetCharResponse();
		ComboCount = Character->CheckCombo();

		UCharacterStatusComponent* StatusComp = Character->FindComponentByClass<UCharacterStatusComponent>();
		if (StatusComp)
		{
			CharacterCombatState = StatusComp->GetCombatState();
		}

		//if (FMath::IsNearlyEqual(Speed, 0.0f) && CurrentState==ECharacterState::Ride)
		//{
		//	Speed = LastSpeed;
		//}

		switch (CurrentState)
		{
		case ECharacterState::Ground:
		{
			CurGroundStance = Character->GetCurGroundStance();

			RightHandIKTypeAlpha = FMath::FInterpTo(RightHandIKTypeAlpha, 0.0f, DeltaSeconds, 5.0f);
			LeftHandIKTypeAlpha = FMath::FInterpTo(LeftHandIKTypeAlpha, 0.0f, DeltaSeconds, 5.0f);
			RightFootIKTypeAlpha = FMath::FInterpTo(RightFootIKTypeAlpha, 0.0f, DeltaSeconds, 5.0f);
			LeftFootIKTypeAlpha = FMath::FInterpTo(LeftFootIKTypeAlpha, 0.0f, DeltaSeconds, 5.0f);

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
			else
			{

			}
			InputDirectionX = Character->GetInputDirection().X;
			InputDirectionY = Character->GetInputDirection().Y;

			if (InputDirectionX == 0.0f && InputDirectionY == 0.0f)
				InputDirection = 180.0f;
			else
				InputDirection = UKismetMathLibrary::DegAtan2(InputDirectionX, InputDirectionY);

			float BlendAlpha = CharacterCombatState == ECharacterCombatState::Block ? 0.99f : 0.01f;
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
			CurLadderStance = Character->GetCurLadderStance();

			RightFootIKAlpha = GetCurveValue(FName("EnableRightLegIK"));
			LeftFootIKAlpha = GetCurveValue(FName("EnableLeftLegIK"));

			if (GetCurveValue(LockIK) > 0.0f && bIsClimb)
			{
				const float Translation_CurveValue_Z = GetCurveValue(FName("Char_Translation_Z"));

				if (Translation_CurveValue_Z > 0.0f && !(CurLadderStance==ELadderStance::Enter_From_Top))
				{
					float Prev_Curve_Value = CurveValue_Root_Z;
					CurveValue_Root_Z = Translation_CurveValue_Z;
					float CurveDifference = CurveValue_Root_Z - Prev_Curve_Value;
					RootCurveDifferenceSum += CurveDifference;
					FVector NewCharLocation = Character->GetActorLocation();
					NewCharLocation.Z += Character->GetClimbDistance() * CurveDifference;
					Character->SetActorLocation(NewCharLocation);
				}

				switch (CurLadderStance)
				{
				case ELadderStance::Enter_From_Top:
				{
					CheckIKValid(FName("Disable_FootIK_L"), LeftFootIKTypeAlpha, DeltaSeconds);
					CheckIKValid(FName("Disable_FootIK_R"), RightFootIKTypeAlpha, DeltaSeconds);
					CheckIKValid(FName("Disable_HandIK_L"), LeftHandIKTypeAlpha, DeltaSeconds);
					CheckIKValid(FName("Disable_HandIK_R"), RightHandIKTypeAlpha, DeltaSeconds);

					const FVector InitLocation = Character->GetClimbComponent()->GetInitTopPosition().GetValue().GetLocation() + FVector(0.0f, 0.0f, Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
					const FVector TargetLocation = Character->GetClimbComponent()->GetEnterTopPosition().GetValue().GetLocation();
					const float Translation_CurveValue_Y = GetCurveValue(FName("Char_Translation_Y"));
					const float Translation_Value_Y = Character->GetClimbComponent()->GetLadderTopTransitionDistance();
					const float Rotation_CurveValue = GetCurveValue(FName("Char_Rotation_Z"));

					FVector NewCharLocation = InitLocation;

					const FVector EnterDirection = FVector(TargetLocation.X - InitLocation.X, TargetLocation.Y - InitLocation.Y, 0.0f);

					if (Translation_CurveValue_Y > 0.0f)
					{
						NewCharLocation = FMath::Lerp(InitLocation, TargetLocation, Translation_CurveValue_Y);
					}

					if (Translation_CurveValue_Z > 0.0f)
					{
						NewCharLocation.Z = FMath::Lerp(InitLocation.Z, TargetLocation.Z, Translation_CurveValue_Z);
						Character->SetActorLocation(NewCharLocation);
					}

					if (Rotation_CurveValue > 0.0f)
					{
						const FRotator InitRotator = Character->GetClimbComponent()->GetInitTopPosition().GetValue().GetRotation().Rotator();
						const FRotator TargetRotator = Character->GetClimbComponent()->GetEnterTopPosition().GetValue().GetRotation().Rotator();
						const float InitYaw = InitRotator.Yaw > 0.0f ? FMath::Fmod(InitRotator.Yaw, 360.0f) : FMath::Fmod(InitRotator.Yaw, 360.0f) + 360.0f;
						const float TargetYaw = TargetRotator.Yaw > 0.0f ? FMath::Fmod(TargetRotator.Yaw, 360.0f) : FMath::Fmod(TargetRotator.Yaw, 360.0f) + 360.0f;

						const float NewRotatorYaw = FMath::Lerp(InitYaw, TargetYaw, Rotation_CurveValue);
						const FRotator NewRotator = FRotator(Character->GetActorRotation().Pitch, NewRotatorYaw, Character->GetActorRotation().Roll);
						Character->SetActorRotation(NewRotator);
					}



					SetLadderIK(FName("Hand_L"), FName("Palm_L"), Hand_L_CurveNameSet, LeftHandLadderOffset, 0.0f, DeltaSeconds);

					if (GetCurveValue(Hand_R_CurveNameSet.CurveNameZ) > -1.0f)
					{
						SetLadderIK(FName("Hand_R"), FName("Palm_R"), Hand_R_CurveNameSet, RightHandLadderOffset, Hand_R_Y_Distance, DeltaSeconds, 1.0f);
					}

					if (GetCurveValue(Foot_L_CurveNameSet.CurveNameZ) > -1.0f)
					{
						SetLadderIK(FName("Foot_L"), FName("ball_l"), Foot_L_CurveNameSet, LeftFootLadderOffset, Foot_L_Y_Distance, DeltaSeconds, 0.5f);
					}

					if (GetCurveValue(Foot_R_CurveNameSet.CurveNameZ) > -1.0f)
					{
						SetLadderIK(FName("Foot_R"), FName("ball_r"), Foot_R_CurveNameSet, RightFootLadderOffset, Foot_R_Y_Distance, DeltaSeconds, 0.5f);
					}

					break;
				}
				case ELadderStance::Exit_From_Top:
				{
					CheckIKValid(FName("Disable_FootIK_L"), LeftFootIKTypeAlpha, DeltaSeconds);
					CheckIKValid(FName("Disable_FootIK_R"), RightFootIKTypeAlpha, DeltaSeconds);
					CheckIKValid(FName("Disable_HandIK_L"), LeftHandIKTypeAlpha, DeltaSeconds);
					CheckIKValid(FName("Disable_HandIK_R"), RightHandIKTypeAlpha, DeltaSeconds);

					const FVector InitLocation = Character->GetClimbComponent()->GetEnterTopPosition().GetValue().GetLocation();
					const FVector TargetLocation = Character->GetClimbComponent()->GetInitTopPosition().GetValue().GetLocation();
					const float Translation_CurveValue_Y = GetCurveValue(FName("Char_Translation_Y"));
					const float Translation_Value_Y = Character->GetClimbComponent()->GetLadderTopTransitionDistance();

					const FVector EnterDirection = FVector(TargetLocation.X - InitLocation.X, TargetLocation.Y - InitLocation.Y, 0.0f).GetSafeNormal();

					if (Translation_CurveValue_Y > 0.0f)
					{
						float Prev_CurveValue_Root_Y = CurveValue_Root_Y;
						CurveValue_Root_Y = Translation_CurveValue_Y;
						float CurveValue_Root_Y_Difference = CurveValue_Root_Y - Prev_CurveValue_Root_Y;
						FVector NewCharLocation = Character->GetActorLocation() + (EnterDirection * Translation_Value_Y * CurveValue_Root_Y_Difference);
						Character->SetActorLocation(NewCharLocation);
					}

					if (GetCurveValue(Hand_R_CurveNameSet.CurveNameZ) > 1.0f)
					{
						SetLadderIK(FName("Hand_L"), FName("Palm_L"), Hand_L_CurveNameSet, LeftHandLadderOffset, Hand_L_Y_Distance, DeltaSeconds);
					}
					SetLadderIK(FName("Hand_R"), FName("Palm_R"), Hand_R_CurveNameSet, RightHandLadderOffset, Hand_R_Y_Distance, DeltaSeconds, 1.0f);
					SetLadderIK(FName("Foot_L"), FName("ball_l"), Foot_L_CurveNameSet, LeftFootLadderOffset, Foot_L_Y_Distance, DeltaSeconds, 0.5f, true);
					SetLadderIK(FName("Foot_R"), FName("ball_r"), Foot_R_CurveNameSet, RightFootLadderOffset, Foot_R_Y_Distance, DeltaSeconds, 0.5f);
					break;
				}
				case ELadderStance::Enter_From_Bottom:
				{
					CheckIKValid(FName("Disable_FootIK_R"), RightFootIKTypeAlpha, DeltaSeconds);
					CheckIKValid(FName("Disable_HandIK_L"), LeftHandIKTypeAlpha, DeltaSeconds);
					CheckIKValid(FName("Disable_HandIK_R"), RightHandIKTypeAlpha, DeltaSeconds);

					LeftFootIKTypeAlpha = 0.0f;

					TTuple<FVector, float> TraceLeftFoot = FootTrace("Foot_L");

					TraceLeftFoot.Value *= LeftFootIKAlpha;

					FootRotation(DeltaSeconds, TraceLeftFoot.Key, &LeftFootRotator, 20.0f);
					
					PelvisOffset = FMath::FInterpTo(PelvisOffset, TraceLeftFoot.Value, DeltaSeconds, 5.0f);
					LeftFootGroundOffset.Z = FMath::FInterpTo(LeftFootGroundOffset.Z, TraceLeftFoot.Value - PelvisOffset, DeltaSeconds, 5.0f);

					SetLadderIK(FName("Hand_L"), FName("Palm_L"), Hand_L_CurveNameSet, LeftHandLadderOffset, 0.0f, DeltaSeconds);
					SetLadderIK(FName("Hand_R"), FName("Palm_R"), Hand_R_CurveNameSet, RightHandLadderOffset, 0.0f, DeltaSeconds, 1.0f);
					SetLadderIK(FName("Foot_R"), FName("ball_r"), Foot_R_CurveNameSet, RightFootLadderOffset, 0.0f, DeltaSeconds, 0.5f);
					break;
				}
				case ELadderStance::Exit_From_Bottom:
				{
					CheckIKValid(FName("Disable_FootIK_R"), RightFootIKTypeAlpha, DeltaSeconds);
					CheckIKValid(FName("Disable_HandIK_L"), LeftHandIKTypeAlpha, DeltaSeconds);
					CheckIKValid(FName("Disable_HandIK_R"), RightHandIKTypeAlpha, DeltaSeconds);

					TTuple<FVector, float> TraceLeftFoot = FootTrace("Foot_L");
					TTuple<FVector, float> TraceRightFoot = FootTrace("Foot_R");

					TraceLeftFoot.Value *= LeftFootIKAlpha;
					TraceRightFoot.Value *= RightFootIKAlpha;

					FootRotation(DeltaSeconds, TraceLeftFoot.Key, &LeftFootRotator, 20.0f);
					FootRotation(DeltaSeconds, TraceRightFoot.Key, &RightFootRotator, 20.0f);

					PelvisOffset = FMath::FInterpTo(PelvisOffset, UKismetMathLibrary::Min(TraceLeftFoot.Value, TraceRightFoot.Value), DeltaSeconds, 5.0f);
					LeftFootGroundOffset.Z = FMath::FInterpTo(LeftFootGroundOffset.Z, TraceLeftFoot.Value - PelvisOffset, DeltaSeconds, 5.0f);
					RightFootGroundOffset.Z = FMath::FInterpTo(RightFootGroundOffset.Z, TraceRightFoot.Value - PelvisOffset, DeltaSeconds, 5.0f);

					break;
				}
				case ELadderStance::ClimbDown_OneStep:
				{
					CheckIKValid(FName("Disable_FootIK_L"), LeftFootIKTypeAlpha, DeltaSeconds);

					TTuple<FVector, float> TraceLeftFoot = FootTrace("Foot_L");

					TraceLeftFoot.Value *= LeftFootIKAlpha;

					FootRotation(DeltaSeconds, TraceLeftFoot.Key, &LeftFootRotator, 20.0f);

					PelvisOffset = FMath::FInterpTo(PelvisOffset, TraceLeftFoot.Value, DeltaSeconds, 5.0f);
					LeftFootGroundOffset.Z = FMath::FInterpTo(LeftFootGroundOffset.Z, TraceLeftFoot.Value - PelvisOffset, DeltaSeconds, 5.0f);

					SetLadderIK(FName("Hand_L"), FName("Palm_L"), Hand_L_CurveNameSet, LeftHandLadderOffset, Hand_L_Y_Distance, DeltaSeconds, 1.0f, true);
					SetLadderIK(FName("Hand_R"), FName("Palm_R"), Hand_R_CurveNameSet, RightHandLadderOffset, 0.0f, DeltaSeconds, 1.0f);
					SetLadderIK(FName("Foot_R"), FName("ball_r"), Foot_R_CurveNameSet, RightFootLadderOffset, 0.0f, DeltaSeconds, 0.5f);
					break;
				}
				case ELadderStance::ClimbUp_OneStep:
				{
					CheckIKValid(FName("Disable_FootIK_L"), LeftFootIKTypeAlpha, DeltaSeconds);

					TTuple<FVector, float> TraceLeftFoot = FootTrace("Foot_L");

					TraceLeftFoot.Value *= LeftFootIKAlpha;

					FootRotation(DeltaSeconds, TraceLeftFoot.Key, &LeftFootRotator, 20.0f);

					PelvisOffset = FMath::FInterpTo(PelvisOffset, TraceLeftFoot.Value, DeltaSeconds, 5.0f);
					LeftFootGroundOffset.Z = FMath::FInterpTo(LeftFootGroundOffset.Z, TraceLeftFoot.Value - PelvisOffset, DeltaSeconds, 5.0f);

					SetLadderIK(FName("Hand_L"), FName("Palm_L"), Hand_L_CurveNameSet, LeftHandLadderOffset, Hand_L_Y_Distance, DeltaSeconds);
					SetLadderIK(FName("Hand_R"), FName("Palm_R"), Hand_R_CurveNameSet, RightHandLadderOffset, 0.0f, DeltaSeconds, 1.0f);
					SetLadderIK(FName("Foot_R"), FName("ball_r"), Foot_R_CurveNameSet, RightFootLadderOffset, 0.0f, DeltaSeconds, 0.5f);
					SetLadderIK(FName("Foot_L"), FName("ball_l"), Foot_L_CurveNameSet, LeftFootLadderOffset, 0.0f, DeltaSeconds, 0.5f);
					break;
				}
				default:
				{
					SetLadderIK(FName("Hand_L"), FName("Palm_L"), Hand_L_CurveNameSet, LeftHandLadderOffset, Hand_L_Y_Distance, DeltaSeconds, 1.0f, true);
					SetLadderIK(FName("Hand_R"), FName("Palm_R"), Hand_R_CurveNameSet, RightHandLadderOffset, Hand_R_Y_Distance, DeltaSeconds);
					SetLadderIK(FName("Foot_L"), FName("ball_l"), Foot_L_CurveNameSet, LeftFootLadderOffset, Foot_L_Y_Distance, DeltaSeconds, 0.5f);
					SetLadderIK(FName("Foot_R"), FName("ball_r"), Foot_R_CurveNameSet, RightFootLadderOffset, Foot_R_Y_Distance, DeltaSeconds, 0.5f);
					break;
				}
				}
			}

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

void UCharacterBaseAnimInstance::DebugLadderStance()
{
	switch (CurLadderStance)
	{
	case ELadderStance::ClimbUp_OneStep:
	{
		UE_LOG(LogTemp, Warning, TEXT("ClimbUp_OneStep"));
		break;
	}
	case ELadderStance::ClimbUp:
	{
		UE_LOG(LogTemp, Warning, TEXT("ClimbUp"));
		break;
	}
	case ELadderStance::ClimbDown_OneStep:
	{
		UE_LOG(LogTemp, Warning, TEXT("ClimbDown_OneStep"));
		break;
	}
	case ELadderStance::ClimbDown:
	{
		UE_LOG(LogTemp, Warning, TEXT("ClimbDown"));
		break;
	}
	case ELadderStance::Enter_From_Bottom:
	{
		UE_LOG(LogTemp, Warning, TEXT("Enter_From_Bottom"));
		break;
	}
	case ELadderStance::Enter_From_Top:
	{
		UE_LOG(LogTemp, Warning, TEXT("Enter_From_Top"));
		break;
	}
	case ELadderStance::Exit_From_Top:
	{
		UE_LOG(LogTemp, Warning, TEXT("Exit_From_Top"));
		break;
	}
	case ELadderStance::Exit_From_Bottom:
	{
		UE_LOG(LogTemp, Warning, TEXT("Exit_From_Bottom"));
		break;
	}
	case ELadderStance::Idle:
	{
		UE_LOG(LogTemp, Warning, TEXT("Idle"));
		break;
	}
	case ELadderStance::Idle_OneStep:
	{
		UE_LOG(LogTemp, Warning, TEXT("Idle_OneStep"));
		break;
	}
	default:
		break;
	}
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
	CurLadderStance = Character->GetCurLadderStance();
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
	CurLadderStance = Character->GetCurLadderStance();
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

void UCharacterBaseAnimInstance::AnimNotify_NOT_ResetLadder()
{

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

void UCharacterBaseAnimInstance::AnimNotify_NOT_DodgeEnd()
{
	OnDodgeEnd.Broadcast();
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_DodgeStart()
{
	OnDodgeStart.Broadcast();
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_AttackStart()
{
	OnAttackStart.Broadcast();
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_CanDodge()
{
	OnCanDodge.Broadcast();
}

/*
void UCharacterBaseAnimInstance::AnimNotify_NOT_EquipEnd()
{
	OnEquipEnd.Broadcast();
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_HolsterEnd()
{
	OnHolsterEnd.Broadcast();
}
*/

FName UCharacterBaseAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Light%d"), Section));
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_EnterWalkState()
{
	OnEnterWalkState.Broadcast();
	SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_EnterLadderState()
{
	OnEnterLadderState.Broadcast();
	//SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
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
	CurLadderStance = Character->GetCurLadderStance();
	CanMovementInput = true;
	//UE_LOG(LogTemp, Warning, TEXT("EnterIdleState"));
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_LeftIdleState()
{
	CanMovementInput = false;
	//UE_LOG(LogTemp, Warning, TEXT("LeftIdleState"));
}