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

			//RightHandLadderAlpha = FMath::FInterpTo(RightHandLadderAlpha, 0.0f, DeltaSeconds, 5.0f);
			//LeftHandLadderAlpha = FMath::FInterpTo(LeftHandLadderAlpha, 0.0f, DeltaSeconds, 5.0f);
			//RightFootLadderAlpha = FMath::FInterpTo(RightFootLadderAlpha, 0.0f, DeltaSeconds, 5.0f);
			//LeftFootLadderAlpha = FMath::FInterpTo(LeftFootLadderAlpha, 0.0f, DeltaSeconds, 5.0f);

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

			//SetPitch();
			//SetRootYawOffset();

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
			CurLadderStance = Character->GetCurLadderStance();

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
					CheckIKValid(FName("Disable_FootIK_L"), LeftFootStateAlpha, DeltaSeconds);
					CheckIKValid(FName("Disable_FootIK_R"), RightFootStateAlpha, DeltaSeconds);
					CheckIKValid(FName("Disable_HandIK_L"), LeftHandStateAlpha, DeltaSeconds);
					CheckIKValid(FName("Disable_HandIK_R"), RightHandStateAlpha, DeltaSeconds);

					const FVector InitLocation = Character->GetClimbComponent()->GetInitTopPosition().GetValue().GetLocation() + FVector(0.0f, 0.0f, Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
					const FVector TargetLocation = Character->GetClimbComponent()->GetEnterTopPosition().GetValue().GetLocation();
					const float Translation_CurveValue_Y = GetCurveValue(FName("Char_Translation_Y"));
					const float Translation_Value_Y = Character->GetClimbComponent()->GetLadderTopTransitionDistance();
					const float Rotation_CurveValue = GetCurveValue(FName("Char_Rotation_Z"));

					FVector NewCharLocation = InitLocation;

					const FVector EnterDirection = FVector(TargetLocation.X - InitLocation.X, TargetLocation.Y - InitLocation.Y, 0.0f);
					//UE_LOG(LogTemp, Warning, TEXT("Curve_Hand_R_Z = %f"), GetCurveValue(Hand_R_Translation_Z));
					//InitClimbPosition = Character->GetClimbComponent()->GetLadderTopTransitionDistance();

					if (Translation_CurveValue_Y > 0.0f)
					{
						NewCharLocation = FMath::Lerp(InitLocation, TargetLocation, Translation_CurveValue_Y);
						/*
						float Prev_CurveValue_Root_Y = CurveValue_Root_Y;
						CurveValue_Root_Y = Translation_CurveValue_Y;
						float CurveValue_Root_Y_Difference = CurveValue_Root_Y - Prev_CurveValue_Root_Y;
						NewCharLocation += (EnterDirection * CurveValue_Root_Y);
						//FMath::Lerp(0.0f, Character->GetClimbDistance(), CurveDifference);
						Character->SetActorLocation(NewCharLocation);
						//UE_LOG(LogTemp, Warning, TEXT("Y Movement Offset = %f"), EnterDirection * Translation_Value_Y* CurveValue_Root_Y_Difference);
						*/
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

						//UE_LOG(LogTemp, Warning, TEXT("InitRotator Pitch = %f, Yaw = %f, Roll = %f"), InitRotator.Pitch, InitRotator.Yaw, InitRotator.Roll);
						//UE_LOG(LogTemp, Warning, TEXT("TargetRotator Pitch = %f, Yaw = %f, Roll = %f"), TargetRotator.Pitch, TargetRotator.Yaw, TargetRotator.Roll);
						//UE_LOG(LogTemp, Warning, TEXT("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ"));
						const float NewRotatorYaw = FMath::Lerp(InitYaw, TargetYaw, Rotation_CurveValue);
						const FRotator NewRotator = FRotator(Character->GetActorRotation().Pitch, NewRotatorYaw, Character->GetActorRotation().Roll);
						Character->SetActorRotation(NewRotator);
						//float Prev_Rotation_CurveValue
					}

					
					//SetLadderIK(FName("Hand_L"), FName("Palm_L"), Hand_L_CurveNameSet, LeftHandTarget, 0.0f, DeltaSeconds);
					//
					//

					SetLadderIK(FName("Hand_L"), FName("Palm_L"), Hand_L_CurveNameSet, LeftHandTarget, 0.0f, DeltaSeconds);

					if (GetCurveValue(Hand_R_CurveNameSet.CurveNameZ) > -1.0f)
					{
						SetLadderIK(FName("Hand_R"), FName("Palm_R"), Hand_R_CurveNameSet, RightHandTarget, Hand_R_Y_Distance, DeltaSeconds, 1.0f);
					}

					if (GetCurveValue(Foot_L_CurveNameSet.CurveNameZ) > -1.0f)
					{
						//UE_LOG(LogTemp, Warning, TEXT("Foot_L_CurveNameSet.CurveNameY = %f"), GetCurveValue(Foot_L_CurveNameSet.CurveNameY));
						//UE_LOG(LogTemp, Warning, TEXT("Foot_L_CurveNameSet.CurveNameZ = %f"), GetCurveValue(Foot_L_CurveNameSet.CurveNameZ));
						SetLadderIK(FName("Foot_L"), FName("ball_l"), Foot_L_CurveNameSet, LeftFootTarget, Foot_L_Y_Distance, DeltaSeconds, 0.5f);
					}
					//else
					//{
					//	LeftFootTarget = Character->GetMesh()->GetSocketLocation("Foot_L");
					//}

					if (GetCurveValue(Foot_R_CurveNameSet.CurveNameZ) > -1.0f)
					{
						SetLadderIK(FName("Foot_R"), FName("ball_r"), Foot_R_CurveNameSet, RightFootTarget, Foot_R_Y_Distance, DeltaSeconds, 0.5f);
					}

					break;
				}
				case ELadderStance::Exit_From_Top:
				{
					UE_LOG(LogTemp, Warning, TEXT("Exit Top"));
					CheckIKValid(FName("Disable_FootIK_L"), LeftFootStateAlpha, DeltaSeconds);
					CheckIKValid(FName("Disable_FootIK_R"), RightFootStateAlpha, DeltaSeconds);
					CheckIKValid(FName("Disable_HandIK_L"), LeftHandStateAlpha, DeltaSeconds);
					CheckIKValid(FName("Disable_HandIK_R"), RightHandStateAlpha, DeltaSeconds);

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
						//FMath::Lerp(0.0f, Character->GetClimbDistance(), CurveDifference);
						Character->SetActorLocation(NewCharLocation);
					}

					if (GetCurveValue(Hand_R_CurveNameSet.CurveNameZ) > 1.0f)
					{
						SetLadderIK(FName("Hand_L"), FName("Palm_L"), Hand_L_CurveNameSet, LeftHandTarget, Hand_L_Y_Distance, DeltaSeconds);
					}
					SetLadderIK(FName("Hand_R"), FName("Palm_R"), Hand_R_CurveNameSet, RightHandTarget, Hand_R_Y_Distance, DeltaSeconds, 1.0f);
					SetLadderIK(FName("Foot_L"), FName("ball_l"), Foot_L_CurveNameSet, LeftFootTarget, Foot_L_Y_Distance, DeltaSeconds, 0.5f, true);
					SetLadderIK(FName("Foot_R"), FName("ball_r"), Foot_R_CurveNameSet, RightFootTarget, Foot_R_Y_Distance, DeltaSeconds, 0.5f);
					break;
				}
				case ELadderStance::Enter_From_Bottom:
				{
					CheckIKValid(FName("Disable_FootIK_R"), RightFootStateAlpha, DeltaSeconds);
					CheckIKValid(FName("Disable_HandIK_L"), LeftHandStateAlpha, DeltaSeconds);
					CheckIKValid(FName("Disable_HandIK_R"), RightHandStateAlpha, DeltaSeconds);

					LeftFootStateAlpha = 0.0f;
					float LeftFootOffsetLast;
					LeftFootOffsetLast = LeftFootOffset;
					TTuple<FVector, float> TraceLeftFoot = FootTrace("Foot_L");
					FootRotation(DeltaSeconds, TraceLeftFoot.Key, &LeftFootRotator, 20.0f);
					LeftFootOffset = FMath::FInterpTo(LeftFootOffsetLast, TraceLeftFoot.Value - PelvisOffset, DeltaSeconds, 15.0f);

					SetLadderIK(FName("Hand_L"), FName("Palm_L"), Hand_L_CurveNameSet, LeftHandTarget, 0.0f, DeltaSeconds);
					SetLadderIK(FName("Hand_R"), FName("Palm_R"), Hand_R_CurveNameSet, RightHandTarget, 0.0f, DeltaSeconds, 1.0f);
					SetLadderIK(FName("Foot_R"), FName("ball_r"), Foot_R_CurveNameSet, RightFootTarget, 0.0f, DeltaSeconds, 0.5f);
					break;
				}
				case ELadderStance::Exit_From_Bottom:
				{
					CheckIKValid(FName("Disable_FootIK_R"), RightFootStateAlpha, DeltaSeconds);
					CheckIKValid(FName("Disable_HandIK_L"), LeftHandStateAlpha, DeltaSeconds);
					CheckIKValid(FName("Disable_HandIK_R"), RightHandStateAlpha, DeltaSeconds);

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
					UE_LOG(LogTemp, Warning, TEXT("Exit Bottom"));
					//SetLadderIK(FName("Hand_L"), FName("Palm_L"), Hand_L_CurveNameSet, LeftHandTarget, 0.0f, DeltaSeconds);
					//SetLadderIK(FName("Hand_R"), FName("Palm_R"), Hand_R_CurveNameSet, RightHandTarget, 0.0f, DeltaSeconds, 1.0f);
					//SetLadderIK(FName("Foot_R"), FName("ball_r"), Foot_R_CurveNameSet, RightFootTarget, 0.0f, DeltaSeconds, 0.5f);
					break;
				}
				case ELadderStance::ClimbDown_OneStep:
				{
					CheckIKValid(FName("Disable_FootIK_L"), LeftFootStateAlpha, DeltaSeconds);

					float LeftFootOffsetLast;
					LeftFootOffsetLast = LeftFootOffset;
					TTuple<FVector, float> TraceLeftFoot = FootTrace("Foot_L");
					FootRotation(DeltaSeconds, TraceLeftFoot.Key, &LeftFootRotator, 20.0f);
					LeftFootOffset = FMath::FInterpTo(LeftFootOffsetLast, TraceLeftFoot.Value - PelvisOffset, DeltaSeconds, 15.0f);

					SetLadderIK(FName("Hand_L"), FName("Palm_L"), Hand_L_CurveNameSet, LeftHandTarget, Hand_L_Y_Distance, DeltaSeconds, 1.0f, true);
					SetLadderIK(FName("Hand_R"), FName("Palm_R"), Hand_R_CurveNameSet, RightHandTarget, 0.0f, DeltaSeconds, 1.0f);
					SetLadderIK(FName("Foot_R"), FName("ball_r"), Foot_R_CurveNameSet, RightFootTarget, 0.0f, DeltaSeconds, 0.5f);
					break;
				}
				case ELadderStance::ClimbUp_OneStep:
				{
					CheckIKValid(FName("Disable_FootIK_L"), LeftFootStateAlpha, DeltaSeconds);

					float LeftFootOffsetLast;
					LeftFootOffsetLast = LeftFootOffset;
					TTuple<FVector, float> TraceLeftFoot = FootTrace("Foot_L");
					FootRotation(DeltaSeconds, TraceLeftFoot.Key, &LeftFootRotator, 20.0f);
					LeftFootOffset = FMath::FInterpTo(LeftFootOffsetLast, TraceLeftFoot.Value - PelvisOffset, DeltaSeconds, 15.0f);

					SetLadderIK(FName("Hand_L"), FName("Palm_L"), Hand_L_CurveNameSet, LeftHandTarget, Hand_L_Y_Distance, DeltaSeconds);
					SetLadderIK(FName("Hand_R"), FName("Palm_R"), Hand_R_CurveNameSet, RightHandTarget, 0.0f, DeltaSeconds, 1.0f);
					SetLadderIK(FName("Foot_R"), FName("ball_r"), Foot_R_CurveNameSet, RightFootTarget, 0.0f, DeltaSeconds, 0.5f);
					SetLadderIK(FName("Foot_L"), FName("ball_l"), Foot_L_CurveNameSet, LeftFootTarget, 0.0f, DeltaSeconds, 0.5f);
					break;
				}
				default:
				{
					SetLadderIK(FName("Hand_L"), FName("Palm_L"), Hand_L_CurveNameSet, LeftHandTarget, Hand_L_Y_Distance, DeltaSeconds, 1.0f);
					SetLadderIK(FName("Hand_R"), FName("Palm_R"), Hand_R_CurveNameSet, RightHandTarget, Hand_R_Y_Distance, DeltaSeconds);
					SetLadderIK(FName("Foot_L"), FName("ball_l"), Foot_L_CurveNameSet, LeftFootTarget, Foot_L_Y_Distance, DeltaSeconds, 0.5f);
					SetLadderIK(FName("Foot_R"), FName("ball_r"), Foot_R_CurveNameSet, RightFootTarget, Foot_R_Y_Distance, DeltaSeconds, 0.5f);
					break;
				}
				}
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


///////// Ladder Placement /////////////
 

void UCharacterBaseAnimInstance::SetLadderIK(const FName& BoneName, const FName& MiddleBoneName, FVectorCurveNameSet CurveNameList, FVector& BoneTarget, float LimbYDistance, float DeltaSeconds, float Offset, bool IsDebug)
{
	if (!BoneNameToBodyType.Contains(BoneName) || !Character->GetMesh()->DoesSocketExist(BoneName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Bone Type or Bone is not valid"));
		return;
	}

	const TOptional<TTuple<FVector, FVector>> GripLoc = Character->GetBoneTargetLoc(BoneNameToBodyType[BoneName]);

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

TOptional<FVector> UCharacterBaseAnimInstance::SetIKTargetLocation(FName BoneName, FName MiddleBoneName, float CurveValue, float DeltaSeconds, float AdjCoefft)
{
	if (!BoneNameToBodyType.Contains(BoneName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Bone Type Doesnt exist in Map"));
		return TOptional<FVector>();
	}

	return TOptional<FVector>();
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