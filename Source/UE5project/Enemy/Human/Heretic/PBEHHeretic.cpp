// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEHHeretic.h"
#include "PBEHHereticAI.h"
#include "PBEMAnimInstance.h"
#include "PBEHHereticSoldier.h"
#include "../../../Item/PBSkill_Teleport.h"
#include "../../../Item/PBSkill_TeleportFin.h"
#include "../../../Item/PBProjectile.h"
#include "../../../Item/PBProjectile_Darkball.h"
#include "../../../Item/PBSkill_Portal.h"

APBEHHeretic::APBEHHeretic()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_Heretic(TEXT("/Game/Asset/Heretic/Meshes/SK_Heretic_A.SK_Heretic_A"));
	if (SK_Heretic.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Heretic.Object);
	}

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WEAPON_MESH(TEXT("/Game/Asset/Heretic/Meshes/SM_Heretic_Staff.SM_Heretic_Staff"));
	if (WEAPON_MESH.Succeeded())
	{
		Weapon->SetStaticMesh(WEAPON_MESH.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>HERETIC_ANIM(TEXT("/Game/Enemy/E_Human/EH_Heretic/EHH_Animation/EHHA_BP/EHHA_Blueprint.EHHA_Blueprint_C"));
	if (HERETIC_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(HERETIC_ANIM.Class);
	}

	FName WeaponSocket(TEXT("S_Weapon"));

	if (Weapon)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		Weapon->SetCanEverAffectNavigation(false);
	}

	AIControllerClass = APBEHHereticAI::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Tags.Add("Heretic");
}

void APBEHHeretic::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	HereticAnim = Cast<UPBEMAnimInstance>(GetMesh()->GetAnimInstance());

	if (HereticAnim != nullptr)
	{
		HereticAnim->OnMontageEnded.AddDynamic(this, &APBEHHeretic::IsMontageEnded);
	}
}

void APBEHHeretic::IsMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (nullptr == HereticAnim)
		return;

	HereticMontage Type = HereticAnim->CheckMontage(Montage);

	switch (Type)
	{
	case HereticMontage::Teleport:
		IsTeleporting = false;
		OnTeleport.Broadcast();
		break;
	case HereticMontage::Attack:
		IsAttack = false;
		IsAction = false;
		GetWorldTimerManager().ClearTimer(DarkballTimerHandle);
		OnAttackEnd.Broadcast();
		break;
	}
}

void APBEHHeretic::Attack()
{
	if (IsAttack)
		return;
	HereticAnim->PlayMontage(HereticMontage::Attack);
	GetWorldTimerManager().SetTimer(DarkballTimerHandle, this, &APBEHHeretic::DarkballTimer, 0.5f, true);
	IsAction = true;
	IsAttack = true;
}

void APBEHHeretic::Teleport()
{
	if (IsTeleporting)
		return;
	FVector TeleLocation = GetActorLocation() + GetActorUpVector() * 90.0f + GetActorForwardVector() * -20.0f;
	HereticAnim->PlayMontage(HereticMontage::Teleport);
	APBSkill_Teleport* Teleport = GetWorld()->SpawnActor<APBSkill_Teleport>(TeleLocation, GetActorRotation());
	IsTeleporting = true;
}

void APBEHHeretic::Summon()
{
	if (IsSummon)
		return;
	GetWorldTimerManager().SetTimer(SummonSoldierTimerHandle, this, &APBEHHeretic::SummonSoldierTimer, 2.0f, true);
	NSummon = FMath::RandRange(1, 3);
	IsAction = true;
	IsSummon = true;
}

void APBEHHeretic::Skill_DarkTornado()
{
}

void APBEHHeretic::Skill_DarkSawTooth()
{

}

void APBEHHeretic::Skill_DarkBall()
{
}

void APBEHHeretic::DarkballTimer()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		FVector ProjLocation = GetActorLocation() + GetActorUpVector() * 300.0f;
		APBProjectile_Darkball* Darkball = World->SpawnActor<APBProjectile_Darkball>(ProjLocation, GetActorRotation());
		if (Darkball)
		{
			UProjectileMovementComponent* ProjectileMovement = Darkball->FindComponentByClass<UProjectileMovementComponent>();
			if (ProjectileMovement)
			{
				ProjectileMovement->Activate();
			}
		}
	}
}

void APBEHHeretic::SummonSoldierTimer()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		ACharacter* Target = UGameplayStatics::GetPlayerCharacter(World, 0);
		FVector Direction = FVector(FMath::RandRange(0.0f, 1.0f), FMath::RandRange(0.0f, 1.0f), 0.0f);
		Direction.Normalize();
		FVector SpawnLocation = Target->GetActorLocation() + Direction * 800.0f;
		APBEHHereticSoldier* HSoldier = World->SpawnActor<APBEHHereticSoldier>(SpawnLocation, GetActorRotation());
		NSummon--;
		if (NSummon == 0)
		{
			GetWorldTimerManager().ClearTimer(SummonSoldierTimerHandle);
			OnSummonEnd.Broadcast();
			IsSummon = false;
			IsAction = false;
		}
	}
}

bool APBEHHeretic::CheckBool(HereticVar CheckVar)
{
	switch (CheckVar)
	{
	case HereticVar::Summon:
		return IsSummon;
	case HereticVar::Action:
		return IsAction;
	default:
		return false;
	}
}