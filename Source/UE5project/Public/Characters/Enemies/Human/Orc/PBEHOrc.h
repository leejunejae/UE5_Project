// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/Human/PBEHuman.h"
#include "PBEHOrc.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API APBEHOrc : public APBEHuman
{
	GENERATED_BODY()
	
public:
	APBEHOrc();
	virtual void Tick(float DeltaTime) override;

public:
	//FOnAttackEndDelegate OnAttackEnd;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = Animation)
		class UPBEHVAnimInstance* OrcAnim;

};
