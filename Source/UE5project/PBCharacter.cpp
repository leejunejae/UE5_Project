// Fill out your copyright notice in the Description page of Project Settings.


#include "PBCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Engine/Classes/Components/CapsuleComponent.h>

#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

// Sets default values
APBCharacter::APBCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = GetCapsuleComponent();

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

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_MoveSpeedToggle(TEXT("/Game/Character/C_Input/C_MoveSpeedToggle.C_MoveSpeedToggle"));
	if (IP_MoveSpeedToggle.Succeeded())
	{
		MoveSpeedToggleAction = IP_MoveSpeedToggle.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Jump(TEXT("/Game/Character/C_Input/C_Jump.C_Jump"));
	if (IP_Jump.Succeeded())
	{
		JumpAction = IP_Jump.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Attack(TEXT("/Game/Character/C_Input/C_Attack.C_Attack"));
	if (IP_Attack.Succeeded())
	{
		AttackAction = IP_Attack.Object;
	}

	CameraSetting();
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
	JumpHold = false;
	JumpTime = 0;
	IsRun = false;
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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APBCharacter::Jump);
		EnhancedInputComponent->BindAction(MoveSpeedToggleAction, ETriggerEvent::Completed, this, &APBCharacter::MoveSpeedToggle);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APBCharacter::Attack);
	}
}

void APBCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void APBCharacter::Move(const FInputActionValue& value)
{
	const FVector2D DirectionValue = value.Get<FVector2D>();
	const FVector forward = GetActorForwardVector();
	const FVector right = GetActorRightVector();

	UE_LOG(LogTemp, Error, TEXT("%f"), GetVelocity().Size());

	AddMovementInput(forward, DirectionValue.Y);
	AddMovementInput(right, DirectionValue.X);
}

void APBCharacter::Look(const FInputActionValue& value)
{
	const FVector2D LookAxisValue = value.Get<FVector2D>();
	AddControllerPitchInput(LookAxisValue.Y);
	AddControllerYawInput(LookAxisValue.X);
}

void APBCharacter::Jump()
{
	if (!JumpHold)
	{
		Super::Jump();
		JumpHold = true;
		GetWorldTimerManager().SetTimer(JumpTimerHandle, this, &APBCharacter::JumpTimer, 1.0f, true);
	}
}

void APBCharacter::MoveSpeedToggle()
{
	UE_LOG(LogTemp, Error, TEXT("TOGGLING"));
	IsRun = !IsRun;
}

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

void APBCharacter::Attack()
{
	IsAttack = true;
}