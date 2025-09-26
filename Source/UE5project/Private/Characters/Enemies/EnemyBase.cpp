// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/EnemyBase.h"
#include "Characters/Enemies/EnemyBaseAIController.h"

#include "Components/CapsuleComponent.h"

#include "Combat/Components/AttackComponent.h"
#include "Combat/Components/HitReactionComponent.h"
#include "Characters/Components/CharacterStatusComponent.h"

#include "Core/Subsystems/EnemyDataSubsystem.h"

#include "Characters/Enemies/EnemyBase.h"

// Sets default values
AEnemyBase::AEnemyBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = GetCapsuleComponent();

	NavigationInvokerComponent = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavigationInvokerComponent"));
	NavigationInvokerComponent->SetGenerationRadii(500.0f, 500.0f);

	GetMesh()->SetCollisionProfileName(TEXT("Character"));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));

	AIControllerClass = AEnemyBaseAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeEnemyData();
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBase::InitializeEnemyData()
{
	UEnemyDataSubsystem* EnemyDataSubsystem = GetGameInstance()->GetSubsystem<UEnemyDataSubsystem>();
	if (!EnemyDataSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyDataSubsystem not found"));
		//FPlatformMisc::RequestExit(false);
		return;
	}
	
	const FEnemyDataSet* EnemyData = EnemyDataSubsystem->GetEnemyData(EnemyID);

	if (!ValidateData(EnemyData))
	{
		UE_LOG(LogTemp, Error, TEXT("Required data is missing. The game will be closed. ID: %s"), *EnemyID.ToString());
		//FPlatformMisc::RequestExit(false);
	}

	const FEnemyStats* LoadedStats = EnemyData->Stats;
	if (!LoadedStats)
		return;

	Stats.InitFromDataTable(*LoadedStats);

	const FEnemyInfo* LoadedInfo = EnemyData->Info;
	
	const UEnemyInstanceDataAsset* LoadedInstance = LoadedInfo->InstanceData.Get();
	GetMesh()->SetSkeletalMesh(LoadedInstance->SkeletalMesh);
	GetMesh()->SetAnimInstanceClass(LoadedInstance->AnimBlueprint);

	const UEnemyAIDataAsset* LoadedAIData = LoadedInfo->AIData.Get();
	CachedAIController->SetControllerData(LoadedAIData->EnemyBehaviorTree, LoadedAIData->EnemyBlackboard);

	const UEnemyComponentDataAsset* LoadedComponentData = LoadedInfo->ComponentData.Get();
	HitReactionComponent->SetHitReactionDA(LoadedComponentData->HitReactionComponentData);
}

bool AEnemyBase::ValidateData(const FEnemyDataSet* EnemyData)
{
	bool bDataValid = true;

	auto CheckValid = [&](const void* Ptr, const TCHAR* Msg) -> bool
	{
		bool bResult = ensureMsgf(Ptr != nullptr, TEXT("%s for ID: %s"), Msg, *EnemyID.ToString());
		if (!bResult) bDataValid = false;
		return bResult;
	};

	if (!CheckValid(EnemyData, TEXT("EnemyData is null"))) return false;
	if (!CheckValid(EnemyData->Stats, TEXT("Stats missing"))) return false;
	if (!CheckValid(EnemyData->Info, TEXT("Info missing"))) return false;

	const UEnemyInstanceDataAsset* InstanceData = EnemyData->Info->InstanceData.LoadSynchronous();
	if (CheckValid(InstanceData, TEXT("InstanceData missing")))
	{
		CheckValid(InstanceData->SkeletalMesh, TEXT("SkeletalMesh missing"));
		CheckValid(InstanceData->AnimBlueprint, TEXT("AnimBlueprint missing"));
	}

	const UEnemyAIDataAsset* AIData = EnemyData->Info->AIData.LoadSynchronous();
	if (CheckValid(AIData, TEXT("AIData missing")))
	{
		CheckValid(AIData->EnemyBehaviorTree, TEXT("BehaviorTree missing"));
		CheckValid(AIData->EnemyBlackboard, TEXT("Blackboard missing"));
	}

	const UEnemyComponentDataAsset* ComponentData = EnemyData->Info->ComponentData.LoadSynchronous();
	if (CheckValid(ComponentData, TEXT("ComponentData missing")))
	{
		CheckValid(ComponentData->HitReactionComponentData, TEXT("HitReactionComponentData missing"));
		CheckValid(ComponentData->AttackComponentData, TEXT("AttackComponentData missing"));
	}

	return bDataValid;
}

bool AEnemyBase::ApplyDamage(const float Amount, const EAttackType AttackType)
{
	float Delta = Amount;

	switch (AttackType)
	{
	case EAttackType::PhysicalDamage:
		Delta *= (1.0f - (Stats.PhysicalDefense / (Stats.PhysicalDefense + 100.0f)));
		break;
	case EAttackType::MagicalDamage:
		Delta *= (1.0f - (Stats.MagicDefense / (Stats.MagicDefense + 100.0f)));
		break;
	case EAttackType::TrueDamage:
		break;
	}

	Stats.Health = FMath::Clamp(Stats.Health - Delta, 0.0f, Stats.MaxHealth);

	if (Stats.Health <= 0.0f)
	{
		CharacterStatusComponent->ExecuteDeath();
		return false;
	}

	return true;
}

bool AEnemyBase::ChangeStance(const float Amount, const EStatChangeType StatChangeType)
{
	float Delta = Amount;

	switch (StatChangeType)
	{
	case EStatChangeType::Damage:
		break;
	case EStatChangeType::Heal:
		Delta *= -1.0f;
		break;
	}

	Stats.Stance = FMath::Clamp(Stats.Stance - Delta, 0.0f, Stats.MaxStance);

	if (Stats.Stance <= 0.0f || Stats.Stance >= Stats.MaxStance)
	{
		Stats.Stance = Stats.MaxStance;
		return false;
	}

	return true;
}

bool AEnemyBase::ChangePoise(const float Amount, const EStatChangeType StatChangeType)
{
	float Delta = Amount;

	switch (StatChangeType)
	{
	case EStatChangeType::Damage:
		break;
	case EStatChangeType::Heal:
		Delta *= -1.0f;
		break;
	}

	Stats.Poise = FMath::Clamp(Stats.Poise - Delta, 0.0f, Stats.MaxPoise);

	if (Stats.Poise <= 0.0f || Stats.Poise >= Stats.MaxPoise)
	{
		Stats.Poise = Stats.MaxPoise;
		return false;
	}

	return true;
}

void AEnemyBase::OnHit_Implementation(const FAttackRequest& AttackInfo)
{
	float HitAngle = HitReactionComponent->CalculateHitAngle(AttackInfo.HitPoint);
	
	EHitResponse Response = HitReactionComponent->EvaluateHitResponse(AttackInfo);
	FHitReactionRequest InputReaction = { Response, HitAngle };
	HitReactionComponent->ExecuteHitResponse(InputReaction);
	
	/*
	switch (Response)
	{
	case EHitResponse::Flinch:
	case EHitResponse::KnockBack:
	case EHitResponse::KnockDown:
	{
		StatComponent->ApplyDamage(AttackInfo.Damage, AttackInfo.AttackType);
		bool IsPoiseEnough = StatComponent->ChangePoise(AttackInfo.PoiseDamage);
		if (StatComponent->GetBaseStats_Native().Poise <= 0.0f && !CharacterStatusComponent->IsDead())
		{
			FHitReactionRequest InputReaction = { Response,HitAngle };
			HitReactionComponent->ExecuteHitResponse(InputReaction);
		}
		break;
	}
	case EHitResponse::None:
	{
		StatComponent->ApplyDamage(AttackInfo.Damage, AttackInfo.AttackType);
		bool IsPoiseEnough = StatComponent->ChangePoise(AttackInfo.PoiseDamage);
		if (StatComponent->GetBaseStats_Native().Poise <= 0.0f || CharacterStatusComponent->IsDead())
		{
			CharacterBaseAnim->SetHitAir(true);
		}
		break;
	}
	case EHitResponse::Block:
	case EHitResponse::BlockLarge:
	{
		float ApplyGuardBoost = AttackInfo.StanceDamage * (1.0f - EquipmentComponent->GetWeaponSetsData_Native().GuardBoost / 100.0f);
		bool IsStaminaEnough = StatComponent->ChangeStamina(ApplyGuardBoost, ESPChangeType::Blocked);
		if (IsStaminaEnough)
		{
			float ApplyNegationDamage = AttackInfo.Damage * (1.0f - EquipmentComponent->GetWeaponSetsData_Native().GuardNegation / 100.0f);
			StatComponent->ApplyDamage(ApplyNegationDamage, AttackInfo.AttackType);
			if (!CharacterStatusComponent->IsDead())
			{
				FHitReactionRequest InputReaction = { Response, HitAngle };
				HitReactionComponent->ExecuteHitResponse(InputReaction);
			}
		}
		else
		{
			StatComponent->ApplyDamage(AttackInfo.Damage, AttackInfo.AttackType);
			Response = Response == EHitResponse::Block ? EHitResponse::BlockBreak : EHitResponse::BlockStun;
		}
		break;
	}
	}
	*/
}

void AEnemyBase::PossessedBy(AController* NewController)
{
	CachedAIController = Cast<AEnemyBaseAIController>(NewController);
}

void AEnemyBase::Attack(FName AttackName, ACharacter* Target)
{
	
}