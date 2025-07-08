// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AnimBoneTransformDataAsset.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FBoneFrameSample
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        float Time;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        FTransform BoneTransform;

    FBoneFrameSample()
        : Time(0.f), BoneTransform(FTransform::Identity)
    {
    }

    FBoneFrameSample(float InTime, const FTransform& InTransform)
        : Time(InTime), BoneTransform(InTransform) {}
};

UCLASS()
class UE5PROJECT_API UAnimBoneTransformDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        TArray<FBoneFrameSample> BoneTransformArray;

    UFUNCTION(BlueprintCallable)
        FTransform GetTransformAtTime(float QueryTime) const;
};
