// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/Projectiles/PBProjectile.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"

// Sets default values
APBProjectile::APBProjectile()
{
	ProjectileCol = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCol"));
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileArrow"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DarkballParticle"));
	ProjectileParticle->bAutoActivate = true;

	SetRootComponent(ProjectileCol);
	ProjectileMesh->SetupAttachment(ProjectileCol);
	ProjectileArrow->SetupAttachment(ProjectileCol);
	ProjectileMovement->SetUpdatedComponent(ProjectileCol);

	ProjectileCol->SetEnableGravity(false);
	ProjectileMesh->SetEnableGravity(false);

	ProjectileMovement->InitialSpeed = 0.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->ProjectileGravityScale = Gravity;
	ProjectileMovement->bRotationFollowsVelocity=true;
	ProjectileMovement->bAutoActivate = false;

	ProjectileParticle->SetupAttachment(ProjectileMesh);

	ProjectileCol->SetCollisionProfileName(TEXT("Projectile"));
}

void APBProjectile::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	Target = UGameplayStatics::GetPlayerCharacter(PlayerController, 0);

	if (Target != nullptr)
	{
		if (ActorHasTag("Homing"))
		{
			ProjectileMovement->bIsHomingProjectile = true;
			ProjectileMovement->HomingAccelerationMagnitude = 2700.0f;
			ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();
		}
		else
			RotateToTarget();
	}
}

void APBProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APBProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ProjectileCol->OnComponentHit.AddDynamic(this, &APBProjectile::OnHit);
}

void APBProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("Enemy") || OtherActor->ActorHasTag("Projectile"))
		return;
	Destroy();
}

void APBProjectile::RotateToTarget()
{
	FVector StartDir = GetActorLocation();
	FVector DestDir = Target->GetActorLocation();
	FVector Direction = (DestDir - StartDir).GetSafeNormal();
	ProjectileMovement->SetVelocityInLocalSpace(Direction*Speed);
	ProjectileCol->SetWorldRotation(Direction.Rotation());
}