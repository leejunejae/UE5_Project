// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Combat/Data/DataAsset/HitReactionDataAsset.h"
#include "Combat/Data/DataAsset/EnemyAttackDataAsset.h"
#include "EnemyComponentDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UEnemyComponentDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/* 피격 컴포넌트 데이터*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComponentData")
		TObjectPtr<UHitReactionDataAsset> HitReactionComponentData;

	/* 공격 컴포넌트 데이터*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComponentData")
		TObjectPtr<UEnemyAttackDataAsset> AttackComponentData;
};
