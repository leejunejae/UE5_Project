// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Human/PBEHuman.h"

// 이동
#include "GameFramework/CharacterMovementComponent.h"

// 컴포넌트
#include "Characters/Components/StatComponent.h"
#include "Combat/Components/AttackComponent.h"

// 콜리전
#include "Components/CapsuleComponent.h"

APBEHuman::APBEHuman(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

}