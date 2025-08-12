// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Components/PlayerHitReactionComponent.h"
#include "GameFramework/Character.h"
#include "Characters/Player/Components/PlayerStatusComponent.h"
#include "Characters/Components/StatComponent.h"

UPlayerHitReactionComponent::UPlayerHitReactionComponent()
{

}

void UPlayerHitReactionComponent::BeginPlay()
{
	Super::BeginPlay();


}


EHitResponse UPlayerHitReactionComponent::EvaluateHitResponse(const FAttackRequest& AttackRequest)
{
	const ECharacterGroundState CombatState = ICharacterStatusInterface::Execute_GetGroundState(CachedPlayerStatus.GetObject());

	if (CombatState == ECharacterGroundState::Invincible) return EHitResponse::None;

	EHitResponse FinalResponse = AttackRequest.Response;

	switch (CombatState)
	{
	case ECharacterGroundState::Hit:
	case ECharacterGroundState::Invincible:
	case ECharacterGroundState::SuperArmor:
	{
		FinalResponse = EHitResponse::None;
		break;
	}
	case ECharacterGroundState::Jump:
	{
		FinalResponse = EHitResponse::HitAir;
		break;
	}
	case ECharacterGroundState::Dodge:
	{
		if (AttackRequest.CanAvoid)
		{
			FinalResponse = EHitResponse::None;
		}
		break;
	}
	case ECharacterGroundState::Block:
	{
		float HitAngle = CalculateHitAngle(AttackRequest.HitPoint);
		if (AttackRequest.CanBlocked && (FMath::Abs(HitAngle) <= 60.0f))
		{
			switch (AttackRequest.Response)
			{
			case EHitResponse::Flinch:
			case EHitResponse::KnockBack:
				FinalResponse = EHitResponse::Block;
				break;
			case EHitResponse::KnockDown:
				FinalResponse = EHitResponse::BlockLarge;
				break;
			}
		}
		break;
	}
	case ECharacterGroundState::Parry:
	{
		float HitAngle = CalculateHitAngle(AttackRequest.HitPoint);
		if (AttackRequest.CanParried && (HitAngle >= -60.0f || HitAngle <= 60.0f))
		{
			FinalResponse = EHitResponse::None;
			// 패링시 발동될 델리게이트 추가
		}
		break;
	}
	}

	return FinalResponse;
}