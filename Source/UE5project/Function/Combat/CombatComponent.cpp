// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "../CharacterStatusComponent.h"
#include "../../StatusData.h"
#include "StatComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

float UCombatComponent::CalculateHitAngle(const FVector HitPoint)
{
	FVector CharacterLocation = GetOwner()->GetActorLocation();
	FVector ImpactVector = HitPoint - CharacterLocation;
	FRotator HitRotator = ImpactVector.Rotation();

	float HitYaw = HitRotator.Yaw;
	float CharacterYaw = GetOwner()->GetActorRotation().Yaw;
	float HitAngle = FMath::FindDeltaAngleDegrees(CharacterYaw, HitYaw);

	return HitAngle;
}

HitResponse UCombatComponent::EvaluateHitResponse(const FAttackRequest& AttackRequest)
{
	UCharacterStatusComponent* StatusComp = GetOwner()->FindComponentByClass<UCharacterStatusComponent>();

	if (!StatusComp) return AttackRequest.Response;

	const ECharacterCombatState CombatState = StatusComp->GetCombatState();

	if (StatusComp->IsInAir() && CombatState != ECharacterCombatState::Invincible)
	{
		return HitResponse::HitAir;
	}

	switch (CombatState)
	{
	case ECharacterCombatState::Block:
	{
		float HitAngle = CalculateHitAngle(AttackRequest.HitPoint);
		if (AttackRequest.CanBlocked && (HitAngle >= -60.0f || HitAngle <= 60.0f))
		{
			bool IsStaminaEnough = true;

			UStatComponent* StatComp = GetOwner()->FindComponentByClass<UStatComponent>();
			if (StatComp)
			{
				float CurStamina = StatComp->GetStamina();
				float StaminaLoss = AttackRequest.BaseAttackPower * (1.0f - StatComp->GetDefensePower());
				if (CurStamina < StaminaLoss) IsStaminaEnough = false;
				StatComp->ChangeStamina(StaminaLoss, ESPChangeType::Blocked);
				UE_LOG(LogTemp, Warning, TEXT("%f"), StatComp->GetStamina());
			}

			switch (AttackRequest.Response)
			{
			case HitResponse::Flinch: return IsStaminaEnough ? HitResponse::Block : HitResponse::BlockBreak;
			case HitResponse::KnockBack: return IsStaminaEnough ? HitResponse::Block : HitResponse::BlockBreak;
			case HitResponse::KnockDown: return IsStaminaEnough ? HitResponse::BlockLarge : HitResponse::BlockStun;
			}
		}
		else
		{
			return AttackRequest.Response;
		}
	}
	default:
		return AttackRequest.Response;
	}
}