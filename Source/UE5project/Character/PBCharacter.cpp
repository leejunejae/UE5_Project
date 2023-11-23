// Fill out your copyright notice in the Description page of Project Settings.


#include "PBCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Engine/Classes/Components/CapsuleComponent.h>
#include "Kismet/KismetMathLibrary.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

// Sets default values
APBCharacter::APBCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = GetCapsuleComponent();
	DamageSystem = CreateDefaultSubobject<UPBDamageSystem>(TEXT("DAMAGESYSTEM"));
	DamageSystem->bAutoActivate = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Character"));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	//GetMesh()->SetOwnerNoSee(true);

	static ConstructorHelpers::FObjectFinder<UInputMappingContext>PBC_Context(TEXT("/Game/Character/C_Input/C_BasicInput.C_BasicInput"));
	if (PBC_Context.Succeeded())
	{
		DefaultContext = PBC_Context.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Move(TEXT("/Game/Character/C_Input/C_Move.C_Move"));
	if (IP_Move.Succeeded())
	{
		MoveAction = IP_Move.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Look(TEXT("/Game/Character/C_Input/C_Look.C_Look"));
	if (IP_Look.Succeeded())
	{
		LookAction = IP_Look.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Dodge(TEXT("/Game/Character/C_Input/C_Dodge.C_Dodge"));
	if (IP_Dodge.Succeeded())
	{
		DodgeAction = IP_Dodge.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_MoveSpeedToggle(TEXT("/Game/Character/C_Input/C_MoveSpeedToggle.C_MoveSpeedToggle"));
	if (IP_MoveSpeedToggle.Succeeded())
	{
		MoveSpeedToggleAction = IP_MoveSpeedToggle.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_ReadinessToggle(TEXT("/Game/Character/C_Input/C_ReadinessToggle.C_ReadinessToggle"));
	if (IP_ReadinessToggle.Succeeded())
	{
		ReadinessToggleAction = IP_ReadinessToggle.Object;
	}

	/*
	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Jump(TEXT("/Game/Character/C_Input/C_Jump.C_Jump"));
	if (IP_Jump.Succeeded())
	{
		JumpAction = IP_Jump.Object;
	}
	*/

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Attack(TEXT("/Game/Character/C_Input/C_Attack.C_Attack"));
	if (IP_Attack.Succeeded())
	{
		AttackAction = IP_Attack.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_HeavyAttack(TEXT("/Game/Character/C_Input/C_HeavyAttack.C_HeavyAttack"));
	if (IP_HeavyAttack.Succeeded())
	{
		HeavyAttackAction = IP_HeavyAttack.Object;
	}

	CameraSetting();
	Tags.Add("Player");
}

// Called when the game starts or when spawned
void APBCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(DefaultContext, 0);
		}
	}
}

// Called every frame
void APBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APBCharacter::Initialization()
{
	//JumpHold = false;
	//JumpTime = 0;
	IsAttack = false;
	IsRun = false;
	CurrentReadiness = CharacterReadiness::Normal;
	CurrentDirection = MovementDirection::Bwd;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void APBCharacter::CameraSetting()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 300.0f;
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
}

// Called to bind functionality to input
void APBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APBCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APBCharacter::Look);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APBCharacter::Jump);
		EnhancedInputComponent->BindAction(MoveSpeedToggleAction, ETriggerEvent::Completed, this, &APBCharacter::MoveSpeedToggle);
		EnhancedInputComponent->BindAction(ReadinessToggleAction, ETriggerEvent::Triggered, this, &APBCharacter::ReadinessToggle);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &APBCharacter::Dodge);
	}
}

void APBCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DamageSystem->OnDeath.BindUFunction(this, FName("Death"));
	DamageSystem->OnBlocked.BindUFunction(this, FName("Block"));
	DamageSystem->OnDamaged.BindUFunction(this, FName("DamageResponse"));
}

/* Input Action */
void APBCharacter::Move(const FInputActionValue& value)
{
	if (IsDodge)
		return;
	const FVector2D DirectionValue = value.Get<FVector2D>();
	const FVector forward = GetActorForwardVector();
	const FVector right = GetActorRightVector();

	DodgeDirection = DirectionValue;
	UE_LOG(LogTemp, Error, TEXT("%f, %f"), DirectionValue.Y, DirectionValue.X);
	AddMovementInput(forward, DirectionValue.Y);
	AddMovementInput(right, DirectionValue.X);
}

void APBCharacter::Look(const FInputActionValue& value)
{
	const FVector2D LookAxisValue = value.Get<FVector2D>();
	if (IsAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f"), SpringArm->GetRelativeRotation().Yaw);
		AddControllerPitchInput(LookAxisValue.Y * 0.1f);
		AddControllerYawInput(LookAxisValue.X * 0.1f);
	}
	else
	{
		AddControllerPitchInput(LookAxisValue.Y * 0.5f);
		AddControllerYawInput(LookAxisValue.X * 0.5f);
	}
}

void APBCharacter::Dodge()
{
	float Direction = UKismetAnimationLibrary::CalculateDirection(GetVelocity(), GetActorRotation());
	if (GetVelocity().IsNearlyZero())
	{
		CurrentDirection = MovementDirection::Bwd;
	}
	else
	{
		if (Direction <= 45.0f && Direction >= -45.0f)
		{
			CurrentDirection = MovementDirection::Fwd;
		}
		else if (Direction <-45.0f && Direction >-135.0f)
			CurrentDirection = MovementDirection::Left;
		else if (Direction > 45.0f && Direction < 135.0f)
			CurrentDirection = MovementDirection::Right;
		else
		{
			CurrentDirection = MovementDirection::Bwd;
		}
	}

	/*
	//GetWorldTimerManager().SetTimer(DodgeTimerHandle, this, &APBCharacter::DodgeTimer, 0.1f, true);
	FVector DesLocation = GetActorForwardVector() * DodgeDirection.Y + GetActorRightVector() * DodgeDirection.X;
	DesLocation.Normalize();
	LaunchCharacter(DesLocation * 800.0f, false, false);
	//SetActorLocation(FMath::VInterpTo(GetActorLocation(), DesLocation, 50.0f, 2.0f));
	*/
	IsDodge = true;
}

/*
void APBCharacter::Jump()
{
	if (!JumpHold)
	{
		Super::Jump();
		JumpHold = true;
		GetWorldTimerManager().SetTimer(JumpTimerHandle, this, &APBCharacter::JumpTimer, 1.0f, true);
	}
}
*/

void APBCharacter::MoveSpeedToggle()
{
	//UE_LOG(LogTemp, Error, TEXT("TOGGLING"));
	IsRun = !IsRun;
}

void APBCharacter::Attack()
{
	CurrentReadiness = CharacterReadiness::Combat;

}

void APBCharacter::Death()
{
	UE_LOG(LogTemp, Warning, TEXT("Your Character was Dead"));
}

void APBCharacter::Block(bool CanParried)
{
	if (CanParried)
	{
		UE_LOG(LogTemp, Warning, TEXT("Your Character Parried"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Your Character Blocked"));
	}
}

void APBCharacter::DamageResponse(HitResponse Response)
{
	UE_LOG(LogTemp, Warning, TEXT("Your Character Was Damaged"));
}

void APBCharacter::ReadinessToggle()
{
	CurrentReadiness = (CurrentReadiness == CharacterReadiness::Normal) ? CharacterReadiness::Combat : CharacterReadiness::Normal;
}

/* Input Action */

/*
void APBCharacter::JumpTimer()
{
	if (JumpHold && !GetMovementComponent()->IsFalling())
	{
		JumpTime++;
		if (JumpTime > 1)
		{
			JumpTime = 0;
			JumpHold = false;
			GetWorldTimerManager().ClearTimer(JumpTimerHandle);
		}
	}
}
*/

bool APBCharacter::ReturnReadiness()
{
	if (CurrentReadiness == CharacterReadiness::Combat)
		return true;
	else
		return false;
}


// 전투 시스템 인터페이스 //
bool APBCharacter::TakeDamage_Implementation(FDamageInfo DamageInfo)
{
	IPBDamagableInterface::TakeDamage_Implementation(DamageInfo);

	return DamageSystem->TakeDamage(DamageInfo);
}

float APBCharacter::GetMaxHealth_Implementation()
{
	IPBDamagableInterface::GetMaxHealth_Implementation();

	return DamageSystem->GetfloatValue("MaxHealth");
}

float APBCharacter::GetHealth_Implementation()
{
	IPBDamagableInterface::GetHealth_Implementation();

	return DamageSystem->GetfloatValue("Health");
}

float APBCharacter::Heal_Implementation(float amount)
{
	IPBDamagableInterface::Heal_Implementation(amount);

	return DamageSystem->Heal(amount);
}

// 전투 시스템 인터페이스 //