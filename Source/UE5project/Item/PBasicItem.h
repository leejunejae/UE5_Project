// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PBasicItem.generated.h"

UCLASS()
class UE5PROJECT_API APBasicItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APBasicItem();;

	UPROPERTY(VisibleDefaultsOnly, Category = Box)
		UStaticMeshComponent* ItemMesh;
};
