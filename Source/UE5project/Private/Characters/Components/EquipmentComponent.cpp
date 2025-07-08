// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/EquipmentComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

#include "Core/Subsystems/WeaponDataSubsystem.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	/*
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	WeaponMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetGenerateOverlapEvents(false);
	*/

	/*
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WEAPON_MESH(TEXT("/Game/Asset/SnSAnimsetPro/Models/Sword/Sword.Sword"));
	if (WEAPON_MESH.Succeeded())
	{
		WeaponMesh->SetStaticMesh(WEAPON_MESH.Object);
	}
	

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SUBEQUIP_MESH(TEXT("/Game/Asset/Bjorn_Viking/Mesh/SM_Bjorn_Viking_Shield.SM_Bjorn_Viking_Shield"));
	if (SUBEQUIP_MESH.Succeeded())
	{
		SubEquipMesh->SetStaticMesh(SUBEQUIP_MESH.Object);
	}
	*/
	// ...
}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CachedCharacter = Cast<ACharacter>(GetOwner());

	if (CheckOwnerExist())
	{
		WeaponMesh = NewObject<UStaticMeshComponent>(GetOwner(), UStaticMeshComponent::StaticClass(), TEXT("WeaponMesh"));
		SubEquipMesh = NewObject<UStaticMeshComponent>(GetOwner(), UStaticMeshComponent::StaticClass(), TEXT("SubEquipMesh"));

		if (WeaponMesh && SubEquipMesh)
		{
			GetOwner()->AddInstanceComponent(WeaponMesh);
			WeaponMesh->RegisterComponent();
			WeaponMesh->AttachToComponent(CachedCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
			WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			WeaponMesh->SetGenerateOverlapEvents(false);
			WeaponMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

			GetOwner()->AddInstanceComponent(SubEquipMesh);
			SubEquipMesh->RegisterComponent();
			SubEquipMesh->AttachToComponent(CachedCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SubEquipSocket);
			SubEquipMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SubEquipMesh->SetGenerateOverlapEvents(false);
			SubEquipMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

			EquipWeapon_Implementation(DefaultWeaponKey);
		}
	}
}


// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FWeaponSetsInfo UEquipmentComponent::GetWeaponSetsData_Implementation() const
{
	return EquipedWeapon;
}

FWeaponPartInfo UEquipmentComponent::GetMainWeaponData_Implementation() const
{
	return EquipedWeapon.MainWeapon;
}

FWeaponPartInfo UEquipmentComponent::GetSubEquipData_Implementation() const
{
	return EquipedWeapon.SubWeapon;
}

UStaticMeshComponent* UEquipmentComponent::GetMainWeaponMeshComponent_Implementation() const
{
	return WeaponMesh;
}

UStaticMeshComponent* UEquipmentComponent::GetSubEquipMeshComponent_Implementation() const
{
	return SubEquipMesh;
}

void UEquipmentComponent::EquipWeapon_Implementation(FName WeaponKey)
{
	UWorld* World = GetWorld();
	if (CheckOwnerExist() && WeaponMesh && World)
	{
		UWeaponDataSubsystem* WeaponSubsystem = World->GetGameInstance()->GetSubsystem<UWeaponDataSubsystem>();
		if (!WeaponSubsystem) return;
		const FWeaponSetsInfo* FindWeapon = WeaponSubsystem->GetWeaponInfo(WeaponKey);

		if (FindWeapon)
		{
			EquipedWeapon = *FindWeapon;
			WeaponMesh->SetStaticMesh(EquipedWeapon.MainWeapon.WeaponInstance.LoadSynchronous()->WeaponMesh);
			WeaponMesh->SetRelativeTransform(EquipedWeapon.MainWeapon.WeaponTransform);
			if (EquipedWeapon.HasSubWeapon)
			{
				SubEquipMesh->SetStaticMesh(EquipedWeapon.SubWeapon.WeaponInstance.LoadSynchronous()->WeaponMesh);
				SubEquipMesh->SetRelativeTransform(EquipedWeapon.SubWeapon.WeaponTransform);
			}
		}
	}
}

bool UEquipmentComponent::ReCastOwner()
{
	CachedCharacter = Cast<ACharacter>(GetOwner());

	return CachedCharacter.IsValid() ?  true : false;
}

bool UEquipmentComponent::CheckOwnerExist()
{
	bool IsExist = true;

	if (!CachedCharacter.IsValid())
	{
		IsExist = ReCastOwner();
	}

	return IsExist;
}
