// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEHHeretic.h"
#include "PBEHHereticAI.h"
#include "PBEMAnimInstance.h"
#include "PBEHHereticSoldier.h"

// 스킬
#include "../../../Item/PBSkill_Teleport.h"
#include "../../../Item/PBSkill_TeleportFin.h"
#include "../../../Item/PBProjectile.h"
#include "../../../Item/PBProjectile_Darkball.h"
#include "../../../Item/PBSkill_Portal.h"
#include "../../../Item/PBSkill_DarkSawTooth.h"
#include "../../../Item/PBSkill_DarkBeam.h"
#include "../../../Item/PBSkill_DarkTornado.h"

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

	if (HereticAnim != nullptr)
	{
		HereticAnim->OnMontageEnded.AddDynamic(this, &APBEHHeretic::IsMontageEnded);
		HereticAnim->OnDarkBallStart.AddUObject(this, &APBEHHeretic::Skill_DarkBall);
		HereticAnim->OnDarkSpearStart.AddUObject(this, &APBEHHeretic::Skill_DarkSpear);
		HereticAnim->OnDarkBeamStart.AddUObject(this, &APBEHHeretic::Skill_DarkBeam);
		HereticAnim->OnSawToothStart.AddUObject(this, &APBEHHeretic::Skill_DarkSawTooth);
		HereticAnim->OnTornadoStart.AddUObject(this, &APBEHHeretic::Skill_DarkTornado);

		HereticAnim->OnDarkBallEnd.AddLambda([this]()->void {
			DarkBall = false;
			IsAction = false;
			GetWorldTimerManager().ClearTimer(DarkBallTimerHandle);
			OnAttackEnd.Broadcast();
			});
		HereticAnim->OnDarkSpearEnd.AddLambda([this]()->void {
			DarkSpear = false;
			IsAction = false;
			OnAttackEnd.Broadcast();
			});
		HereticAnim->OnDarkBeamEnd.AddLambda([this]()->void {
			//DarkBeam = false;
			IsAction = false;
			OnAttackEnd.Broadcast();
			});
		HereticAnim->OnSawToothEnd.AddLambda([this]()->void {
			SawTooth = false;
			IsAction = false;
			OnAttackEnd.Broadcast();
			});
		HereticAnim->OnTornadoEnd.AddLambda([this]()->void {
			UE_LOG(LogTemp, Warning, TEXT("TornadoEnd2"));
			//Tornado = false;
			IsAction = false;
			OnAttackEnd.Broadcast();
			});
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
	}
}

void APBEHHeretic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APBEHHeretic::Skill_Attack(HereticSkill SelectedSkill)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("HereticSkill"), true);
	FString SkillLog = EnumPtr->GetNameStringByValue((int32)SelectedSkill);
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, SkillLog);
	switch (SelectedSkill)
	{
	case HereticSkill::DarkBall:
		DarkBall = true;
		break;
	case HereticSkill::SawTooth:
		SawTooth = true;
		break;
	case HereticSkill::DarkSpear:
		DarkSpear = true;
		break;
	case HereticSkill::DarkBeam:
		DarkBeam = true;
		break;
	case HereticSkill::Tornado:
		Tornado = true;
		break;
	case HereticSkill::None:
		break;
	}
	
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

bool APBEHHeretic::CheckBool(HereticVar CheckVar)
{
	switch (CheckVar)
	{
	case HereticVar::Summon:
		return IsSummon;
	case HereticVar::Action:
		return IsAction;
	case HereticVar::DarkBall:
		return DarkBall;
	case HereticVar::SawTooth:
		return SawTooth;
	case HereticVar::DarkSpear:
		return DarkSpear;
	case HereticVar::DarkBeam:
		return DarkBeam;
	case HereticVar::Tornado:
		return Tornado;
	default:
		return false;
	}
}

TArray<HereticSkill> APBEHHeretic::CheckSkillIsReady()
{
	TArray<HereticSkill> IsReadyList;

	TArray<bool> SkillsReady = { DarkBallIsReady, SawToothIsReady, DarkSpearIsReady, DarkBeamIsReady, TornadoIsReady};

	// ���� �˻� �� ��ų �߰�
	for (int32 i = 0; i < SkillsReady.Num(); ++i)
	{
		
		if (SkillsReady[i])
		{
			IsReadyList.Add(static_cast<HereticSkill>(i));
		}
	}
	
	return IsReadyList;
}