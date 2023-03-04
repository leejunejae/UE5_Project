// Fill out your copyright notice in the Description page of Project Settings.


#include "PBCharacter.h"

#include "GameFramework/SpringArmComponent.h"
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

	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetOwnerNoSee(true);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(10.0f, 0.0f, BaseEyeHeight));
	Camera->bUsePawnControlRotation = true;


	static ConstructorHelpers::FObjectFinder<USkeletalMesh>PBC_Mannequin(TEXT("/Game/Character/C_Mesh/Paladin_WProp_J_Nordstrom.Paladin_WProp_J_Nordstrom"));
	if (PBC_Mannequin.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(PBC_Mannequin.Object);
	}

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

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Jump(TEXT("/Game/Character/C_Input/C_Jump.C_Jump"));
	if (IP_Jump.Succeeded())
	{
		JumpAction = IP_Jump.Object;
	}
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

// Called to bind functionality to input
void APBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APBCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APBCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APBCharacter::Jump);
	}
}

void APBCharacter::Move(const FInputActionValue& value)
{
	const FVector2D DirectionValue = value.Get<FVector2D>();
	const FVector forward = GetActorForwardVector();
	const FVector right = GetActorRightVector();
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
	Super::Jump();
}