// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// 인터페이스
#include "Characters/Interfaces/EquipmentDataInterface.h"
#include "Characters/Interfaces/StatInterface.h"

#include "Items/Weapons/Data/WeaponData.h"

#include "EquipmentComponent.generated.h"

class ACharacter;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipmentMulDel, const EWeaponType);

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
	TScriptInterface<IStatInterface> CachedStat;
	
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

	FORCEINLINE FWeaponSetsInfo GetWeaponSetsData_Native() const { return EquipedWeapon; }
	FORCEINLINE FWeaponPartInfo GetMainWeaponData_Native() const { return EquipedWeapon.MainWeapon; }
	FORCEINLINE FWeaponPartInfo GetSubEquipData_Native() const { return EquipedWeapon.SubWeapon; }

	FWeaponSetsInfo GetWeaponSetsData_Implementation() const { return EquipedWeapon; }
	FWeaponPartInfo GetMainWeaponData_Implementation() const { return EquipedWeapon.MainWeapon;}
	FWeaponPartInfo GetSubEquipData_Implementation() const { return EquipedWeapon.SubWeapon; }
	UStaticMeshComponent* GetMainWeaponMeshComponent_Implementation() const { return WeaponMesh; }
	UStaticMeshComponent* GetSubEquipMeshComponent_Implementation() const { return SubEquipMesh; }
	void EquipWeapon_Implementation(FName WeaponKey) override;
	FVector GetWeaponSocketLocation_Implementation(FName SocketName, bool IsSubWeapon) const;

	void SetWeaponSocketName(FName SocketName) { WeaponSocket = SocketName; }
	void SetSubEquipSocketName(FName SocketName) { SubEquipSocket = SocketName; }

	bool ReCastOwner();
	bool CheckOwnerExist();

	const bool CheckWeaponValid(bool IsSubWeapon) const;

	FOnEquipmentMulDel OnWeaponChangedDelegate;
	virtual FOnEquipmentMulDel& OnWeaponSetChanged() override { return OnWeaponChangedDelegate; }
};
