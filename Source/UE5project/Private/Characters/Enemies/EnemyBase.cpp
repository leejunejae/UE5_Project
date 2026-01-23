// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/EnemyBase.h"
#include "Characters/Enemies/EnemyBaseAnimInstance.h"
#include "Characters/Enemies/EnemyBaseAIController.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Characters/Enemies/Components/CharacterStatComponent.h"
#include "Combat/Components/AttackComponent.h"
#include "Combat/Components/HitReactionComponent.h"
#include "Characters/Components/CharacterStatusComponent.h"

#include "Core/Subsystems/GameInstanceSystem/EnemyDataSubsystem.h"
#include "Core/Subsystems/GameInstanceSystem/NPCAnimRegistrySubsystem.h"

#include "Utils/CoreLog.h"

#include "Characters/Enemies/EnemyBase.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
AEnemyBase::AEnemyBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatComponent = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("StatComponent"));
	StatComponent->bAutoActivate = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Character_NPC"));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));

	AIControllerClass = AEnemyBaseAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	TeamID = 1;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	//UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), FVector(-230.f, 4400.f, 0.f));
	//InitializeEnemyData();
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//CharacterBaseAnim = Cast<UEnemyBaseAnimInstance>(GetMesh()->GetAnimInstance());
}

bool AEnemyBase::ApplyEnemyInfo(const FEnemyInfo* Info)
{
	bool bDataValid = true;

	auto CheckValid = [&](const void* Ptr, const TCHAR* Msg) -> bool
	{
		bool bResult = ensureMsgf(Ptr != nullptr, TEXT("%s for ID: %s"), Msg, *EnemyID.ToString());
		if (!bResult) bDataValid = false;
		return bResult;
	};

	if(!CheckValid(Info, TEXT("EnemyInfo is null"))) return false;

	const UEnemyInstanceDataAsset* InstanceData = Info->InstanceData.LoadSynchronous();
	if (CheckValid(InstanceData, TEXT("InstanceData missing")))
	{
		if(CheckValid(InstanceData->SkeletalMesh, TEXT("SkeletalMesh missing")))
			GetMesh()->SetSkeletalMesh(InstanceData->SkeletalMesh);

		if(CheckValid(InstanceData->AnimBlueprint, TEXT("AnimBlueprint missing")))
			GetMesh()->SetAnimInstanceClass(InstanceData->AnimBlueprint);

		CharacterBaseAnim = Cast<UEnemyBaseAnimInstance>(GetMesh()->GetAnimInstance());

		UNPCAnimRegistrySubsystem* NPCAnimRegistrySystem = GetGameInstance()->GetSubsystem<UNPCAnimRegistrySubsystem>();
		
		if (CheckValid(NPCAnimRegistrySystem, TEXT("Anim Registry System Not Valid")))
		{
			if (InstanceData->UseDefaultAnim)
			{
				FGameplayTag DefaultProfileTag = FGameplayTag::RequestGameplayTag(TEXT("Weapon.Unarmed"));

				const FLocomotionAnim* DefaultProfile = NPCAnimRegistrySystem->GetAnimProfile(InstanceData->SkeletonTag);

				if (DefaultProfile != nullptr)
				{
					AnimProfiles.Add(DefaultProfileTag, *DefaultProfile);
					CurrentProfileTag = DefaultProfileTag;

					CharacterBaseAnim->InitAnimationData(*AnimProfiles.Find(CurrentProfileTag));
				}
				else
				{
					UE_LOG(Log_NPCSpawn, Error, TEXT("DefaultProfile Not Valid"));
				}
			}
			//AnimProfiles.Add(InstanceData->WeaponTag, *NPCAnimRegistrySystem->GetAnimProfile(InstanceData->SkeletonTag, InstanceData->WeaponTag));
		}

		GaitData = InstanceData->LocomotionGaitData;
		SetCurLocomotionGait(ELocomotionGait::Walk);
		//GetCharacterMovement()->MaxWalkSpeed = InstanceData->LocomotionGaitData.Find(ELocomotionGait::Walk)->MaxSpeed;
	}

	UDataTable* CombatPatternData = Info->CombatPatternData.LoadSynchronous();
	if (CheckValid(CombatPatternData, TEXT("CombatPatternData missing")))
	{
		CachedAIController->SetCombatPatternData(CombatPatternData);
	}

	const UEnemyAIDataAsset* AIData = Info->AIData.LoadSynchronous();
	if (CheckValid(AIData, TEXT("AI missing")))
	{
		if (CheckValid(AIData->EnemyBehaviorTree, TEXT("BehaviorTree missing")) && CheckValid(AIData->EnemyBlackboard, TEXT("Blackboard missing")))
			CachedAIController->SetControllerData(AIData->EnemyBehaviorTree, AIData->EnemyBlackboard);
	}

	const UEnemyComponentDataAsset* ComponentData = Info->ComponentData.LoadSynchronous();
	if (CheckValid(ComponentData, TEXT("ComponentData missing")))
	{
		//if (CheckValid(ComponentData->AttackComponentData, TEXT("AttackComponentData missing")))
			//GetMesh()->SetSkeletalMesh(InstanceData->SkeletalMesh);

		if (CheckValid(ComponentData->HitReactionComponentData, TEXT("HitReactionComponentData missing")))
			HitReactionComponent->SetHitReactionDA(ComponentData->HitReactionComponentData);
	}

	return true;
}

bool AEnemyBase::ApplyEnemyStats(const FEnemyStats* Stat)
{
	if (!Stat)
		return false;

	FNPCStats RuntimeStats;
	RuntimeStats.BaseStats.Health.InitResource(Stat->Health);
	RuntimeStats.BaseStats.Poise.InitResource(Stat->Poise);
	RuntimeStats.BaseStats.PhysicalDefense = Stat->PhysicalDefense;
	RuntimeStats.BaseStats.MagicDefense = Stat->MagicDefense;
	RuntimeStats.BaseStats.Resistance = Stat->Resistance;
	RuntimeStats.Stance.InitResource(Stat->Stance);

	StatComponent->InitializeNPCStats(RuntimeStats);

	return true;
}

FAttackTraceSource AEnemyBase::GetAttackTraceSource_Implementation(EAttackSourceType AttackSourceType) const
{
	FAttackTraceSource OutSource;

	switch (AttackSourceType)
	{
	case EAttackSourceType::MainHand:
	{
		OutSource.TraceComponent = MainWeapon;
		OutSource.Radius = 10.0f;
	}
	case EAttackSourceType::OffHand:
	{
		OutSource.TraceComponent = SubEquip;
		OutSource.Radius = 10.0f;
	}
	}

	return OutSource;
}

FAttackDamageSource AEnemyBase::GetAttackDamageSource_Implementation() const
{
	return FAttackDamageSource();
}

void AEnemyBase::OnHit_Implementation(const FAttackRequest& AttackInfo)
{
	float HitAngle = HitReactionComponent->CalculateHitAngle(AttackInfo.HitPoint);
	
	EHitResponse Response = HitReactionComponent->EvaluateHitResponse(AttackInfo);
	//FHitReactionRequest InputReaction = { Response, HitAngle };
	//HitReactionComponent->ExecuteHitResponse(InputReaction);

	UE_LOG(Log_Hit, Log, TEXT("[EnemyBase] %s was hit by attack that required a %s"), *this->GetName(), *StaticEnum<EHitResponse>()->GetNameStringByValue((int64)Response));

	switch (Response)
	{
	case EHitResponse::Flinch:
	case EHitResponse::KnockBack:
	case EHitResponse::KnockDown:
	{
		GetStatComponent()->ApplyDamage(AttackInfo.Damage, AttackInfo.AttackType);
		GetStatComponent()->ChangePoise(AttackInfo.PoiseDamage, EStatChangeType::Damage);
		if (GetStatComponent()->GetNPCStats().BaseStats.Poise.Current <= 0.0f && !CharacterStatusComponent->IsDead())
		{
			UE_LOG(Log_Hit, Log, TEXT("[EnemyBase] %s stagger occurred"), *Owner->GetName());
			FHitReactionRequest InputReaction = { Response,HitAngle };
			HitReactionComponent->ExecuteHitResponse(InputReaction);
		}
		break;
	}
	case EHitResponse::None:
	{
		GetStatComponent()->ApplyDamage(AttackInfo.Damage, AttackInfo.AttackType);
		GetStatComponent()->ChangePoise(AttackInfo.PoiseDamage, EStatChangeType::Damage);
		if (GetStatComponent()->GetNPCStats().BaseStats.Poise.Current <= 0.0f || CharacterStatusComponent->IsDead())
		{
			CharacterBaseAnim->SetHitAir(true);
		}
		break;
	}
	case EHitResponse::Block:
	case EHitResponse::BlockLarge:
	{
		bool IsStaminaEnough = StatComponent->ChangeStance(AttackInfo.StanceDamage, EStatChangeType::Damage);
		if (IsStaminaEnough)
		{
			float ApplyNegationDamage = AttackInfo.Damage * (1.0f - GuardNegation / 100.0f);
			GetStatComponent()->ApplyDamage(ApplyNegationDamage, AttackInfo.AttackType);
			if (!CharacterStatusComponent->IsDead())
			{
				FHitReactionRequest InputReaction = { Response, HitAngle };
				HitReactionComponent->ExecuteHitResponse(InputReaction);
			}
		}
		else
		{
			GetStatComponent()->ApplyDamage(AttackInfo.Damage, AttackInfo.AttackType);
			Response = Response == EHitResponse::Block ? EHitResponse::BlockBreak : EHitResponse::BlockStun;
		}
		break;
	}
	}
}

void AEnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	CachedAIController = Cast<AEnemyBaseAIController>(NewController);
}