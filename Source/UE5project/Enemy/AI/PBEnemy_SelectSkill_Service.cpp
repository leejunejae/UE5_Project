// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEnemy_SelectSkill_Service.h"
#include "../Human/PBEHumanAI.h"
#include "../Human/Heretic/PBEHHeretic.h"

UPBEnemy_SelectSkill_Service::UPBEnemy_SelectSkill_Service()
{
	NodeName = TEXT("SelectSkill");
	Interval = 1.0f;
}

void UPBEnemy_SelectSkill_Service::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = Cast<APBEHHeretic>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
		return;

	int32 RandSkill;

	if (!ControllingPawn->CheckBool(HereticVar::Action))
		RandSkill = FMath::RandRange(1, 2);
	else
		RandSkill = 0;

	switch (RandSkill)
	{
	case 0:
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Skill")), static_cast<uint8>(HereticSkill::None));
		break;
	case 1:
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Skill")), static_cast<uint8>(HereticSkill::SummonSoldier));
		break;
	case 2:
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Skill")), static_cast<uint8>(HereticSkill::Darkball));
		break;
	}

	return;
}