// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PBEHuman.h"
#include "Components/TimeLineComponent.h"
#include "PBEHHereticSoldier.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API APBEHHereticSoldier : public APBEHuman
{
	GENERATED_BODY()
	
public:
	APBEHHereticSoldier();
	void Attack() override;
	void Appear() override;
	void Rush();
	void Stealth();
	void Swoop() override;
	void Death() override;

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
		USkeletalMeshComponent* BootMesh;
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

	UPROPERTY(VisibleAnywhere, Category = Particle)
		class UNiagaraComponent* HSoldierDissolve;

	UPROPERTY(VisibleAnywhere, Category = Particle)
		class UNiagaraComponent* HSoldierSpawn;

	UPROPERTY(VisibleAnywhere, Category = Particle)
		UMaterialParameterCollection* DisORSpawnMPC;

	UPROPERTY(VisibleAnywhere, Category = Particle)
		UMaterialParameterCollectionInstance* DisORSpawnMPCInst;


// Dissolve에 사용될 Timeline 변수들
	UPROPERTY(VisibleAnywhere, Category = Timeline)
		UCurveFloat* DisORSpawnCurve;

	FTimeline DisORSpawnTimeline;
	float DisORSpawnTimelineLength;

	UFUNCTION()
		void DisORSpawnUpdate(float Value);

	UFUNCTION()
		void DisORSpawnFin();

private:
	virtual void PostInitializeComponents() override;
	void IsMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;

private:
	bool IsSwoop;
};
