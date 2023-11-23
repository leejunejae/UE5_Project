// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEHHereticSoldierAI.h"

APBEHHereticSoldierAI::APBEHHereticSoldierAI()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_AI/EHHS_BlackBoard.EHHS_BlackBoard"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_AI/EHHS_BehaviorTree.EHHS_BehaviorTree"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}