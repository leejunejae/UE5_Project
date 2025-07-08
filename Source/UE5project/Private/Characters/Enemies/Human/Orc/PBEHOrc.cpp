// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Human/Orc/PBEHOrc.h"
#include "Characters/Enemies/Human/Orc/PBEHOrcAI.h"
#include "Characters/Enemies/Human/Orc/PBEHOAnimInstance.h"

APBEHOrc::APBEHOrc()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_Orc(TEXT("/Game/Asset/Orc_Marauder/Meshes/SK_Orc_Marauder_E.SK_Orc_Marauder_E"));
	if (SK_Orc.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Orc.Object);
	}

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WEAPON_MESH(TEXT("/Game/Asset/Orc_Marauder/Meshes/SM_Orc_Marauder_Sword.SM_Orc_Marauder_Sword"));
	if (WEAPON_MESH.Succeeded())
	{
		Weapon->SetStaticMesh(WEAPON_MESH.Object);
	}

	/*
	SubEquip = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SubEquipComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SUBEQUIP_MESH(TEXT("/Game/Asset/Bjorn_Viking/Mesh/SM_Bjorn_Viking_Shield.SM_Bjorn_Viking_Shield"));
	if (SUBEQUIP_MESH.Succeeded())
	{
		SubEquip->SetStaticMesh(SUBEQUIP_MESH.Object);
	}
	*/

	FName WeaponSocket(TEXT("S_Weapon"));
	//FName ShieldSocket(TEXT("V_Shield"));

	if (Weapon)
	{
		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}
	
	/*
	if (SubEquip)
	{
		SubEquip->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, ShieldSocket);
	}
	*/

	AIControllerClass = APBEHOrcAI::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	MeleeRadius = 300.0f;
	DefendRadius = 350.0f;
	RangedRadius = 0.0f;
}

void APBEHOrc::BeginPlay()
{
	Super::BeginPlay();

}

void APBEHOrc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}