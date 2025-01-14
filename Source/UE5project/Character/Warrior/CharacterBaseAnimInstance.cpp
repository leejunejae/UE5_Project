// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBaseAnimInstance.h"
#include "FallenKnight.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

UCharacterBaseAnimInstance::UCharacterBaseAnimInstance()
{
	IsInAir = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DEATH_MONTAGE(TEXT("/Game/Character/C_Warrior/CW_Animation/CWA_BP/CWAB_DeathMontage.CWAB_DeathMontage"));
	if (DEATH_MONTAGE.Succeeded())
	{
		DeathMontage = DEATH_MONTAGE.Object;
	}

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
		CurrentState = Character->GetCurrentState();
		CurStance = Character->GetStance();
		IsMovementInput = Character->GetIsMovementInput();
		IsInAir = Character->GetMovementComponent()->IsFalling();
		IsAttack = Character->IsAttacking();
		IsAttackInput = Character->IsAttackInput();
		IsBlock = Character->IsBlocking();
		IsParry = Character->IsParrying();
		IsRoll = Character->IsRolling();
		Response = Character->GetCharResponse();
		IsRide = Character->IsRiding();
		IsEscape = Character->GetVertical() > 1.0f ? true : false;
		IsRightHand = Character->GetCurHand();
		ComboCount = Character->CheckCombo();

		switch (CurrentState)
		{
		case ECharacterState::Ground:
		{
			CurGroundStance = Character->GetCurGroundStance();

			RightHandLadderAlpha = FMath::FInterpTo(RightHandLadderAlpha, 0.0f, DeltaSeconds, 5.0f);
			LeftHandLadderAlpha = FMath::FInterpTo(LeftHandLadderAlpha, 0.0f, DeltaSeconds, 5.0f);
			RightFootLadderAlpha = FMath::FInterpTo(RightFootLadderAlpha, 0.0f, DeltaSeconds, 5.0f);
			LeftFootLadderAlpha = FMath::FInterpTo(LeftFootLadderAlpha, 0.0f, DeltaSeconds, 5.0f);

			RightHandStateAlpha = FMath::FInterpTo(RightHandStateAlpha, 0.0f, DeltaSeconds, 5.0f);
			LeftHandStateAlpha = FMath::FInterpTo(LeftHandStateAlpha, 0.0f, DeltaSeconds, 5.0f);
			RightFootStateAlpha = FMath::FInterpTo(RightFootStateAlpha, 0.0f, DeltaSeconds, 5.0f);
			LeftFootStateAlpha = FMath::FInterpTo(LeftFootStateAlpha, 0.0f, DeltaSeconds, 5.0f);

			Speed = Character->GetVelocity().Length();//FMath::FInterpTo(Speed, VelocityLength, DeltaSeconds, 20.0f);
			Direction = CalculateDirection(Character->GetVelocity(), Character->GetActorRotation());
			IsAccelerating = Character->GetCharacterMovement()->GetCurrentAcceleration() != FVector::ZeroVector && Speed > 3.0f ? true : false;

			if (GetCurveValue(FName("TurnLock")) > 0.0f || Character->GetLastMovementInputVector().IsNearlyZero())
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

			float BlendAlpha = IsBlock ? 0.99f : 0.01f;
			BlockBlend = FMath::FInterpTo(BlockBlend, BlendAlpha, DeltaSeconds, 10.0f);

			SetPitch();
			SetRootYawOffset();

			float LeftFootOffsetLast;
			float RightFootOffsetLast;
			float PelvisOffsetLast;

			LeftFootOffsetLast = LeftFootOffset;
			RightFootOffsetLast = RightFootOffset;
			PelvisOffsetLast = PelvisOffset;

			TTuple<FVector, float> TraceLeftFoot = FootTrace("Foot_L");
			TTuple<FVector, float> TraceRightFoot = FootTrace("Foot_R");

			//UE_LOG(LogTemp, Warning, TEXT("LeftFootOffset = %f"), TraceLeftFoot.Value);
			//UE_LOG(LogTemp, Warning, TEXT("RightFootOffset = %f"), TraceRightFoot.Value);

			FootRotation(DeltaSeconds, TraceLeftFoot.Key, &LeftFootRotator, 20.0f);
			FootRotation(DeltaSeconds, TraceRightFoot.Key, &RightFootRotator, 20.0f);
			//UE_LOG(LogTemp, Warning, TEXT("LeftFootRotator (Pitch : %f, Yaw : %f, Roll : %f) "), LeftFootRotator.Pitch, LeftFootRotator.Yaw, LeftFootRotator.Roll);
			//UE_LOG(LogTemp, Warning, TEXT("RightFootRotator (Pitch : %f, Yaw : %f, Roll : %f)"), RightFootRotator.Pitch, RightFootRotator.Yaw, RightFootRotator.Roll);

			PelvisOffset = FMath::FInterpTo(PelvisOffsetLast, UKismetMathLibrary::Min(TraceLeftFoot.Value, TraceRightFoot.Value), DeltaSeconds, 15.0f);
			LeftFootOffset = FMath::FInterpTo(LeftFootOffsetLast, TraceLeftFoot.Value - PelvisOffset, DeltaSeconds, 15.0f);
			RightFootOffset = FMath::FInterpTo(RightFootOffsetLast, -1.0f * (TraceRightFoot.Value - PelvisOffset), DeltaSeconds, 15.0f);
			break;
		}
		case ECharacterState::Ladder:
		{
			CheckIKValid(FName("Disable_FootIK_L"), LeftFootStateAlpha, DeltaSeconds);
			CheckIKValid(FName("Disable_FootIK_R"), RightFootStateAlpha, DeltaSeconds);
			CheckIKValid(FName("Disable_HandIK_L"), LeftHandStateAlpha, DeltaSeconds);
			CheckIKValid(FName("Disable_HandIK_R"), RightHandStateAlpha, DeltaSeconds);

			const TOptional<FVector> Grip_Hand_R = Character->GetBoneTargetLoc(EBodyType::Hand_R);
			const TOptional<FVector> Grip_Hand_L = Character->GetBoneTargetLoc(EBodyType::Hand_L);
			const TOptional<FVector> Grip_Foot_R = Character->GetBoneTargetLoc(EBodyType::Foot_R);
			const TOptional<FVector> Grip_Foot_L = Character->GetBoneTargetLoc(EBodyType::Foot_L);
			
			CurLadderStance = Character->GetCurLadderStance();

			if (GetCurveValue(LockIK) > 0.0f && bIsClimb)
			{
				const float Pelvis_CurveValue = GetCurveValue(FName("root_translation_Z"));
					//GetCurveValue(Pelvis_Curve);
				const float Hand_L_CurveValue = GetCurveValue(Hand_L_Curve);
				const float Hand_R_CurveValue = GetCurveValue(Hand_R_Curve);
				const float Foot_L_CurveValue = GetCurveValue(Foot_L_Curve);
				const float Foot_R_CurveValue = GetCurveValue(Foot_R_Curve);

				if (Pelvis_CurveValue > 0.0f)// && Pelvis_CurveValue < 1.0f)
				{
					float Prev_Curve_Value = CurveValue_Pelvis.Z;
					CurveValue_Pelvis.Z = Pelvis_CurveValue;
					float CurveDifference = CurveValue_Pelvis.Z - Prev_Curve_Value;
					FVector NewCharLocation = Character->GetActorLocation();
					NewCharLocation.Z += Character->GetClimbDistance() * CurveDifference;
						//FMath::Lerp(0.0f, Character->GetClimbDistance(), CurveDifference);
					Character->SetActorLocation(NewCharLocation);
					
					/*
					UE_LOG(LogTemp, Warning, TEXT("Prev_Curve_Value = %f"), Prev_Curve_Value);
					UE_LOG(LogTemp, Warning, TEXT("CurveValue_Pelvis = %f"), CurveValue_Pelvis);
					UE_LOG(LogTemp, Warning, TEXT("CurveDifference = %f"), CurveDifference);
					UE_LOG(LogTemp, Warning, TEXT("Func CharacterLocation = X : %f, Y : %f, Z : %f"), Character->GetActorLocation().X, Character->GetActorLocation().Y, Character->GetActorLocation().Z);
					UE_LOG(LogTemp, Warning, TEXT("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ"));
					*/
				}

				SetLadderIK(FName("Hand_L"), FName("Palm_L"), Hand_L_CurveValue, LeftHandTarget, LeftHandLadderAlpha, DeltaSeconds);
				SetLadderIK(FName("Hand_R"), FName("Palm_R"), Hand_R_CurveValue, RightHandTarget, RightHandLadderAlpha, DeltaSeconds);
				SetLadderIK(FName("Foot_L"), FName("ball_l"), Foot_L_CurveValue, LeftFootTarget, LeftFootLadderAlpha, DeltaSeconds, 0.5f);
				SetLadderIK(FName("Foot_R"), FName("ball_r"), Foot_R_CurveValue, RightFootTarget, RightFootLadderAlpha, DeltaSeconds, 0.5f);

				/*
				if(Hand_L_CurveValue > 0.0f)
				{
					const TOptional<FVector> Hand_L_Target = SetBodyLocationOnLadder(FName("Hand_L"), FName("Palm_L"), Hand_L_CurveValue, LeftHandTarget, DeltaSeconds);
					LeftHandTarget = Hand_L_Target.IsSet() ? Hand_L_Target.GetValue() : LeftHandTarget;
					LeftHandLadderAlpha = Hand_L_CurveValue < 0.5f ? FMath::Lerp(1.0f, 0.0f, Hand_L_CurveValue * 2.0f) : FMath::Lerp(0.0f, 1.0f, Hand_L_CurveValue * 2.0f - 1);
				}
				else
				{
					LeftHandTarget = Character->GetMesh()->GetSocketLocation(FName("Hand_L"));
					LeftHandLadderAlpha = FMath::FInterpTo(LeftHandLadderAlpha, 1.0f, DeltaSeconds, 3.0f);
				}

				if (Hand_R_CurveValue > 0.0f)
				{
					const TOptional<FVector> Hand_R_Target = SetBodyLocationOnLadder(FName("Hand_R"), FName("Palm_R"), Hand_R_CurveValue, RightHandTarget, DeltaSeconds);
					RightHandTarget = Hand_R_Target.IsSet() ? Hand_R_Target.GetValue() : RightHandTarget;
					RightHandLadderAlpha = Hand_R_CurveValue < 0.5f ? FMath::Lerp(1.0f, 0.0f, Hand_R_CurveValue * 2.0f) : FMath::Lerp(0.0f, 1.0f, Hand_R_CurveValue * 2.0f - 1);
				}
				else
				{
					RightHandTarget = Character->GetMesh()->GetSocketLocation(FName("Hand_R"));
					RightHandLadderAlpha = FMath::FInterpTo(RightHandLadderAlpha, 1.0f, DeltaSeconds, 3.0f);
				}

				if (Foot_L_CurveValue > 0.0f)
				{
					const TOptional<FVector> Foot_L_Target = SetBodyLocationOnLadder(FName("Foot_L"), FName("ball_l"), Foot_L_CurveValue, LeftFootTarget, DeltaSeconds, 0.5f);
					LeftFootTarget = Foot_L_Target.IsSet() ? Foot_L_Target.GetValue() : LeftFootTarget;
					LeftFootLadderAlpha = Foot_L_CurveValue < 0.5f ? FMath::Lerp(1.0f, 0.0f, Foot_L_CurveValue * 2.0f) : FMath::Lerp(0.0f, 1.0f, (Foot_L_CurveValue - 0.5f) * 2.0f);
				}
				else
				{
					LeftFootTarget = Character->GetMesh()->GetSocketLocation(FName("Foot_L"));
					LeftFootLadderAlpha = FMath::FInterpTo(LeftFootLadderAlpha, 1.0f, DeltaSeconds, 3.0f);
				}

				if (Foot_R_CurveValue > 0.0f)
				{
					const TOptional<FVector> Foot_R_Target = SetBodyLocationOnLadder(FName("Foot_R"), FName("ball_r"), Foot_R_CurveValue, RightFootTarget, DeltaSeconds, 0.5f);
					RightFootTarget = Foot_R_Target.IsSet() ? Foot_R_Target.GetValue() : RightFootTarget;
					RightFootLadderAlpha = Foot_R_CurveValue < 0.5f ? FMath::Lerp(1.0f, 0.0f, Foot_R_CurveValue * 2.0f) : FMath::Lerp(0.0f, 1.0f, Foot_R_CurveValue * 2.0f - 1);
				}
				else
				{
					RightFootTarget = Character->GetMesh()->GetSocketLocation(FName("Foot_R"));
					RightFootLadderAlpha = FMath::FInterpTo(RightFootLadderAlpha, 1.0f, DeltaSeconds, 3.0f);
				}
				/*
				UE_LOG(LogTemp, Warning, TEXT("LeftHandLadderAlpha = %f"), LeftHandLadderAlpha);
				UE_LOG(LogTemp, Warning, TEXT("RightHandLadderAlpha = %f"), RightHandLadderAlpha);
				UE_LOG(LogTemp, Warning, TEXT("LeftFootLadderAlpha = %f"), LeftFootLadderAlpha);
				UE_LOG(LogTemp, Warning, TEXT("RightFootLadderAlpha = %f"), RightFootLadderAlpha);
				UE_LOG(LogTemp, Warning, TEXT("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ"));
				UE_LOG(LogTemp, Warning, TEXT("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ"));
				*/
			}
			else
			{
			}

			//DebugLadderStance();
			//UE_LOG(LogTemp, Warning, TEXT("%s"), IsOneStep ? TEXT("True") : TEXT("False"));
			break;
		}
		case ECharacterState::Riding:
		{
			MountRight = Character->GetMountDir();
			Speed = Character->GetVertical();
			Direction = Character->GetHorizontal();
			break;
		}
		default:
		{
			if (!Character->GetClimbComponent())
				break;

			if (CurLadderStance == ELadderStance::Enter_From_Bottom)
			{
				FTransform InitClimbPosition;
				InitClimbPosition = Character->GetClimbComponent()->GetInitTopPosition().GetValue();
			}
			else
			{

			}
			
				//UE_LOG(LogTemp, Warning, TEXT("Default Section"));
			CheckIKValid(FName("Disable_FootIK_L"), LeftFootStateAlpha, DeltaSeconds);
			CheckIKValid(FName("Disable_FootIK_R"), RightFootStateAlpha, DeltaSeconds);
			CheckIKValid(FName("Disable_HandIK_L"), LeftHandStateAlpha, DeltaSeconds);
			CheckIKValid(FName("Disable_HandIK_R"), RightHandStateAlpha, DeltaSeconds);

			const TOptional<FVector> Grip_Hand_R = Character->GetBoneTargetLoc(EBodyType::Hand_R);
			const TOptional<FVector> Grip_Hand_L = Character->GetBoneTargetLoc(EBodyType::Hand_L);
			const TOptional<FVector> Grip_Foot_R = Character->GetBoneTargetLoc(EBodyType::Foot_R);
			const TOptional<FVector> Grip_Foot_L = Character->GetBoneTargetLoc(EBodyType::Foot_L);

			CurLadderStance = Character->GetCurLadderStance();

			if (GetCurveValue(LockIK) > 0.0f && bIsClimb)
			{
				const FVector Pelvis_CurveValue = FVector(0.0f, GetCurveValue(FName("root_translation_Y")), GetCurveValue(FName("root_translation_Z")));
				const FRotator Pelvis_CurveValue_Rotator = FRotator(0.0f, GetCurveValue(FName("root_rotation_Z")), 0.0f);
					//GetCurveValue(Pelvis_Curve);
				const float Hand_L_CurveValue = GetCurveValue(Hand_L_Curve);
				const float Hand_R_CurveValue = GetCurveValue(Hand_R_Curve);
				const float Foot_L_CurveValue = GetCurveValue(Foot_L_Curve);
				const float Foot_R_CurveValue = GetCurveValue(Foot_R_Curve);

				if (Pelvis_CurveValue.Z > 0.0f && Pelvis_CurveValue.Z <= 1.0f)
				{
					FVector Prev_CurveValue_Pelvis = CurveValue_Pelvis;
					CurveValue_Pelvis = Pelvis_CurveValue;
					FVector CurveValue_Pelvis_Difference = CurveValue_Pelvis - Prev_CurveValue_Pelvis;
					FVector NewCharLocation = Character->GetActorLocation() + (Character->GetActorForwardVector() * CurveValue_Pelvis_Difference.Y);
					NewCharLocation.Z += CurveValue_Pelvis_Difference.Z * Character->GetClimbDistance();
					
					FRotator Prev_CurveValue_Pelvis_Rotator = CurveValue_Pelvis_Rotator;
					CurveValue_Pelvis_Rotator = Pelvis_CurveValue_Rotator;
					FRotator CurveValue_Pelvis_Rotator_Difference = CurveValue_Pelvis_Rotator - Prev_CurveValue_Pelvis_Rotator;
					FRotator NewCharRotation = Character->GetActorRotation() + CurveValue_Pelvis_Rotator_Difference;

						//FMath::Lerp(0.0f, Character->GetClimbDistance(), CurveDifference);
					Character->SetActorLocationAndRotation(NewCharLocation, NewCharRotation);
					//Character->SetActorLocation(NewCharLocation);
					//AdjustCurveSum += CurveDifference * (Character->GetClimbDistance() / 130.0f);

					//UE_LOG(LogTemp, Warning, TEXT("Distance Difference = %f"), Character->GetClimbDistance() / 25.0f);
					//UE_LOG(LogTemp, Warning, TEXT("Pelvis Z Offset = %f"), CurveDifference * (Character->GetClimbDistance() / 25.0f));
					//UE_LOG(LogTemp, Warning, TEXT("Adjust Curve Sum = %f"), AdjustCurveSum);
					//UE_LOG(LogTemp, Warning, TEXT("Curve Sum = %f"), PelvisCurveSum);
					//UE_LOG(LogTemp, Warning, TEXT("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ"));

					/*
					UE_LOG(LogTemp, Warning, TEXT("Prev_Curve_Value = %f"), Prev_Curve_Value);
					UE_LOG(LogTemp, Warning, TEXT("CurveValue_Pelvis = %f"), CurveValue_Pelvis);
					UE_LOG(LogTemp, Warning, TEXT("CurveDifference = %f"), CurveDifference);
					UE_LOG(LogTemp, Warning, TEXT("Func CharacterLocation = X : %f, Y : %f, Z : %f"), Character->GetActorLocation().X, Character->GetActorLocation().Y, Character->GetActorLocation().Z);
					UE_LOG(LogTemp, Warning, TEXT("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ"));
					*/
				}
				// middle_01_l


				/*
				if (Hand_L_CurveValue > 0.0f)
				{
					const TOptional<FVector> Hand_L_Target = SetBodyLocationOnLadder(FName("Hand_L"), FName("Palm_L"), Hand_L_CurveValue, LeftHandTarget, DeltaSeconds);
					LeftHandTarget = Hand_L_Target.IsSet() ? Hand_L_Target.GetValue() : LeftHandTarget;
					LeftHandLadderAlpha = Hand_L_CurveValue < 0.5f ? FMath::Lerp(1.0f, 0.0f, Hand_L_CurveValue * 2.0f) : FMath::Lerp(0.0f, 1.0f, Hand_L_CurveValue * 2.0f - 1);
				}
				else
				{
					LeftHandTarget = Character->GetMesh()->GetSocketLocation(FName("Hand_L"));
					LeftHandLadderAlpha = FMath::FInterpTo(LeftHandLadderAlpha, 1.0f, DeltaSeconds, 3.0f);
				}

				if (Hand_R_CurveValue > 0.0f)
				{
					FVector Hand_R_Loc = Character->GetMesh()->GetSocketLocation(FName("Hand_R"));
					const TOptional<FVector> Hand_R_Target = SetBodyLocationOnLadder(FName("Hand_R"), FName("Palm_R"), Hand_R_CurveValue, RightHandTarget, DeltaSeconds);
					RightHandTarget = Hand_R_Target.IsSet() ? Hand_R_Target.GetValue() : RightHandTarget;
					RightHandLadderAlpha = Hand_R_CurveValue < 0.5f ? FMath::Lerp(1.0f, 0.0f, Hand_R_CurveValue * 2.0f) : FMath::Lerp(0.0f, 1.0f, Hand_R_CurveValue * 2.0f - 1);
				}
				else
				{
					RightHandTarget = Character->GetMesh()->GetSocketLocation(FName("Hand_R"));
					RightHandLadderAlpha = FMath::FInterpTo(RightHandLadderAlpha, 1.0f, DeltaSeconds, 3.0f);
				}

				if (Foot_L_CurveValue > 0.0f)
				{
					const TOptional<FVector> Foot_L_Target = SetBodyLocationOnLadder(FName("Foot_L"), FName("ball_l"), Foot_L_CurveValue, LeftFootTarget, DeltaSeconds, 0.5f);
					LeftFootTarget = Foot_L_Target.IsSet() ? Foot_L_Target.GetValue() : LeftFootTarget;
					LeftFootLadderAlpha = Foot_L_CurveValue < 0.5f ? FMath::Lerp(1.0f, 0.0f, Foot_L_CurveValue * 2.0f) : FMath::Lerp(0.0f, 1.0f, (Foot_L_CurveValue - 0.5f) * 2.0f);
				}
				else
				{
					LeftFootTarget = Character->GetMesh()->GetSocketLocation(FName("Foot_L"));
					LeftFootLadderAlpha = FMath::FInterpTo(LeftFootLadderAlpha, 1.0f, DeltaSeconds, 3.0f);
				}

				if (Foot_R_CurveValue > 0.0f)
				{
					const TOptional<FVector> Foot_R_Target = SetBodyLocationOnLadder(FName("Foot_R"), FName("ball_r"), Foot_R_CurveValue, RightFootTarget, DeltaSeconds, 0.5f);
					RightFootTarget = Foot_R_Target.IsSet() ? Foot_R_Target.GetValue() : RightFootTarget;
					RightFootLadderAlpha = Foot_R_CurveValue < 0.5f ? FMath::Lerp(1.0f, 0.0f, Foot_R_CurveValue * 2.0f) : FMath::Lerp(0.0f, 1.0f, Foot_R_CurveValue * 2.0f - 1);
				}
				else
				{
					RightFootTarget = Character->GetMesh()->GetSocketLocation(FName("Foot_R"));
					RightFootLadderAlpha = FMath::FInterpTo(RightFootLadderAlpha, 1.0f, DeltaSeconds, 3.0f);
				}
				/*
				UE_LOG(LogTemp, Warning, TEXT("LeftHandLadderAlpha = %f"), LeftHandLadderAlpha);
				UE_LOG(LogTemp, Warning, TEXT("RightHandLadderAlpha = %f"), RightHandLadderAlpha);
				UE_LOG(LogTemp, Warning, TEXT("LeftFootLadderAlpha = %f"), LeftFootLadderAlpha);
				UE_LOG(LogTemp, Warning, TEXT("RightFootLadderAlpha = %f"), RightFootLadderAlpha);
				UE_LOG(LogTemp, Warning, TEXT("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ"));
				UE_LOG(LogTemp, Warning, TEXT("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ"));
				*/
			}
			else
			{
			}
			//DebugLadderStance();

			//UE_LOG(LogTemp, Warning, TEXT("%s"), IsOneStep ? TEXT("True") : TEXT("False"));
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
	enum ECollisionChannel TraceChannel = Speed > 100.0f ? ECC_Visibility : ECC_GameTraceChannel8;

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
		float Offset = ((DistHitResult.ImpactPoint - DistHitResult.TraceEnd).Length() + 5.0f) - TraceDistance;
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

void UCharacterBaseAnimInstance::FootPlacement(float DeltaTime, float TargetValue, float* FootValue, float InterpSpeed)
{
	*FootValue = UKismetMathLibrary::FInterpTo(*FootValue, TargetValue, DeltaTime, InterpSpeed);
}


///////// Ladder Placement /////////////
 

void UCharacterBaseAnimInstance::SetLadderIK(const FName& BoneName, const FName& MiddleBoneName, float CurveValue, FVector& BoneTarget, float& LimbLadderAlpha, float DeltaSeconds, float Offset)
{
	if (CurveValue > 0.0f)
	{
		const TOptional<FVector> LimbTarget = SetBodyLocationOnLadder(BoneName, MiddleBoneName, CurveValue, BoneTarget, DeltaSeconds, Offset);
		BoneTarget = LimbTarget.IsSet() ? LimbTarget.GetValue() : BoneTarget;
		LimbLadderAlpha = CurveValue < 0.5f ? FMath::Lerp(1.0f, 0.0f, CurveValue * 2.0f) : FMath::Lerp(0.0f, 1.0f, CurveValue * 2.0f - 1);
	}
	else
	{
		BoneTarget = Character->GetMesh()->GetSocketLocation(BoneName);
		LimbLadderAlpha = FMath::FInterpTo(LimbLadderAlpha, 1.0f, DeltaSeconds, 3.0f);
	}
}

void UCharacterBaseAnimInstance::CheckIKValid(FName CurveName, float& AlphaValue, float DeltaSeconds)
{
	if (GetCurveValue(CurveName) > 0.0f)
	{
		AlphaValue = FMath::FInterpTo(AlphaValue, 0.0f, DeltaSeconds, 5.0f);
	}
	else
	{
		AlphaValue = FMath::FInterpTo(AlphaValue, 1.0f, DeltaSeconds, 5.0f);
	}
}

TOptional<FVector> UCharacterBaseAnimInstance::SetBodyLocationOnLadder(FName BoneName, FName MiddleBoneName, float CurveValue, FVector PrevTargetLoc, float DeltaSeconds, float AdjCoefft)
{
	if (!BoneNameToBodyType.Contains(BoneName))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Bone Type Doesnt exist in Map"));
		return TOptional<FVector>();
	}

	const TOptional<FVector> GripLoc = Character->GetBoneTargetLoc(BoneNameToBodyType[BoneName]);

	if (!Character->GetMesh()->DoesSocketExist(BoneName) || !GripLoc.IsSet())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Bone Doesnt exist"));
		return TOptional<FVector>();
	}

	FVector BoneLoc = Character->GetMesh()->GetSocketLocation(BoneName);

	FVector AdjustBoneLoc = FVector::ZeroVector;

	if (Character->GetMesh()->DoesSocketExist(MiddleBoneName))
	{
		FVector MiddleBoneLoc = Character->GetMesh()->GetSocketLocation(MiddleBoneName);
		AdjustBoneLoc = (MiddleBoneLoc - BoneLoc) * AdjCoefft;
	}

	FVector DifferenceGripAndBone = GripLoc.GetValue() - (BoneLoc + AdjustBoneLoc);
	FVector TargetLoc = FMath::Lerp(FVector::ZeroVector, DifferenceGripAndBone, CurveValue);

	return BoneLoc + TargetLoc;
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

void UCharacterBaseAnimInstance::PlayMontage(MontageType Type, int32 Section)
{
	switch (Type)
	{
	case MontageType::Death:
		Montage_Play(DeathMontage, 1.0f);
		break;
	default:
		break;
	}
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
	CurveValue_Pelvis.Z = 0.0f;
	CurveValue_Pelvis.Y = 0.0f;
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
	OnEnterLocomotion.Broadcast();
}

void UCharacterBaseAnimInstance::AnimNotify_NOT_LeftLocomotion()
{
	//UE_LOG(LogTemp, Warning, TEXT("ResetHurt"));
	OnLeftLocomotion.Broadcast();
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
	SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
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

MontageType UCharacterBaseAnimInstance::CheckMontage(UAnimMontage* Montage)
{
	if (Montage == DeathMontage)
		return MontageType::Death;
	else
		return MontageType::None;
}