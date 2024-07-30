// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Engine/Classes/Components/CapsuleComponent.h>
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "../Function/PBDefaultWidget.h"
#include "Blueprint/UserWidget.h"
#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
ACharacterBase::ACharacterBase()
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

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_CheckMoveInput(TEXT("/Game/Character/C_Input/C_CheckMoveInput.C_CheckMoveInput"));
	if (IP_CheckMoveInput.Succeeded())
	{
		CheckMoveAction = IP_CheckMoveInput.Object;
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

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_HeavyAttack(TEXT("/Game/Character/C_Input/C_HeavyAttack.C_HeavyAttack"));
	if (IP_HeavyAttack.Succeeded())
	{
		HeavyAttackAction = IP_HeavyAttack.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Block(TEXT("/Game/Character/C_Input/C_Block.C_Block"));
	if (IP_Block.Succeeded())
	{
		BlockAction = IP_Block.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Interact(TEXT("/Game/Character/C_Input/C_Interact.C_Interact"));
	if (IP_Interact.Succeeded())
	{
		InteractAction = IP_Interact.Object;
	}

	static ConstructorHelpers::FClassFinder<UPBDefaultWidget> DEFAULTWIDGET(TEXT("/Game/Character/C_Source/PBDefaultWidget_BP"));
	if (!ensure(DEFAULTWIDGET.Class != nullptr)) return;

	DefaultWidgetClass = DEFAULTWIDGET.Class;

	NavigationInvokerComponent = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavigationInvokerComponent"));
	NavigationInvokerComponent->SetGenerationRadii(500.0f, 500.0f);

	CameraSetting();
	Tags.Add("Player");
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
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

	if (DefaultWidgetClass)
	{
		DefaultWidget = CreateWidget<UPBDefaultWidget>(PlayerController, DefaultWidgetClass);
	}

	DamageSystem->SetHealth(GetMaxHP());
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
	float NewSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, DeltaTime, 1.0f);

	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	//UE_LOG(LogTemp, Error, TEXT("%f"), GetCharacterMovement()->MaxWalkSpeed);
}

void ACharacterBase::Initialization()
{
	//JumpHold = false;
	//JumpTime = 0;
	IsAttack = false;
	IsRun = false;
	CurrentDirection = MovementDirection::Bwd;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetCharacterMovement()->JumpZVelocity = 800.0f;
	GetCharacterMovement()->MaxFlySpeed = 350.0f;
	GetCharacterMovement()->BrakingDecelerationFlying = 1000.0f;
}

void ACharacterBase::CameraSetting()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 500.0f;
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACharacterBase::Move);
		EnhancedInputComponent->BindAction(CheckMoveAction, ETriggerEvent::Started, this, &ACharacterBase::StartMoveInput);
		EnhancedInputComponent->BindAction(CheckMoveAction, ETriggerEvent::Completed, this, &ACharacterBase::EndMoveInput);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACharacterBase::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacterBase::Jump);
		EnhancedInputComponent->BindAction(MoveSpeedToggleAction, ETriggerEvent::Completed, this, &ACharacterBase::MoveSpeedToggle);
		//EnhancedInputComponent->BindAction(SwitchStanceAction, ETriggerEvent::Triggered, this, &ACharacterBase::SwitchStance);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ACharacterBase::Dodge);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACharacterBase::Interact);
	}
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DamageSystem->OnDeath.BindUFunction(this, FName("Death"));
}

/* Input Action */
void ACharacterBase::Move(const FInputActionValue& value)
{
	FVector2D DirectionValue = value.Get<FVector2D>();
	//const FVector forward = GetActorForwardVector();
	//const FVector right = GetActorRightVector();
	
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	InputVector = FVector(DirectionValue.X, DirectionValue.Y, 0.0f);

	/*
	FVector ValueX = UKismetMathLibrary::GetForwardVector(YawRotation) * DirectionValue.Y;
	FVector ValueY = UKismetMathLibrary::GetRightVector(YawRotation) * DirectionValue.X;
	FVector Direction = ValueY + ValueX;
	FVector InputDegree = GetActorTransform().InverseTransformVectorNoScale(Direction);
	InputY = InputDegree.X;
	InputX = InputDegree.Y;
	*/

	DirectionValue.Normalize();

	//if (IsDodge)
		//return;
	if (!IsLocomotion)
		return;

	//AddMovementInput(forward, DirectionValue.Y);
	//AddMovementInput(right, DirectionValue.X);
	AddMovementInput(UKismetMathLibrary::GetForwardVector(YawRotation), DirectionValue.Y);
	AddMovementInput(UKismetMathLibrary::GetRightVector(YawRotation), DirectionValue.X);
}

void ACharacterBase::Look(const FInputActionValue& value)
{
	const FVector2D LookAxisValue = value.Get<FVector2D>();
	AddControllerPitchInput(LookAxisValue.Y * 0.5f);
	AddControllerYawInput(LookAxisValue.X * 0.5f);
	/*
	if (IsAttack)
	{
		AddControllerPitchInput(LookAxisValue.Y * 0.1f);
		AddControllerYawInput(LookAxisValue.X * 0.1f);
	}
	else
	{
		AddControllerPitchInput(LookAxisValue.Y * 0.5f);
		AddControllerYawInput(LookAxisValue.X * 0.5f);
	}
	*/
}

void ACharacterBase::StartMoveInput()
{
	IsMovementInput = true;
}

void ACharacterBase::EndMoveInput()
{
	IsMovementInput = false;
}

void ACharacterBase::Dodge()
{
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	FVector ValueX = UKismetMathLibrary::GetForwardVector(YawRotation) * InputVector.Y;
	FVector ValueY = UKismetMathLibrary::GetRightVector(YawRotation) * InputVector.X;
	FVector Direction = ValueY + ValueX;
	FVector InputDegree = GetActorTransform().InverseTransformVectorNoScale(Direction);
	float InputY = InputDegree.X;
	float InputX = InputDegree.Y;

	DodgeVector = FVector(InputX, InputY, 0.0f);
	
	float DodgeDegree;
	float OriginalLocX;
	float OriginalLocY;

	if (DodgeVector.X == 0.0f && DodgeVector.Y == 0.0f)
		DodgeDegree = 180.0f;
	else
		DodgeDegree = UKismetMathLibrary::DegAtan2(DodgeVector.X, DodgeVector.Y);

	if (DodgeDegree >= 0.0f && DodgeDegree <= 90.0f)
	{
		OriginalLocX = 5.1111f * DodgeDegree;
		OriginalLocY = 460.0f - OriginalLocX;
	}
	else if (DodgeDegree > 90.0f && DodgeDegree <= 180.0f)
	{
		OriginalLocY = -5.1111f * (DodgeDegree - 90.0f);
		OriginalLocX = 460.0f + OriginalLocY;
	}
	else if (DodgeDegree < 0.0f && DodgeDegree >= -90.0f)
	{
		OriginalLocX = 5.1111f * DodgeDegree;
		OriginalLocY = 460.0f + OriginalLocX;
	}
	else if (DodgeDegree < -90.0f && DodgeDegree >= -180.0f)
	{
		OriginalLocY = 5.1111f * (DodgeDegree + 90.0f);
		OriginalLocX = -460.0f - OriginalLocY;
	}

	UE_LOG(LogTemp, Warning, TEXT("OriginalLocX : %f, OriginalLocY : %f"), OriginalLocX, OriginalLocY);

	float OriginalLength = FMath::Sqrt(OriginalLocX * OriginalLocX + OriginalLocY * OriginalLocY);

	float AdjustFactorX = (OriginalLocX / OriginalLength) * 460.0f;
	float AdjustFactorY = (OriginalLocY / OriginalLength) * 460.0f;

	UE_LOG(LogTemp, Warning, TEXT("AdjustFactorX : %f, AdjustFactorY : %f"), AdjustFactorX, AdjustFactorY);

	float TargetLocX = AdjustFactorX - OriginalLocX;
	float TargetLocY = AdjustFactorY - OriginalLocY;;

	UE_LOG(LogTemp, Warning, TEXT("TargetLocX : %f, TargetLocY : %f"), TargetLocX, TargetLocY);
	//DodgeDirection = UKismetMathLibrary::GetForwardVector(YawRotation) * InputVector.Y + UKismetMathLibrary::GetRightVector(YawRotation) * InputVector.X;
	DodgeDirection = GetActorForwardVector() * TargetLocY + GetActorRightVector() * TargetLocX;
	//AddActorWorldOffset(DodgeDirection);
}

void ACharacterBase::Block()
{
	IsBlock = true;
}

void ACharacterBase::Jump()
{
	if (!IsLocomotion)
		return;
	Super::Jump();
	//GetCharacterMovement()->bOrientRotationToMovement = false;
}

void ACharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ACharacterBase::MoveSpeedToggle()
{
	//UE_LOG(LogTemp, Error, TEXT("TOGGLING"));
	IsRun = !IsRun;
	TargetSpeed = IsRun ? 600.0f : 300.0f;
}

void ACharacterBase::Interact()
{
	if (InteractActorList.IsEmpty())
		return;

	if (IsInteraction)
	{
		bUseControllerRotationYaw = true;
		GetController()->SetIgnoreMoveInput(false);
		GetController()->StopMovement();
		IsInteraction = false;
		return;
	}

	ACharacter* InteractActor = nullptr;

	FVector CharLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();

	float CurDotVal = 0.0f;
	float Threshold = 0.5f;
	for (auto& Act : InteractActorList)
	{
		FVector ActorLocation = Act->GetActorLocation();

		FVector ToTargetDir = ActorLocation - CharLocation;
		ToTargetDir.Normalize();

		float DotToTarget = FVector::DotProduct(ForwardVector, ToTargetDir);

		if (DotToTarget > Threshold && DotToTarget > CurDotVal)
		{
			CurDotVal = DotToTarget;
			InteractActor = Act;
		}
	}

	if (InteractActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interactable Actor Doesn't Exist"));
		return;
	}

	if (InteractActor->GetClass()->ImplementsInterface(UPBInteractInterface::StaticClass()))
	{
		if (InteractActor->ActorHasTag("NPC"))
		{
			UE_LOG(LogTemp, Warning, TEXT("Interact To NPC"));
			IPBInteractInterface::Execute_Interact(InteractActor, this);
			//GetController()->SetIgnoreMoveInput(true);
		}
		else if(InteractActor->ActorHasTag("Ride"))
		{
			IsInteraction = true;
			bUseControllerRotationYaw = false;
			IPBInteractInterface::Execute_RegisterInteractActor(InteractActor, this);
			MovetoInteractPos(InteractActor);
		}
		else if(InteractActor->ActorHasTag("Climbable"))
		{

		}
	}
}

void ACharacterBase::MovetoInteractPos(ACharacter* InteractActor)
{
	USceneComponent* Target = IPBInteractInterface::Execute_GetInteractComponent(InteractActor);
	FVector DestLoc = Target->GetComponentLocation();
	FRotator DestRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DestLoc);
	SetActorRotation(FRotator(0.0f, DestRot.Yaw, 0.0f));
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), DestLoc);
	GetController()->SetIgnoreMoveInput(true);
	
	InteractTimerDelegate.BindUObject(this, &ACharacterBase::InteractTimer, Target, InteractActor);
	GetWorldTimerManager().SetTimer(InteractTimerHandle, InteractTimerDelegate, 0.1f, true);
}

void ACharacterBase::InteractTimer(USceneComponent* Target, ACharacter* InteractActor)
{
	FVector2D CharLoc = FVector2D(GetActorLocation().X, GetActorLocation().Y);
	FVector2D TargetLoc = FVector2D(Target->GetComponentLocation().X, Target->GetComponentLocation().Y);
	float Distance = FVector2D::Distance(CharLoc, TargetLoc);
	if (Distance < 50.0f)
	{
		GetController()->StopMovement();
		DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (InteractActor->ActorHasTag("Ride"))
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			SetActorLocation(Target->GetComponentLocation());
			SetActorRotation(Target->GetComponentRotation());
			Ride = InteractActor;
			IsRide = true;
			//GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		}
		GetWorldTimerManager().ClearTimer(InteractTimerHandle);
	}
}

void ACharacterBase::MountEnd()
{
	IPBInteractInterface::Execute_Interact(Ride, this);
	//SpringArm->bUsePawnControlRotation = false;
	//GetCharacterMovement()->bUseControllerDesiredRotation = false;
	//GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	UE_LOG(LogTemp, Warning, TEXT("MountEnd2"));
}

void ACharacterBase::Attack()
{
	//UGameplayStatics::SuggestProjectileVelocity_CustomArc
}

void ACharacterBase::Death()
{
	UE_LOG(LogTemp, Warning, TEXT("Your Character was Dead"));
}

void ACharacterBase::Block(bool CanParried)
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

bool ACharacterBase::GetIsMovementInput()
{
	return IsMovementInput;
}

bool ACharacterBase::IsRiding()
{
	return IsRide;
}

float ACharacterBase::GetVertical()
{
	if (!IsRide || Ride==nullptr)
		return 0.0f;

	return IRideInterface::Execute_GetRideVertical(Ride);
}

float ACharacterBase::GetHorizontal()
{
	if (!IsRide || Ride == nullptr)
		return 0.0f;

	return IRideInterface::Execute_GetRideHorizontal(Ride);
}

FVector ACharacterBase::GetInputDirection()
{
	return DodgeVector;
}

bool ACharacterBase::GetMountDir()
{
	return IRideInterface::Execute_GetMountDir(Ride);
}

FString ACharacterBase::GetName()
{
	return CharInfo.CName;
}

int32 ACharacterBase::GetMaxHP()
{
	return CharInfo.CMaxHP;
}

int32 ACharacterBase::GetOffensePower()
{
	return CharInfo.COffensePower;
}

int32 ACharacterBase::GetDefenseCap()
{
	return CharInfo.CDefenseCap;
}

void ACharacterBase::SwitchStance()
{

}

/* Input Action */

/*
void ACharacterBase::JumpTimer()
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

bool ACharacterBase::IsPlayer_Implementation()
{
	IPBPlayerInterface::IsPlayer_Implementation();

	return false;
}

void ACharacterBase::RegisterInteractableActor_Implementation(ACharacter* Interactable)
{
	IPBPlayerInterface::RegisterInteractableActor_Implementation(Interactable);

	InteractActorList.Add(Interactable);
}

void ACharacterBase::DeRegisterInteractableActor_Implementation(ACharacter* Interactable)
{
	IPBPlayerInterface::DeRegisterInteractableActor_Implementation(Interactable);

	InteractActorList.Remove(Interactable);
}

void ACharacterBase::EndInteraction_Implementation(ACharacter* Interactable)
{
	IPBPlayerInterface::EndInteraction_Implementation(Interactable);

	if (Interactable->ActorHasTag("Ride"))
	{
		IsRide = false;

		//GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	}
}

FComponentTransform ACharacterBase::GetCameraData_Implementation()
{
	FComponentTransform CameraTransform;
	CameraTransform.Location = Camera->GetComponentLocation();
	CameraTransform.Rotation = Camera->GetComponentRotation();
	return CameraTransform;
}

void ACharacterBase::DisMountEnd()
{
	USceneComponent* GetDownLoc = IPBInteractInterface::Execute_GetEndInteractComponent(Ride);
	SetActorLocation(GetDownLoc->GetComponentLocation());
	SetActorRotation(GetDownLoc->GetRelativeRotation());
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules(
		EDetachmentRule::KeepWorld,
		false
	);
	DetachFromActor(DetachmentRules);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	
	bUseControllerRotationYaw = true;
	SpringArm->bUsePawnControlRotation = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	FComponentTransform CameraTransform = IPBInteractInterface::Execute_GetCameraData(Ride);
	Camera->SetWorldLocation(CameraTransform.Location);
	Camera->SetWorldRotation(CameraTransform.Rotation);

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(this);

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = "OnMoveComponentToEnd";
	UKismetSystemLibrary::MoveComponentTo(
		Camera,
		FVector(0.0f, 0.0f, 0.0f),
		FRotator(0.0f, 0.0f, 0.0f),
		false,
		false,
		0.5f,
		false,
		EMoveComponentAction::Type::Move,
		LatentInfo
	);

	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

// ���� �ý��� �������̽� //
void ACharacterBase::TakeDamage_Implementation(FAttackInfo DamageInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("Take Damage1"));
	IPBDamagableInterface::TakeDamage_Implementation(DamageInfo);
}

float ACharacterBase::GetMaxHealth_Implementation()
{
	IPBDamagableInterface::GetMaxHealth_Implementation();

	return DamageSystem->GetfloatValue("MaxHealth");
}

float ACharacterBase::GetHealth_Implementation()
{
	IPBDamagableInterface::GetHealth_Implementation();

	return DamageSystem->GetfloatValue("Health");
}

float ACharacterBase::Heal_Implementation(float amount)
{
	IPBDamagableInterface::Heal_Implementation(amount);

	return DamageSystem->Heal(amount);
}

// ���� �ý��� �������̽� //