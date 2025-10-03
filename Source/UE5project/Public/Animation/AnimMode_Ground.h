// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimModeBase.h"
#include "AnimMode_Ground.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UAnimMode_Ground : public UAnimModeBase
{
	GENERATED_BODY()
	
public:
	virtual void OnModeEnter() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	// === 지상 전용 상태(누적/보간 캐시 등) ===
	float PrevYaw = 0.f;            // GetAnimDirection 대체용
	bool  bFirstTick = true;

	// 튜닝 값
	UPROPERTY(EditAnywhere, Category = "Ground|Lean") float LeanScale = 0.0375f;   // 기존 GetAnimDirection 계수:contentReference[oaicite:2]{index=2}
	UPROPERTY(EditAnywhere, Category = "Ground|IK")   float TraceDistance = 50.f;  // FootTrace 거리:contentReference[oaicite:3]{index=3}
	UPROPERTY(EditAnywhere, Category = "Ground|IK")   float IKInterpSpeed = 5.f;   // 골반/발 보간
	UPROPERTY(EditAnywhere, Category = "Ground|IK")   float RotInterpSpeed = 20.f; // 발 회전 보간

	// === 내부 계산 함수들(원래 AnimInstance에 있던 것 이전) ===
	float   ComputeLeanYawPerSecond(float DeltaSeconds);
	TTuple<FVector, float> FootTrace(class USkeletalMeshComponent* Mesh, class UCapsuleComponent* Capsule, FName Socket);
	void    FootRotation(float DeltaSeconds, const FVector& TargetNormal, FRotator& InOutRot);

	// === 지상 로직 보조 ===
	void    UpdateFootIK(float DeltaSeconds, float Speed);
};
