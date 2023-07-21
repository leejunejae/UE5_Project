// Fill out your copyright notice in the Description page of Project Settings.


#include "PCWarrior.h"
#include "PCWAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

APCWarrior::APCWarrior()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	
	static ConstructorHelpers::FClassFinder<UAnimInstance>WARRIOR_ANIM(TEXT("/Game/Character/C_Warrior/CW_Animation/CWA_BP/CWAB_AnimBlueprint.CWAB_AnimBlueprint_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->MaxAcceleration = 2048.0f;
	GetCharacterMovement()->GroundFriction = 0.1f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;

	IsAttack = false;
	PrevLoc = GetActorLocation() - GetMesh()->GetBoneTransform(0).GetLocation();
	NextLoc = GetActorLocation() - GetMesh()->GetBoneTransform(0).GetLocation();

	MaxCombo = 4;
	AttackEndComboState();
}

void APCWarrior::BeginPlay()
{
	Super::BeginPlay();
}

void APCWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APCWarrior::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APCWarrior::Attack);
	}
}


void APCWarrior::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	WarriorAnim = Cast<UPCWAnimInstance>(GetMesh()->GetAnimInstance());

	WarriorAnim->OnMontageEnded.AddDynamic(this, &APCWarrior::OnAttackMontageEnded);

	WarriorAnim->OnNextAttackCheck.AddLambda([this]()->void {
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			WarriorAnim->JumpToAttackMontageSection(CurrentCombo);
		}
		});
}


void APCWarrior::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttack = false;
	AttackEndComboState();
}

void APCWarrior::MoveSpeedToggle()
{
	Super::MoveSpeedToggle();
	if (!IsRun)
	{
		GetCharacterMovement()->MaxAcceleration = 0.0f;
		GetCharacterMovement()->BrakingDecelerationWalking = 1024.0f;
		GetWorldTimerManager().SetTimer(MoveSpeedTimerHandle, this, &APCWarrior::MoveSpeedTimer, 0.1f, true);
	}
	else
	{
		if(GetWorldTimerManager().IsTimerActive(MoveSpeedTimerHandle))
			GetWorldTimerManager().ClearTimer(MoveSpeedTimerHandle);
		GetCharacterMovement()->MaxAcceleration = 2048.0f;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;
	}
}

void APCWarrior::MoveSpeedTimer()
{
	if (GetVelocity().Size() <= 300.0f)
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		GetCharacterMovement()->MaxAcceleration = 2048.0f;
		GetWorldTimerManager().ClearTimer(MoveSpeedTimerHandle);
		GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;
	}
}

void APCWarrior::Attack()
{
	if (nullptr == WarriorAnim)
	{
		UE_LOG(LogTemp, Error, TEXT("ATTACKFAILED"));
		return;
	}
	Super::Attack();
	if (IsAttack)
	{
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		AttackStartComboState();
		WarriorAnim->PlayAttackMontage();
		WarriorAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttack = true;
	}
}

void APCWarrior::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void APCWarrior::AttackEndComboState()
{
	UE_LOG(LogTemp, Error, TEXT("CHECK1"));
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}