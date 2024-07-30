// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PEnumHeader.h"
#include "Animation/AnimInstance.h"
#include "RideAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API URideAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	URideAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	class ARide* Character = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float Vertical;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float Horizontal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsBreak;
};
