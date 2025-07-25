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


	for (UActorComponent* Comp : CachedCharacter->GetComponents())
	{
		if (Comp->GetClass()->ImplementsInterface(UStatInterface::StaticClass()))
		CachedStat = TScriptInterface<IStatInterface>(Comp);
		
		if (Comp->GetClass()->ImplementsInterface(UPlayerStatusInterface::StaticClass()))
		CachedPlayerStatus = TScriptInterface<IPlayerStatusInterface>(Comp);

		if (CachedStat && CachedPlayerStatus) break;
	}

	check(CachedStat && CachedPlayerStatus);
}


HitResponse UPlayerHitReactionComponent::EvaluateHitResponse(const FAttackRequest& AttackRequest)
{
	const ECharacterGroundState CombatState = IPlayerStatusInterface::Execute_GetGroundState(CachedPlayerStatus.GetObject());

	if (CombatState == ECharacterGroundState::Invincible) return HitResponse::None;

	HitResponse FinalResponse = AttackRequest.Response;

	switch (CombatState)
	{
	case ECharacterGroundState::Hit:
	case ECharacterGroundState::Invincible:
	{
		FinalResponse = HitResponse::None;
		break;
	}
	case ECharacterGroundState::Jump:
	{
		FinalResponse = HitResponse::HitAir;
		break;
	}
	case ECharacterGroundState::Dodge:
	{
		if (AttackRequest.CanAvoid)
		{
			FinalResponse = HitResponse::None;
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
			case HitResponse::Flinch:
			case HitResponse::KnockBack:
				FinalResponse = HitResponse::Block;
				break;
			case HitResponse::KnockDown:
				FinalResponse = HitResponse::BlockLarge;
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
			FinalResponse = HitResponse::None;
			// 패링시 발동될 델리게이트 추가
		}
		break;
	}
	}

	return FinalResponse;
}