// Fill out your copyright notice in the Description page of Project Settings.


#include "PCWarrior.h"
#include "PCWAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../PBPWShield.h"
#include "../../PBPWSword.h"

APCWarrior::APCWarrior()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>WARRIOR_MESH(TEXT("/Game/Asset/Fallen_Knight/Mesh/SK_Fallen_Knight.SK_Fallen_Knight"));
	if (WARRIOR_MESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(WARRIOR_MESH.Object);
	}

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WEAPON_MESH(TEXT("/Game/Asset/Fallen_Knight/Mesh/Separated_Mesh/Weapon/SM_Sword.SM_Sword"));
	if (WEAPON_MESH.Succeeded())
	{
		Weapon->SetStaticMesh(WEAPON_MESH.Object);
	}

	SubEquip = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SubEquipComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SUBEQUIP_MESH(TEXT("/Game/Asset/Fallen_Knight/Mesh/Separated_Mesh/Weapon/SM_Shield.SM_Shield"));
	if (SUBEQUIP_MESH.Succeeded())
	{
		SubEquip->SetStaticMesh(SUBEQUIP_MESH.Object);
	}

	FName WeaponSocket(TEXT("S_Sword"));
	FName ShieldSocket(TEXT("S_Shield"));

	if (Weapon)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
	}

	if (SubEquip)
	{
		SubEquip->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, ShieldSocket);
	}


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

	WarriorAnim->OnMontageEnded.AddDynamic(this, &APCWarrior::IsMontageEnded);

	WarriorAnim->OnNextAttackCheck.AddLambda([this]()->void {
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			WarriorAnim->JumpToAttackMontageSection(CurrentCombo);
		}
		});
		
}

void APCWarrior::IsMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (nullptr==WarriorAnim)
		return;
	MontageType Type = WarriorAnim->CheckMontage(Montage);
	
	switch (Type)
	{
	case MontageType::Attack:
		IsAttack = false;
		AttackEndComboState();
		break;
	case MontageType::Death:
		break;
	case MontageType::Hit:
		break;
	default:
		break;
	}
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
		WarriorAnim->PlayMontage(MontageType::Attack);
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

void APCWarrior::Death()
{
	Super::Death();
	if (nullptr == WarriorAnim)
		return;
	WarriorAnim->PlayMontage(MontageType::Death);
}

void APCWarrior::Block(bool CanParried)
{
	Super::Block(CanParried);
	if (CanParried)
	{
		UE_LOG(LogTemp, Warning, TEXT("Your Character Parried"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Your Character Blocked"));
	}
}

void APCWarrior::DamageResponse(HitResponse Response)
{
	Super::DamageResponse(Response);
	switch (Response)
	{
	case HitResponse::HitReaction:
		break;
	case HitResponse::Collpase:
		break;
	case HitResponse::Fall:
		break;
	case HitResponse::KnockBack:
		break;
	case HitResponse::Stagger:
		break;
	case HitResponse::Stun:
		break;
	}
}