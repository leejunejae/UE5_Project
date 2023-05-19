// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSupply.generated.h"

class UBoxComponent;

UCLASS()
class UE5PROJECT_API APSupply : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APSupply();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, Category = Box)
		TObjectPtr<UBoxComponent> Trigger;

	//UPROPERTY(VisibleAnywhere, Category = Box)
		//FActorComponentInstanceData
		//FActorComponentInstanceSourceInfo
};