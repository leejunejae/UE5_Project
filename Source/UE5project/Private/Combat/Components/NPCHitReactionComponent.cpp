// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Components/NPCHitReactionComponent.h"

UNPCHitReactionComponent::UNPCHitReactionComponent()
{

	// ...
}

HitResponse UNPCHitReactionComponent::EvaluateHitResponse(const FAttackRequest& AttackRequest)
{
	return HitResponse();
}
