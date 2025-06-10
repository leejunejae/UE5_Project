// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackData.h"

// Sets default values
AAttackData::AAttackData()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

}

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