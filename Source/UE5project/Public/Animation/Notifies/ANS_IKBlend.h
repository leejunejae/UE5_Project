// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Characters/Data/IKData.h"
#include "ANS_IKBlend.generated.h"

/**
 * 
 */

UCLASS()
class UE5PROJECT_API UANS_IKBlend : public UAnimNotifyState
{
	GENERATED_BODY()

public:
    // 이 노티파이가 다루는 림별 램프들(여러 개)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
        TArray<FIKLimbData> LimbData;

    // 전체 길이 대비 추가 EaseIn/EaseOut(공통 래핑, 선택)
    //UPROPERTY(EditAnywhere, Category = "IK") float GlobalEaseInTime = 0.f;
    //UPROPERTY(EditAnywhere, Category = "IK") float GlobalEaseOutTime = 0.f;

    // 종료 시 To=1로 러치(접촉 유지 완료)할지
    //UPROPERTY(EditAnywhere, Category = "IK") bool bLatchToAtEnd = true;

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Anim, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

    virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Anim, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
    
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Anim, const FAnimNotifyEventReference& EventReference) override;

private:
    float TotalLen = 0.f;
    float Elapsed = 0.f;

    bool bAlphaBlendCurrentType;
};
