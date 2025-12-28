// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/BlendSpace1D.h"
#include "Core/MovementTypes.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h" 
#include "AnimData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPhaseAnimByCardinal
{
    GENERATED_BODY()

public:
        // 방향별 Start/Stop (필요한 쪽만 채워도 OK)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        TMap<EDirection8Way, TObjectPtr<UAnimSequence>> StartAnims;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        TMap<EDirection8Way, TObjectPtr<UAnimSequence>> StopAnims;
};

USTRUCT(BlueprintType)
struct FLocomotionAnim
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        TObjectPtr<UBlendSpace> Locomotion_CycleBS;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        TObjectPtr<UAnimSequence> Locomotion_Idle;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        TMap<ELocomotionGait, FPhaseAnimByCardinal> LocomotionPhaseAnim;
};

USTRUCT(BlueprintType)
struct FLocomotionProfile
{
    GENERATED_BODY()
public:
        // 이 프로파일이 적용되기 위한 조건 태그(부분집합 매칭)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        FGameplayTagContainer MatchTags;

    // 실제 애니메이션 셋
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        FLocomotionAnim LocomotionAnim;
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
