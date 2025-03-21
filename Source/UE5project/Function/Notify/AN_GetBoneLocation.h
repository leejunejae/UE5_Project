// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_GetBoneLocation.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UAN_GetBoneLocation : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
		FName TargetBone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
		int32 Count;
};
