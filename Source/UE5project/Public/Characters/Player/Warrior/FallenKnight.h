 
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/PlayerBase.h"
#include "Components/TimeLineComponent.h"
#include "FallenKnight.generated.h"

/**
 * 
 */

class UCharacterBaseAnimInstance;

UCLASS()
class UE5PROJECT_API AFallenKnight : public APlayerBase
{
	GENERATED_BODY()
	
public:
	AFallenKnight(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

private:
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* TopMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* BottomMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* ShoesMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* GlovesMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* HatMesh;

	UPROPERTY(VisibleAnywhere, Category = Equipment)
		UStaticMeshComponent* SheatheMesh;
};