// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyInstanceDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UEnemyInstanceDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
    /** 캐릭터 외형 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
        TObjectPtr<USkeletalMesh> SkeletalMesh;

    /** 애니메이션 블루프린트 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
        TSubclassOf<UAnimInstance> AnimBlueprint;
};
