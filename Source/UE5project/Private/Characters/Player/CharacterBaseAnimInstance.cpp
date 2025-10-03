// Fill out your copyright notice in the Description page of Project Settings.

// 엔진 헤더
#include "Characters/Player/CharacterBaseAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Core/Subsystems/PlayerAnimRegistrySubsystem.h"

#include "Characters/Components/BaseCharacterMovementComponent.h"
#include "Characters/Player/Components/PlayerStatusComponent.h"
#include "Characters/Components/EquipmentComponent.h"

// 애니메이션 모드
#include "Animation/AnimMode_Ground.h"
#include "Animation/AnimMode_Ladder.h"

// 참조할 액터
#include "Characters/Player/PlayerBase.h" // CharacterBase로 변경예정

// Kismet 유틸리티
#include "KismetAnimationLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// 컴포넌트
#include "Interaction/Climb/Components/ClimbComponent.h"

UCharacterBaseAnimInstance::UCharacterBaseAnimInstance()
{
	IsInAir = false;
}

void UCharacterBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<APlayerBase>(TryGetPawnOwner());

	if (Character)
	{
		Character->GetCharacterStatusComponent()->OnDeath.AddUObject(this, &UCharacterBaseAnimInstance::HandleDeath);
		Character->GetEquipmentComponent()->OnWeaponChangedDelegate.AddUObject(this, &UCharacterBaseAnimInstance::HandleWeaponChange);
	}

	AnimModeMap.Add(ECharacterState::Ground, NewObject<UAnimMode_Ground>(this));
	AnimModeMap.Add(ECharacterState::Ladder, NewObject<UAnimMode_Ladder>(this));

	for (auto& Pair : AnimModeMap)
	{
		Pair.Value->Character = Character;
		Pair.Value->AnimInst = this;
	}

	SwitchAnimMode(ECharacterState::Ground);
}

void UCharacterBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Character)
	{

		IsMovementInput = Character->GetIsMovementInput();
		CurrentState = Character->GetCharacterStatusComponent()->GetCharacterState_Native();

		const auto NewState = Character->GetCharacterStatusComponent()->GetCharacterState_Native();
		if (!CurrentMode || AnimModeMap.FindRef(NewState) != CurrentMode)
		{
			SwitchAnimMode(NewState);
		}

		if (CurrentMode)
		{
			CurrentMode->Tick(DeltaSeconds);
		}

		//IsAccelerating = Character->GetCharacterMovement()->GetCurrentAcceleration() != FVector::ZeroVector && Speed > 3.0f ? true : false;
		
		/*
		switch (CurrentState)
		{
		case ECharacterState::Ground:
		{
			//CurGroundStance = Character->GetCurGroundStance();

			Speed = Character->GetVelocity().Length();
			//Direction = GetAnimDirection(DeltaSeconds);
			IsInAir = Character->GetMovementComponent()->IsFalling();
			IsJumping = false;
			IsFalling = false;
			IsLanding = false;
			if (IsInAir)
			{
				Character->GetVelocity().Z > 0.0f ? IsJumping = true : IsFalling = true;
				auto* BaseMovement = Cast<UBaseCharacterMovementComponent>(Character->GetMovementComponent());
				if (BaseMovement)
				{
					float GroundDistance = BaseMovement->GetGroundDistance(false);
					IsLanding = GroundDistance < 50.0f;
				}
			}
			MovementAlpha = FMath::GetRangePct(400.0f, 600.0f, Speed);

			float LocomotionAnimCurrentTime = GetCurveValue(FName("LocomotionTimeCurve"));
			float LocomotionAnimLength = GetCurveValue(FName("LocomotionAnimEntireLength"));
			LocomotionAnimTime = LocomotionAnimCurrentTime / LocomotionAnimLength;

			CharacterGroundState = Character->GetCharacterStatusComponent()->GetGroundStance_Native();

			FVector WorldAcceleration = Character->GetCharacterMovement()->GetCurrentAcceleration() * FVector(1.0f, 1.0f, 0.0f);
			FVector LocalAcceleration = Character->GetActorRotation().UnrotateVector(WorldAcceleration);
			float AccelerationLength = LocalAcceleration.SizeSquared();
			IsAccelerating = !FMath::IsNearlyZero(AccelerationLength);

			if (IsAccelerating)
			{
				LastDirection = Direction;
				LastSpeed = Speed;
			}

			float BlendAlpha = CharacterGroundState == EGroundStance::Block ? 0.99f : 0.01f;
			BlockBlend = FMath::FInterpTo(BlockBlend, BlendAlpha, DeltaSeconds, 10.0f);

			TTuple<FVector, float> TraceLeftFoot = FootTrace("Foot_L");
			TTuple<FVector, float> TraceRightFoot = FootTrace("Foot_R");

			FootRotation(DeltaSeconds, TraceLeftFoot.Key, &LeftFootRotator, 20.0f);
			FootRotation(DeltaSeconds, TraceRightFoot.Key, &RightFootRotator, 20.0f);

			PelvisOffset = FMath::FInterpTo(PelvisOffset, UKismetMathLibrary::Min(TraceLeftFoot.Value, TraceRightFoot.Value), DeltaSeconds, 5.0f);
			LeftFootGroundOffset.Z = FMath::FInterpTo(LeftFootGroundOffset.Z, TraceLeftFoot.Value - PelvisOffset, DeltaSeconds, 5.0f);
			RightFootGroundOffset.Z = FMath::FInterpTo(RightFootGroundOffset.Z, TraceRightFoot.Value - PelvisOffset, DeltaSeconds, 5.0f);
			
			break;
		}
		case ECharacterState::Ladder:
		{			
			CurLadderStance = Character->GetClimbComponent()->GetLadderStance_Native();

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
		*/
	}


	//IsFirstUpdateYaw = false;
	
}

void UCharacterBaseAnimInstance::SwitchAnimMode(const ECharacterState TargetMode)
{
	if (CurrentMode)
		CurrentMode->OnModeExit();

	if (CurrentMode = AnimModeMap[TargetMode])
	{
		CurrentMode->OnModeEnter();
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

void UCharacterBaseAnimInstance::SetHitAir(bool HitState)
{
	bIsHitAir = HitState;
}

void UCharacterBaseAnimInstance::ResetHitAir_Implementation()
{
	SetHitAir(false);
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

void UCharacterBaseAnimInstance::AnimNotify_NOT_ResetClimbState()
{
	UE_LOG(LogTemp, Warning, TEXT("ResetClimbState"));
	Character->GetClimbComponent()->ResetClimbState();
}

void UCharacterBaseAnimInstance::HandleWeaponChange(EWeaponType WeaponData)
{
	UWorld* World = GetWorld();
	if (!World)
		return;

	UPlayerAnimRegistrySubsystem* PlayerAnimSubsystem = World->GetGameInstance()->GetSubsystem<UPlayerAnimRegistrySubsystem>();
	if (!PlayerAnimSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerAnimSubsystem not found"));
		return;		
	}

	const EWeaponType TargetAnimType = WeaponData;

	const FPlayerAnimSet* TargetAnimSet = PlayerAnimSubsystem->GetPlayerAnimSet(TargetAnimType);

	Locomotion_CycleBS = TargetAnimSet->Locomotion_CycleBS.IsNull() ? nullptr : TargetAnimSet->Locomotion_CycleBS.LoadSynchronous();
	Locomotion_Idle = TargetAnimSet->Locomotion_Idle.IsNull() ? nullptr : TargetAnimSet->Locomotion_Idle.LoadSynchronous();
	Locomotion_Start = TargetAnimSet->Locomotion_Start.IsNull() ? nullptr : TargetAnimSet->Locomotion_Start.LoadSynchronous();
	Locomotion_Stop_Jog = TargetAnimSet->Locomotion_Stop_Jog.IsNull() ? nullptr : TargetAnimSet->Locomotion_Stop_Jog.LoadSynchronous();
	Locomotion_Stop_Run = TargetAnimSet->Locomotion_Stop_Run.IsNull() ? nullptr : TargetAnimSet->Locomotion_Stop_Run.LoadSynchronous();

	Jump_Start = TargetAnimSet->Jump_Start.IsNull() ? nullptr : TargetAnimSet->Jump_Start.LoadSynchronous();
	Jump_Loop = TargetAnimSet->Jump_Loop.IsNull() ? nullptr : TargetAnimSet->Jump_Loop.LoadSynchronous();
	Fall_Loop = TargetAnimSet->Fall_Loop.IsNull() ? nullptr : TargetAnimSet->Fall_Loop.LoadSynchronous();
	Land_Jump = TargetAnimSet->Land_Jump.IsNull() ? nullptr : TargetAnimSet->Land_Jump.LoadSynchronous();
	Land_Fall = TargetAnimSet->Land_Fall.IsNull() ? nullptr : TargetAnimSet->Land_Fall.LoadSynchronous();
	Land_Jog = TargetAnimSet->Land_Jog.IsNull() ? nullptr : TargetAnimSet->Land_Jog.LoadSynchronous();
	Land_High = TargetAnimSet->Land_High.IsNull() ? nullptr : TargetAnimSet->Land_High.LoadSynchronous();

	HitAir_Start = TargetAnimSet->HitAir_Start.IsNull() ? nullptr : TargetAnimSet->HitAir_Start.LoadSynchronous();
	HitAir_Loop = TargetAnimSet->HitAir_Loop.IsNull() ? nullptr : TargetAnimSet->HitAir_Loop.LoadSynchronous();
	HitAir_End = TargetAnimSet->HitAir_End.IsNull() ? nullptr : TargetAnimSet->HitAir_End.LoadSynchronous();
	GetUp = TargetAnimSet->GetUp.IsNull() ? nullptr : TargetAnimSet->GetUp.LoadSynchronous();
	HitAir_Death = TargetAnimSet->HitAir_Death.IsNull() ? nullptr : TargetAnimSet->HitAir_Death.LoadSynchronous();
	Ground_Death = TargetAnimSet->Ground_Death.IsNull() ? nullptr : TargetAnimSet->Ground_Death.LoadSynchronous();

	bInitAnimSet = true;
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

/*
float UCharacterBaseAnimInstance::GetAnimDirection(float DeltaSeconds)
{
	float PrevCharacterYaw = CharacterYaw;
	CharacterYaw = Character->GetActorRotation().Yaw;

	float DeltaCharacterYaw = CharacterYaw - PrevCharacterYaw;
	float YawDeltaSpeed = UKismetMathLibrary::SafeDivide(DeltaCharacterYaw, DeltaSeconds);
	float LeanAngle = YawDeltaSpeed * 0.0375;

	return LeanAngle;
}
*/