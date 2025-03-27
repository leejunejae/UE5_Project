// Fill out your copyright notice in the Description page of Project Settings.

#include "Ride.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Engine/Classes/Components/CapsuleComponent.h>
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Components/WidgetComponent.h"
#include "../Function/PlayerInterface.h"
#include "RideAnimInstance.h"

// Sets default values
ARide::ARide()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RideTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ride.Horse")));
	RideTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Interactable.Ride")));

	RootComponent = GetCapsuleComponent();
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>HORSE_MESH(TEXT("/Game/HorseAnimsetPro/Meshes/Horses/SK_Horse.SK_Horse"));
	if (HORSE_MESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(HORSE_MESH.Object);
	}

	Saddle = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Saddle"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SADDLE(TEXT("/Game/HorseAnimsetPro/Meshes/Props/SK_Horse_Saddle.SK_Horse_Saddle"));
	if (SADDLE.Succeeded())
	{
		Saddle->SetSkeletalMesh(SADDLE.Object);
	}
	Saddle->SetupAttachment(GetMesh());
	Saddle->SetLeaderPoseComponent(GetMesh());

	Reins = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Reins"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> REINS(TEXT("/Game/HorseAnimsetPro/Meshes/Props/SK_Horse_Reins.SK_Horse_Reins"));
	if (REINS.Succeeded())
	{
		Reins->SetSkeletalMesh(REINS.Object);
	}
	Reins->SetupAttachment(GetMesh());
	Reins->SetLeaderPoseComponent(GetMesh());

	RiderTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("RiderTrigger"));
	RiderTrigger->SetupAttachment(GetMesh());
	RiderTrigger->SetRelativeScale3D(FVector(3.5f, 3.0f, 1.5f));

	FName MountSocket(TEXT("MountPoint"));
	RiderLocation1 = CreateDefaultSubobject<USceneComponent>(TEXT("RiderLocation"));
	RiderLocation1->SetupAttachment(GetMesh(), MountSocket);

	RiderGetDownLoc = CreateDefaultSubobject<USceneComponent>(TEXT("RiderGetDownLoc"));
	RiderGetDownLoc->SetupAttachment(GetMesh());
	RiderGetDownLoc->SetRelativeLocation(FVector(0.0f, -110.0f, -90.0f));

	RiderMountLocLeft = CreateDefaultSubobject<USceneComponent>(TEXT("RiderMountLocLeft"));
	RiderMountLocLeft->SetupAttachment(GetMesh());
	RiderMountLocLeft->SetRelativeLocation(FVector(70.0f, 44.0f, 85.0f));
	RiderMountLocLeft->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	RiderMountLocRight = CreateDefaultSubobject<USceneComponent>(TEXT("RiderMountLocRight"));
	RiderMountLocRight->SetupAttachment(GetMesh());
	RiderMountLocRight->SetRelativeLocation(FVector(-70.0f, 44.0f, 85.0f));
	RiderMountLocRight->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	
	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("INTERACTWIDGET"));
	InteractWidget->SetupAttachment(GetMesh());
	static ConstructorHelpers::FClassFinder<UUserWidget>INTERACT(TEXT("/Game/Character/C_Source/InteractWidget_BP.InteractWidget_BP_C"));
	if (INTERACT.Succeeded())
	{
		InteractWidget->SetWidgetClass(INTERACT.Class);
		InteractWidget->SetDrawSize(FVector2D(70.0f, 100.0f));
	}
	InteractWidget->SetRelativeLocation(FVector(0.0f, 140.0f, 210.0f));

	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));

	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeLocation(FVector(0.0f,0.0f,90.0f));
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	bUseControllerRotationYaw = false;

	InputSetting();

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	
	static ConstructorHelpers::FClassFinder<UAnimInstance>HORSE_ANIM(TEXT("/Game/Character/Ride/R_Horse/RH_Animation/RHA_BP/RHAB_AnimBlueprint.RHAB_AnimBlueprint_C"));
	if (HORSE_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(HORSE_ANIM.Class);
	}

	CurDirection = HorseDirection::Stop;
	CurMovement = HorseMovement::Walk;

	Tags.Add("Ride");
}

void ARide::InputSetting()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>PR_Context(TEXT("/Game/Character/C_Input/R_BasicInput.R_BasicInput"));
	if (PR_Context.Succeeded())
	{
		DefaultContext = PR_Context.Object;
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

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Interact(TEXT("/Game/Character/C_Input/C_Interact.C_Interact"));
	if (IP_Interact.Succeeded())
	{
		InteractAction = IP_Interact.Object;
	}
}

// Called when the game starts or when spawned
void ARide::BeginPlay()
{
	Super::BeginPlay();
	
	CurMovement = HorseMovement::Walk;
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
void ARide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Vertical = FMath::FInterpTo(Vertical, VerticalTarget, DeltaTime, 3.0f);

	if (CanInteraction)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!PlayerController)
			return;

		APawn* PlayerPawn = PlayerController->GetPawn();
		if (!PlayerPawn)
			return;
		
		FVector StartDir = GetActorLocation();
		FVector DestDir = PlayerPawn->GetActorLocation();
		FVector Direction = (DestDir - StartDir).GetSafeNormal();
		InteractWidget->SetWorldRotation(Direction.Rotation());

		if (WidgetAlpha < 1.0f)
		{
			WidgetAlpha = FMath::FInterpTo(WidgetAlpha, 1.0f, DeltaTime, 0.5f);
			InteractWidget->SetTintColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, WidgetAlpha));
		}
	}
	else
	{
		if (WidgetAlpha > 0.0f)
		{
			WidgetAlpha = FMath::FInterpTo(WidgetAlpha, 0.0f, DeltaTime, 0.5f);
			InteractWidget->SetTintColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, WidgetAlpha));
		}
	}
}

// Called to bind functionality to input
void ARide::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARide::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ARide::Stop);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARide::Look);
		EnhancedInputComponent->BindAction(MoveSpeedToggleAction, ETriggerEvent::Triggered, this, &ARide::MoveSpeedToggle);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ARide::Interact);
	}
}

void ARide::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RiderTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARide::TriggerBegin);
	RiderTrigger->OnComponentEndOverlap.AddDynamic(this, &ARide::TriggerEnd);
}

float ARide::CheckVar(RideVar CheckVar)
{
	switch (CheckVar)
	{
	case RideVar::Horizontal:
		return Horizontal;
	case RideVar::Vertical:
		return Vertical;
	default:
		return 0.0f;
	}
}

bool ARide::CheckBool(RideVar CheckVar)
{
	switch (CheckVar)
	{
	case RideVar::IsBreak:
		return IsBreak;
	default:
		return false;
	}
}

void ARide::Move(const FInputActionValue& value)
{
	FVector2D DirectionValue = value.Get<FVector2D>();
	const FVector forward = GetActorForwardVector();
	const FVector right = GetActorRightVector();

	Horizontal = DirectionValue.X;
	
	if (FMath::IsNearlyEqual(DirectionValue.Y, 1.0f))
	{
		CurDirection = HorseDirection::Front;
		switch (CurMovement)
		{
		case HorseMovement::Walk:
			VerticalTarget = 1.0f;
			break;
		case HorseMovement::Trot:
			VerticalTarget = 2.0f;
			break;
		case HorseMovement::Canter:
			VerticalTarget = 3.0f;
			break;
		case HorseMovement::Gallop:
			VerticalTarget = 4.0f;
			break;
		case HorseMovement::Sprint:
			VerticalTarget = 5.0f;
			break;
		}
	}
	else if (FMath::IsNearlyEqual(DirectionValue.Y, -1.0f))
	{
		CurDirection = HorseDirection::Back;
		VerticalTarget = -1.0f;
	}
	else
	{
		CurDirection = HorseDirection::Stop;
		VerticalTarget = 0.0f;
	}

	//AddMovementInput(forward, DirectionValue.Y);
	//AddMovementInput(right, DirectionValue.X);
}

void ARide::Look(const FInputActionValue& value)
{
	const FVector2D LookAxisValue = value.Get<FVector2D>();
	AddControllerPitchInput(LookAxisValue.Y * 0.5f);
	AddControllerYawInput(LookAxisValue.X * 0.5f);
}

void ARide::Stop()
{
	if (CurMovement == HorseMovement::Sprint || CurMovement == HorseMovement::Gallop)
		IsBreak = true;
	CurDirection = HorseDirection::Stop;
	CurMovement = HorseMovement::Walk;
	VerticalTarget = 0.0f;
	Horizontal = 0.0f;
}

void ARide::MoveSpeedToggle()
{
	if (!(CurDirection == HorseDirection::Front) || CurMovement == HorseMovement::Sprint)
		return;

	switch (CurMovement)
	{
	case HorseMovement::Walk:
		CurMovement = HorseMovement::Trot;
		break;
	case HorseMovement::Trot:
		CurMovement = HorseMovement::Canter;
		break;
	case HorseMovement::Canter:
		CurMovement = HorseMovement::Gallop;
		break;
	case HorseMovement::Gallop:
		CurMovement = HorseMovement::Sprint;
		break;
	case HorseMovement::Sprint:
		CurMovement = HorseMovement::Walk;
		break;
	}
}

void ARide::Interact()
{
	if (!Rider || !RiderGetDownLoc)
		return;

	
	//Rider->SetActorLocation(RiderGetDownLoc->GetComponentLocation());
	if (Rider->GetClass()->ImplementsInterface(UPlayerInterface::StaticClass()))
	{
		DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		IPlayerInterface::Execute_EndInteraction(Rider, this);
	}
	
	Rider = nullptr;
}

bool ARide::FindMountPos()
{
	FVector DistRightLoc = Rider->GetActorLocation() - RiderMountLocRight->GetComponentLocation();
	FVector DistLeftLoc = Rider->GetActorLocation() - RiderMountLocLeft->GetComponentLocation();

	return DistRightLoc.Length() < DistLeftLoc.Length();
}

void ARide::TriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		CanInteraction = true;
		if (OtherActor->GetClass()->ImplementsInterface(UPlayerInterface::StaticClass()))
		{
			IPlayerInterface::Execute_RegisterInteractableActor(OtherActor, this);
		}
	}
}

void ARide::TriggerEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		CanInteraction = false;
		if (OtherActor->GetClass()->ImplementsInterface(UPlayerInterface::StaticClass()))
		{
			IPlayerInterface::Execute_DeRegisterInteractableActor(OtherActor, this);
		}
	}
}

void ARide::RegisterInteractActor_Implementation(ACharacter* InteractActor)
{
	IInteractInterface::RegisterInteractActor_Implementation(InteractActor);

	Rider = InteractActor;
}

void ARide::Interact_Implementation(ACharacter* InteractActor)
{
	IInteractInterface::Interact_Implementation(InteractActor);
	
	if (Rider != InteractActor)
		return;
	

	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(
		EAttachmentRule::KeepRelative,
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepRelative,
		false
	);

	InteractActor->AttachToComponent(RiderLocation1, AttachmentRules);
	InteractActor->SetActorLocation(RiderLocation1->GetComponentLocation());
	InteractActor->SetActorRotation(RiderLocation1->GetComponentRotation());

	FComponentTransform CameraTransform = IPlayerInterface::Execute_GetCameraData(Rider);
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

USceneComponent* ARide::GetEnterInteractLocation_Implementation(AActor* Target)
{
	IInteractInterface::GetEnterInteractLocation_Implementation(Target);

	FVector DistRightLoc = Target->GetActorLocation() - RiderMountLocRight->GetComponentLocation();
	FVector DistLeftLoc = Target->GetActorLocation() - RiderMountLocLeft->GetComponentLocation();

	if (DistRightLoc.Length() < DistLeftLoc.Length())
	{
		MountRight = true;
		return RiderMountLocRight;
	}
	else
	{
		MountRight = false;
		return RiderMountLocLeft;
	}
}

USceneComponent* ARide::GetLeftInteractLocation_Implementation()
{
	IInteractInterface::GetLeftInteractLocation_Implementation();

	return RiderGetDownLoc;
}

FComponentTransform ARide::GetCameraData_Implementation()
{
	FComponentTransform CameraTransform;
	CameraTransform.Location = Camera->GetComponentLocation();
	CameraTransform.Rotation = Camera->GetComponentRotation();
	return CameraTransform;
}

float ARide::GetRideVertical_Implementation()
{ 
	IRideInterface::GetRideVertical_Implementation();

	return Vertical;
}

float ARide::GetRideHorizontal_Implementation()
{
	IRideInterface::GetRideHorizontal_Implementation();

	return Horizontal;
}

bool ARide::GetMountDir_Implementation()
{
	IRideInterface::GetMountDir_Implementation();

	return MountRight;
}
