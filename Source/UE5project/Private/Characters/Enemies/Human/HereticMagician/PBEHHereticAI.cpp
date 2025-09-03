// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Human/HereticMagician/PBEHHereticAI.h"

APBEHHereticAI::APBEHHereticAI()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Enemy/E_Human/EH_Heretic/EHH_AI/EHH_Blackboard.EHH_Blackboard"));
	//static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Enemy/E_Human/EH_Heretic/EHH_AI/EHH_BlackBoard_2.EHH_BlackBoard_2"));
	if (BBObject.Succeeded())
	{
		CachedBB = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Enemy/E_Human/EH_Heretic/EHH_AI/EHH_BehaviorTree.EHH_BehaviorTree"));
	//static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Enemy/E_Human/EH_Heretic/EHH_AI/EHH_BehaviorTree_2.EHH_BehaviorTree_2"));
	if (BTObject.Succeeded())
	{
		CachedBT = BTObject.Object;
	}
}