// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEHViking.h"
#include "PBEHVikingAI.h"
#include "PBEHVAnimInstance.h"
#include "../../../Item/PBWeapon.h"

APBEHViking::APBEHViking()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_Viking(TEXT("/Game/Asset/Bjorn_Viking/Mesh/SK_Bjorn_Viking_B.SK_Bjorn_Viking_B"));
	if (SK_Viking.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Viking.Object);
	}

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WEAPON_MESH(TEXT("/Game/Asset/Bjorn_Viking/Mesh/SM_Bjorn_Viking_Axe.SM_Bjorn_Viking_Axe"));
	if (WEAPON_MESH.Succeeded())
	{
		Weapon->SetStaticMesh(WEAPON_MESH.Object);
	}

	SubEquip = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SubEquipComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SUBEQUIP_MESH(TEXT("/Game/Asset/Bjorn_Viking/Mesh/SM_Bjorn_Viking_Shield.SM_Bjorn_Viking_Shield"));
	if (SUBEQUIP_MESH.Succeeded())
	{
		SubEquip->SetStaticMesh(SUBEQUIP_MESH.Object);
	}

	FName WeaponSocket(TEXT("V_Weapon"));
	FName ShieldSocket(TEXT("V_Shield"));

	if (Weapon)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
	}

	if (SubEquip)
	{
		SubEquip->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, ShieldSocket);
	}

	AIControllerClass = APBEHVikingAI::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	MeleeRadius = 150.0f;
	DefendRadius = 350.0f;
	RangedRadius = 0.0f;
}

void APBEHViking::BeginPlay()
{
	Super::BeginPlay();
	
}


void APBEHViking::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APBEHViking::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	VikingAnim = Cast<UPBEHVAnimInstance>(GetMesh()->GetAnimInstance());

	if (VikingAnim != nullptr)
	{
		VikingAnim->OnMontageEnded.AddDynamic(this, &APBEHViking::OnAttackMontageEnded);
	
	}
}

void APBEHViking::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttack = false;
	OnAttackEnd.Broadcast();
}

void APBEHViking::Attack()
{
	Super::Attack();

	if (IsAttack)
	{
		return;
	}
	VikingAnim->PlayAttackMontage();
	IsAttack = true;
}