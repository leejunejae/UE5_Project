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
	
	IsReadyList = ControllingPawn->CheckSkillIsReady();
	int32 Skillindex;
	if (IsReadyList.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("SkillList is Empty"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Size of SkillList : %d"), IsReadyList.Num());
	}

	if (!ControllingPawn->CheckBool(HereticVar::Action))
		Skillindex = FMath::RandRange(0, IsReadyList.Num() - 1);
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Skill")), static_cast<uint8>(HereticSkill::None));
		return;
	}

	if (IsReadyList.IsValidIndex(Skillindex))
	{
		switch (IsReadyList[Skillindex])
		{
		case HereticSkill::DarkBall:
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Skill")), static_cast<uint8>(HereticSkill::DarkBall));
			break;
		case HereticSkill::SawTooth:
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Skill")), static_cast<uint8>(HereticSkill::SawTooth));
			break;
		case HereticSkill::DarkSpear:
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Skill")), static_cast<uint8>(HereticSkill::DarkSpear));
			break;
		case HereticSkill::DarkBeam:
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Skill")), static_cast<uint8>(HereticSkill::DarkBeam));
			break;
		case HereticSkill::Tornado:
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Skill")), static_cast<uint8>(HereticSkill::Tornado));
			break;
		default:
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("Skill")), static_cast<uint8>(HereticSkill::None));
			break;
		}
	}

	return;
}