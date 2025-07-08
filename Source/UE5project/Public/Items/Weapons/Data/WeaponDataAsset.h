// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
//#include "Items/Weapons/Data/WeaponData.h"
#include "WeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API UWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere)
        TObjectPtr<UStaticMesh> WeaponMesh;

    UPROPERTY(EditAnywhere)
        TObjectPtr<UTexture2D> WeaponIcon;

    UPROPERTY(EditAnywhere)
        TObjectPtr<UParticleSystem> WeaponTrail;

    UPROPERTY(EditAnywhere)
        TObjectPtr<USoundBase> WeaponSound;
};
