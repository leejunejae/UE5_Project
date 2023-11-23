// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEHuman.h"
#include <Engine/Classes/Components/CapsuleComponent.h>
#include "GameFramework/CharacterMovementComponent.h"

APBEHuman::APBEHuman()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = GetCapsuleComponent();
	DamageSystem = CreateDefaultSubobject<UPBDamageSystem>(TEXT("DAMAGESYSTEM"));
	DamageSystem->bAutoActivate = true;
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	//bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 180.0f);

	Tags.Add("Enemy");
}

void APBEHuman::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DamageSystem->OnDeath.BindUFunction(this, FName("Death"));
	DamageSystem->OnBlocked.BindUFunction(this, FName("Block"));
	DamageSystem->OnDamaged.BindUFunction(this, FName("DamageResponse"));
}

void APBEHuman::Attack()
{
	
}

void APBEHuman::SetAttackInfo(float Amount, AttackType Type, HitResponse Response, bool Invincible, bool CanBlocked, bool CanParried, bool ForceInterrupt)
{
	AttackInfo.Amount = Amount;
	AttackInfo.Type = Type;
	AttackInfo.Response = Response;
	AttackInfo.Invincible = Invincible;
	AttackInfo.CanBlocked = CanBlocked;
	AttackInfo.CanParried = CanParried;
	AttackInfo.ForceInterrupt = ForceInterrupt;
}

void APBEHuman::AttackTimer()
{
	/*
	FVector StartLoc = GetMesh()->GetSocketLocation("S_RangeStart");
	FVector EndLoc = GetMesh()->GetSocketLocation("S_RangeEnd");
	*/
	if (ActorHasTag("Viking"))
	{
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams(NAME_None, false, this);

		/*
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLoc,
			EndLoc,
			ECC_Visibility,
			CollisionParams
		);
		*/

		bool bResult = GetWorld()->SweepSingleByChannel(
			HitResult,
			GetActorLocation(),
			GetActorLocation() + GetActorForwardVector() * 200.0f,
			FQuat::Identity,
			ECollisionChannel::ECC_EngineTraceChannel3,
			FCollisionShape::MakeSphere(50.0f),
			CollisionParams
		);

		FVector TraceVec = GetActorForwardVector() * 200.0f;
		FVector Center = GetActorLocation() + TraceVec * 0.5f;
		float HalfHeight = 200.0f * 0.5f + 50.0f;
		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
		FColor DrawColor = bResult ? FColor::Green : FColor::Red;
		float DebugLifeTime = 5.0f;

		DrawDebugCapsule(GetWorld(), Center, HalfHeight, 50.0f, CapsuleRot, DrawColor, false, DebugLifeTime);

		if (bResult)
		{
			if (HitResult.GetActor()->ActorHasTag("Player"))
			{
				IPBDamagableInterface* GetDamagedEnemy = Cast<IPBDamagableInterface>(HitResult.GetActor());
				GetDamagedEnemy->TakeDamage_Implementation(AttackInfo);
				if (GetWorldTimerManager().IsTimerActive(AttackTimerHandle))
					GetWorldTimerManager().ClearTimer(AttackTimerHandle);
			}
		}
	}
}

void APBEHuman::IsMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	
}

void APBEHuman::SetMovementSpeed(float speed)
{
	GetCharacterMovement()->MaxWalkSpeed = speed;
}

FRangeInfo APBEHuman::GetIdealRange()
{
	FRangeInfo ReturnRange;
	ReturnRange.Melee = MeleeRadius;
	ReturnRange.Ranged = RangedRadius;
	ReturnRange.Defend = DefendRadius;

	return ReturnRange;
}

void APBEHuman::Death()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Dead"));
	IsDead = true;
	SetActorEnableCollision(false);
}

void APBEHuman::Block(bool CanParried)
{
	if (CanParried)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Parried"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Blocked"));
	}
}

void APBEHuman::Teleport()
{
	
}

void APBEHuman::Summon()
{
	
}

void APBEHuman::Appear()
{

}

void APBEHuman::Swoop()
{

}

void APBEHuman::DamageResponse(HitResponse Response)
{
	UE_LOG(LogTemp, Warning, TEXT("Your Character Was Damaged"));
}

bool APBEHuman::TakeDamage_Implementation(FDamageInfo DamageInfo)
{
	IPBDamagableInterface::TakeDamage_Implementation(DamageInfo);

	return DamageSystem->TakeDamage(DamageInfo);
}

float APBEHuman::GetMaxHealth_Implementation()
{
	IPBDamagableInterface::GetMaxHealth_Implementation();

	return DamageSystem->GetfloatValue("MaxHealth");
}

float APBEHuman::GetHealth_Implementation()
{
	IPBDamagableInterface::GetHealth_Implementation();

	return DamageSystem->GetfloatValue("Health");
}

float APBEHuman::Heal_Implementation(float amount)
{
	IPBDamagableInterface::Heal_Implementation(amount);

	return DamageSystem->Heal(amount);
}