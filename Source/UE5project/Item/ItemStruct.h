// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemStruct.generated.h"

UCLASS()
class UE5PROJECT_API AItemStruct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemStruct();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

