// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../../PEnumHeader.h"
#include "PBEHAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UPBEHAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPBEHAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	class APBEHuman* Character = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Classes, Meta = (AllowPrivateAccess = true))
		class UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsDead;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float CurrentSpeed;
};
