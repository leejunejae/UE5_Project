// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/Services/PBEnemy_SelectSkill_Service.h"
#include "Characters/Enemies/Human/PBEHumanAI.h"
#include "Characters/Enemies/Human/HereticMagician/PBEHHeretic.h"

UPBEnemy_SelectSkill_Service::UPBEnemy_SelectSkill_Service()
{
	NodeName = TEXT("SelectSkill");
	Interval = 1.0f;
}

void UPBEnemy_SelectSkill_Service::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

}