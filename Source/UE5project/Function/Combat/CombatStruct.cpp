// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatStruct.h"

// Sets default values
ACombatStruct::ACombatStruct()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACombatStruct::BeginPlay()
{
	Super::BeginPlay();
	
}

uint32 GetTypeHash(const FAttackInfo& AttackInfo)
{
	return GetTypeHash(AttackInfo.FeatureID);
}

uint32 GetTypeHash(const FAttackData& AttackData)
{
	return GetTypeHash(AttackData.SectionName);
}

uint32 GetTypeHash(const FNextAttackList& NextAttackList)
{
	return GetTypeHash(NextAttackList.Condition);
}