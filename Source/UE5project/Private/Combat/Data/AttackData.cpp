// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Data/AttackData.h"

uint32 GetTypeHash(const FAttackFeature& AttackFeature)
{
	return GetTypeHash(AttackFeature.FeatureID);
}

uint32 GetTypeHash(const FAttackInfo& AttackInfo)
{
	return GetTypeHash(AttackInfo.SectionName);
}

uint32 GetTypeHash(const FNextAttackList& NextAttackList)
{
	return GetTypeHash(NextAttackList.Condition);
}