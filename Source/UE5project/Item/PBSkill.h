// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "NiagaraComponent.h"
#include "PBSkill.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UArrowComponent;
class UParticleSystemComponent;

UCLASS()
class UE5PROJECT_API APBSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APBSkill();

protected:
	UPROPERTY(VisibleAnywhere, Category = Skill)
		UBoxComponent* SkillCol;

	UPROPERTY(VisibleAnywhere, Category = Skill)
		UStaticMeshComponent* SkillMesh;

	UPROPERTY(VisibleAnywhere, Category = Particle)
		UParticleSystemComponent* SkillParticle;

	UPROPERTY(VisibleAnywhere, Category = Particle)
		class UNiagaraComponent* SkillNiagara;
};
