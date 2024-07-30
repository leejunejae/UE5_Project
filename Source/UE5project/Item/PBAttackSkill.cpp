// Fill out your copyright notice in the Description page of Project Settings.


#include "PBAttackSkill.h"

APBAttackSkill::APBAttackSkill()
{
	SkillMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("SkillMovement"));
	SkillMovement->SetUpdatedComponent(SkillCol);
	SkillMovement->ProjectileGravityScale = 0.0f;
	SkillCol->SetCollisionProfileName(TEXT("Projectile"));
	SkillMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void APBAttackSkill::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	Target = UGameplayStatics::GetPlayerCharacter(PlayerController, 0);

	if (Target != nullptr)
	{
		if (ActorHasTag("Homing"))
		{
			SkillMovement->bIsHomingProjectile = true;
			SkillMovement->HomingAccelerationMagnitude = 1000.0f;
			SkillMovement->HomingTargetComponent = Target->GetRootComponent();
		}
		else if(ActorHasTag("Projectile"))
		{
			FVector StartDir = GetActorLocation();
			FVector DestDir = Target->GetActorLocation();
			FVector Direction = (DestDir - StartDir).GetSafeNormal();
			SkillMovement->SetVelocityInLocalSpace(Direction * Speed);
			//SkillCol->SetWorldRotation(Direction.Rotation());
		}
		/*
		else
		{
			FVector StartDir = GetActorLocation();
			FVector DestDir = Target->GetActorLocation();
			FVector Direction = (DestDir - StartDir).GetSafeNormal();
			FRotator RotationDir = UKismetMathLibrary::MakeRotFromX(Direction);
			SkillCol->SetRelativeRotation(RotationDir);
		}
		*/
	}
}

void APBAttackSkill::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SkillCol->OnComponentHit.AddDynamic(this, &APBAttackSkill::OnHit);
}

void APBAttackSkill::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}