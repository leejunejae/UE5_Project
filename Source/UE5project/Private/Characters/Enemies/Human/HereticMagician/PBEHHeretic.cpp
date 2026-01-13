// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Human/HereticMagician/PBEHHeretic.h"
#include "Characters/Enemies/Human/HereticMagician/PBEHHereticAI.h"
#include "Characters/Enemies/Human/HereticMagician/PBEMAnimInstance.h"
#include "Characters/Enemies/Human/HereticKnight/PBEHHereticSoldier.h"

// 스킬
#include "Skills/Actors/PBSkill_Teleport.h"
#include "Skills/Actors/PBSkill_TeleportFin.h"
#include "Skills/Projectiles/PBProjectile.h"
#include "Skills/Projectiles/PBProjectile_Darkball.h"
#include "Skills/Actors/PBSkill_Portal.h"
#include "Skills/Actors/PBSkill_DarkSawTooth.h"
#include "Skills/Actors/PBSkill_DarkBeam.h"
#include "Skills/Actors/PBSkill_DarkTornado.h"

APBEHHeretic::APBEHHeretic(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
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
		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
		Weapon->SetCanEverAffectNavigation(false);
	}

	AIControllerClass = APBEHHereticAI::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	DarkBallIsReady = true;
	DarkSpearIsReady = true;
	TornadoIsReady = true;
	DarkBeamIsReady = true;
	SawToothIsReady = true;

	Tags.Add("Heretic");
}

void APBEHHeretic::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	HereticAnim = Cast<UPBEMAnimInstance>(GetMesh()->GetAnimInstance());
}

void APBEHHeretic::IsMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (nullptr == HereticAnim)
		return;

}

void APBEHHeretic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APBEHHeretic::Teleport()
{
	if (IsTeleporting)
		return;
	FVector TeleLocation = GetActorLocation() + GetActorUpVector() * 90.0f + GetActorForwardVector() * -20.0f;
	//HereticAnim->PlayMontage(HereticMontage::Teleport);
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

void APBEHHeretic::Skill_DarkSawTooth()
{
	if (IsAction)
		return;
	IsAction = true;
	GetWorld()->SpawnActor<APBSkill_DarkSawTooth>(GetActorLocation() + GetActorRightVector() * 200.0f, FRotator(0.0f, 0.0f, 0.0f));
	GetWorld()->SpawnActor<APBSkill_DarkSawTooth>(GetActorLocation() - GetActorRightVector() * 200.0f, FRotator(0.0f, 0.0f, 0.0f));

	SawToothIsReady = false;
	GetWorldTimerManager().SetTimer(SawToothCoolTimeHandle, this, &APBEHHeretic::SawToothCoolTimer, 5.0f, false);
}

void APBEHHeretic::Skill_DarkBall()
{
	if (IsAction)
		return;
	IsAction = true;
	GetWorldTimerManager().SetTimer(DarkBallTimerHandle, this, &APBEHHeretic::DarkBallTimer, 1.0f, true);
	GetWorldTimerManager().SetTimer(DarkBallCoolTimeHandle, this, &APBEHHeretic::DarkBallCoolTimer, 1.0f, false);
	DarkBallIsReady = false;
}

void APBEHHeretic::Skill_DarkSpear()
{
	if (IsAction)
		return;
	IsAction = true;
	//UE_LOG(LogTemp, Warning, TEXT("DarkSpear"));
	GetWorldTimerManager().SetTimer(DarkSpearCoolTimeHandle, this, &APBEHHeretic::DarkSpearCoolTimer, 10.0f, false);
	DarkSpearIsReady = false;
}

void APBEHHeretic::Skill_DarkBeam()
{
	if (IsAction)
		return;
	IsAction = true;
	GetWorld()->SpawnActor<APBSkill_DarkBeam>(GetActorLocation() + GetActorForwardVector() * 100.0f, FRotator(0.0f, 0.0f, 0.0f));
	GetWorldTimerManager().SetTimer(DarkBeamTimerHandle, this, &APBEHHeretic::DarkBeamTimer, 2.0f, false);
	GetWorldTimerManager().SetTimer(DarkBeamCoolTimeHandle, this, &APBEHHeretic::DarkBeamCoolTimer, 30.0f, false);
	DarkBeamIsReady = false;
}

void APBEHHeretic::Skill_DarkTornado()
{
	if (IsAction)
		return;
	IsAction = true;
	GetWorld()->SpawnActor<APBSkill_DarkTornado>(GetActorLocation() + GetActorForwardVector() * 200.0f + GetActorUpVector() * -72.0f, GetActorRotation());
	GetWorldTimerManager().SetTimer(TornadoTimerHandle, this, &APBEHHeretic::TornadoTimer, 10.0f, false);
	GetWorldTimerManager().SetTimer(TornadoCoolTimeHandle, this, &APBEHHeretic::TornadoCoolTimer, 30.0f, false);
	TornadoIsReady = false;
}

void APBEHHeretic::DarkBallTimer()
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

void APBEHHeretic::DarkBeamTimer()
{
	DarkBeam = false;
}

void APBEHHeretic::TornadoTimer()
{
	Tornado = false;
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

void APBEHHeretic::DarkBallCoolTimer()
{
	DarkBallIsReady = true;
}

void APBEHHeretic::SawToothCoolTimer()
{
	SawToothIsReady = true;
}

void APBEHHeretic::DarkSpearCoolTimer()
{
	DarkSpearIsReady = true;
}

void APBEHHeretic::DarkBeamCoolTimer()
{
	DarkBeamIsReady = true;
}

void APBEHHeretic::TornadoCoolTimer()
{
	TornadoIsReady = true;
}