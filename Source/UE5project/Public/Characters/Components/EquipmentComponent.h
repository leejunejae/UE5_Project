// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Characters/Interfaces/EquipmentDataInterface.h"
#include "Items/Weapons/Data/WeaponData.h"

#include "EquipmentComponent.generated.h"

class ACharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROJECT_API UEquipmentComponent : public UActorComponent,
	public IEquipmentDataInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<ACharacter> CachedCharacter;

	UPROPERTY(VisibleAnywhere, Category = Equipment)
		TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = Equipment)
		TObjectPtr<UStaticMeshComponent> SubEquipMesh;

	UPROPERTY(VisibleAnywhere, Category = Equipment)
		FName WeaponSocket;

	UPROPERTY(VisibleAnywhere, Category = Equipment)
		FName SubEquipSocket;

	FWeaponSetsInfo EquipedWeapon;

	FName DefaultWeaponKey = FName("DefaultSNS");

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FWeaponSetsInfo GetWeaponSetsData_Implementation() const;
	FWeaponPartInfo GetMainWeaponData_Implementation() const;
	FWeaponPartInfo GetSubEquipData_Implementation() const;
	UStaticMeshComponent* GetMainWeaponMeshComponent_Implementation() const;
	UStaticMeshComponent* GetSubEquipMeshComponent_Implementation() const;
	void EquipWeapon_Implementation(FName WeaponKey) override;

	void SetWeaponSocketName(FName SocketName) { WeaponSocket = SocketName; }
	void SetSubEquipSocketName(FName SocketName) { SubEquipSocket = SocketName; }

	bool ReCastOwner();
	bool CheckOwnerExist();
};
