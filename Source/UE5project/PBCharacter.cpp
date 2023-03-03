// Fill out your copyright notice in the Description page of Project Settings.


#include "PBCharacter.h"

#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

// Sets default values
APBCharacter::APBCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	}
}

void APBCharacter::Move(const FInputActionValue& value)
{
	const bool CurrentValue = value.Get<bool>();
	if (CurrentValue)
	{
		UE_LOG(LogTemp, Warning, TEXT("MOVE"));
	}
}