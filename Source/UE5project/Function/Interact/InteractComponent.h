// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROJECT_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TSet<AActor*> InteractActorList;

public:	
	// Called every frame
	void AddInteractObject(AActor* InteractObject);
	void RemoveInteractObject(AActor* InteractObject);
	AActor* FindInteractActorByDegree(AActor* StartActor, float SearchDegrees);
};
