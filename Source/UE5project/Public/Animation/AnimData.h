// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/BlendSpace1D.h"
#include "UObject/NoExportTypes.h"
#include "AnimData.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAnimDirection : uint8
{
    None UMETA(DisplayName = "None"),
    Forward UMETA(DisplayName = "Forward"),
    Backward UMETA(DisplayName = "Backward"),
    Right UMETA(DisplayName = "Right"),
    Left UMETA(DisplayName = "Left")
};

USTRUCT(Atomic, BlueprintType)
struct FPlayerAnimSet
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSoftObjectPtr<UBlendSpace1D> Locomotion_CycleBS;             // 무기별 보행/달리기 블렌드스페이스
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSoftObjectPtr<UAnimSequence> Locomotion_Idle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSoftObjectPtr<UAnimSequence> Locomotion_Start;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSoftObjectPtr<UAnimSequence> Locomotion_Stop_Jog;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSoftObjectPtr<UAnimSequence> Locomotion_Stop_Run;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSoftObjectPtr<UAnimSequence> Jump_Start;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSoftObjectPtr<UAnimSequence> Jump_Loop;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSoftObjectPtr<UAnimSequence> Fall_Loop;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSoftObjectPtr<UAnimSequence> Land_Jump;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSoftObjectPtr<UAnimSequence> Land_Fall;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSoftObjectPtr<UAnimSequence> Land_Jog;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSoftObjectPtr<UAnimSequence> Land_High;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSoftObjectPtr<UAnimSequence> HitAir_Start;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSoftObjectPtr<UAnimSequence> HitAir_Loop;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSoftObjectPtr<UAnimSequence> HitAir_End;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSoftObjectPtr<UAnimSequence> GetUp;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSoftObjectPtr<UAnimSequence> HitAir_Death;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSoftObjectPtr<UAnimSequence> Ground_Death;
};

UCLASS()
class UE5PROJECT_API UAnimData : public UObject
{
	GENERATED_BODY()
	
};
