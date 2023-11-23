// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PBEHuman.h"
#include "PBEHKnight.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API APBEHKnight : public APBEHuman
{
	GENERATED_BODY()
	
public:
	APBEHKnight();
	void Attack() override;
	void Appear() override;
	void Unsheathe();

	void Tick(float DeltaTime) override;

protected:
	void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* BracerMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* BodyMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* PantMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* GloveMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* CapeMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* ShoulderMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* GorgetMesh;
	UPROPERTY(VisibleAnywhere, Category = Equipment)
		USkeletalMeshComponent* ArmorMesh;

	UPROPERTY(VisibleAnywhere, Category = Animation)
		class UPBEHHSAnimInstance* HSoldierAnim;

	UPROPERTY(VisibleAnywhere, Category = Particle)
		class UNiagaraComponent* HSoldierAura;

private:
	virtual void PostInitializeComponents() override;
	void IsMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;
};
