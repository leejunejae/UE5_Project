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
		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}

	if (SubEquip)
	{
		SubEquip->SetupAttachment(GetMesh(), ShieldSocket);
	}

	AIControllerClass = APBEHVikingAI::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	MeleeRadius = 150.0f;
	DefendRadius = 350.0f;
	RangedRadius = 0.0f;

	Tags.Add("Viking");
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
		VikingAnim->OnMontageEnded.AddDynamic(this, &APBEHViking::IsMontageEnded);
	
	}

	/*
	VikingAnim->OnStartHit.AddLambda([this]()->void {
		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &APBEHViking::AttackTimer, 0.01f, true);
		});

	
	VikingAnim->OnEndHit.AddLambda([this]()->void {
		if (GetWorldTimerManager().IsTimerActive(AttackTimerHandle))
		{
			GetWorldTimerManager().ClearTimer(AttackTimerHandle);
		}
		else
			return;
		});
		*/
}

void APBEHViking::IsMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttack = false;
	OnAttackEnd.Broadcast();
}

void APBEHViking::Attack(FName AttackName, ACharacter* Target)
{
	Super::Attack(AttackName);

	if (IsAttack)
	{
		return;
	}
	//VikingAnim->PlayMontage(MontageType::Attack);
	IsAttack = true;
}

void APBEHViking::Death()
{
	Super::Death();
	APBEHVikingAI* VikingAI = Cast<APBEHVikingAI>(GetController());
	VikingAI->StopAI();
	//VikingAnim->PlayMontage(MontageType::Death);
	//GetMesh()->SetSimulatePhysics(true);
	//GetController()->UnPossess();
}

/*
void APBEHViking::Block(bool CanParried)
{
	Super::Block(CanParried);
}
*/