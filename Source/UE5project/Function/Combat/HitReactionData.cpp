// Fill out your copyright notice in the Description page of Project Settings.


#include "HitReactionData.h"

// Sets default values
AHitReactionData::AHitReactionData()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

}

// Called when the game starts or when spawned
void AHitReactionData::BeginPlay()
{
	Super::BeginPlay();
	
}

uint32 GetTypeHash(const FHitReactionAnimData& HitReactionAnimData)
{
	return GetTypeHash(HitReactionAnimData.SectionName);
}