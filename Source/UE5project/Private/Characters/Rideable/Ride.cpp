// Fill out your copyright notice in the Description page of Project Settings.

// 기본 헤더
#include "Characters/Rideable/Ride.h"

// 카메라
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// 이동
#include "GameFramework/CharacterMovementComponent.h"

// 콜리전
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

// 입력
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

// Kismet 유틸리티
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetTextLibrary.h"

// UI
#include "Components/WidgetComponent.h"

// 인터페이스
#include "Characters/Player/Interfaces/PlayerInterface.h"

// 애니메이션
#include "Characters/Rideable/RideAnimInstance.h"




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

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Ride"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>HORSE_MESH(TEXT("/Game/00_Character/Ride/R_Horse/RH_Mesh/SK_Horse.SK_Horse"));
	if (HORSE_MESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(HORSE_MESH.Object);
	}
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(-50.0f, 0.0f, -90.0f));

	Saddle = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Saddle"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SADDLE(TEXT("/Game/00_Character/Ride/R_Horse/RH_Mesh/SK_Horse_Saddle.SK_Horse_Saddle"));
	if (SADDLE.Succeeded())
	{
		Saddle->SetSkeletalMesh(SADDLE.Object);
	}
	Saddle->SetupAttachment(GetMesh());
	Saddle->SetLeaderPoseComponent(GetMesh());

	Reins = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Reins"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> REINS(TEXT("/Game/00_Character/Ride/R_Horse/RH_Mesh/SK_Horse_Reins.SK_Horse_Reins"));
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
	RiderLocation = CreateDefaultSubobject<USceneComponent>(TEXT("RiderLocation"));
	RiderLocation->SetupAttachment(RootComponent, MountSocket);
	RiderLocation->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
	//RiderLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

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
	static ConstructorHelpers::FClassFinder<UUserWidget>INTERACT(TEXT("/Game/00_Character/Data/InteractWidget_BP.InteractWidget_BP_C"));
	if (INTERACT.Succeeded())
	{
		InteractWidget->SetWidgetClass(INTERACT.Class);
		InteractWidget->SetDrawSize(FVector2D(70.0f, 100.0f));
	}
	InteractWidget->SetRelativeLocation(FVector(0.0f, 140.0f, 210.0f));
	InteractWidget->bOwnerNoSee = true;

	SpringArm->TargetArmLength = 200.0f;
	SpringArm->SetRelativeLocation(FVector(0.0f,0.0f,90.0f));
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->SocketOffset = FVector(0.0f, 60.0f, 0.0f);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 10.0f;
	SpringArm->CameraRotationLagSpeed = 10.0f;

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->MaxWalkSpeed = 800.0f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 120.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->BrakingDecelerationWalking = 300.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;

	GetCharacterMovement()->bRunPhysicsWithNoController = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	InputSetting();

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	
	static ConstructorHelpers::FClassFinder<UAnimInstance>HORSE_ANIM(TEXT("/Game/00_Character/Ride/R_Horse/RH_Animation/RHA_BP/RHAB_AnimBlueprint.RHAB_AnimBlueprint_C"));
	if (HORSE_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(HORSE_ANIM.Class);
	}

	Tags.Add("Ride");
}

void ARide::InputSetting()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>PR_Context(TEXT("/Game/00_Character/C_Input/R_BasicInput.R_BasicInput"));
	if (PR_Context.Succeeded())
	{
		DefaultContext = PR_Context.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Move(TEXT("/Game/00_Character/C_Input/C_Move.C_Move"));
	if (IP_Move.Succeeded())
	{
		MoveAction = IP_Move.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Look(TEXT("/Game/00_Character/C_Input/C_Look.C_Look"));
	if (IP_Look.Succeeded())
	{
		LookAction = IP_Look.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_MoveSpeedToggle(TEXT("/Game/00_Character/C_Input/C_MoveSpeedToggle.C_MoveSpeedToggle"));
	if (IP_MoveSpeedToggle.Succeeded())
	{
		MoveSpeedToggleAction = IP_MoveSpeedToggle.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_DisMount(TEXT("/Game/00_Character/C_Input/C_SpawnRide.C_SpawnRide"));
	if (IP_DisMount.Succeeded())
	{
		DisMountAction = IP_DisMount.Object;
	}
}

// Called when the game starts or when spawned
void ARide::BeginPlay()
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
void ARide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDismount)
	{
		AddMovementInput(GetActorForwardVector(), 1.0f, true);
		Direction = FMath::FInterpTo(Direction, 0.0f, DeltaTime, 5.0f);
	}


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
		FVector NormalDirection = (DestDir - StartDir).GetSafeNormal();
		InteractWidget->SetWorldRotation(NormalDirection.Rotation());

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
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARide::Look);
		EnhancedInputComponent->BindAction(MoveSpeedToggleAction, ETriggerEvent::Triggered, this, &ARide::MoveSpeedToggle);
		EnhancedInputComponent->BindAction(DisMountAction, ETriggerEvent::Triggered, this, &ARide::DisMount);
	}
}

void ARide::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RiderTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARide::TriggerBegin);
	RiderTrigger->OnComponentEndOverlap.AddDynamic(this, &ARide::TriggerEnd);
}

float ARide::GetDirection()
{
	return Direction;
}

void ARide::Move(const FInputActionValue& value)
{
	FVector2D DirectionValue = value.Get<FVector2D>();
	//MovementInputValue = value;
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	FVector2D MovementScale = DirectionValue;
	MovementScale.Normalize();

	//GetCharacterMovement()->GetLastInputVector();

	FVector MovementDirection = GetActorForwardVector();
	FVector LastInputDirection = (UKismetMathLibrary::GetForwardVector(YawRotation) * MovementScale.Y) + (UKismetMathLibrary::GetRightVector(YawRotation) * MovementScale.X);

	float DotProductDirection = FVector::DotProduct(MovementDirection, LastInputDirection);
	float DotProductRadian = FMath::Acos(DotProductDirection);
	float DotProductDegree = FMath::RadiansToDegrees(DotProductRadian);

	FVector RotationAxis = FVector::CrossProduct(MovementDirection, LastInputDirection);
	RotationAxis.Normalize();

	Direction = RotationAxis.Z > 0.0f ? DotProductDegree : -1.0f * DotProductDegree;

	if (DotProductDegree > 160.0f)
	{
		return;
	}

	float AngleRadians = DotProductDegree > 5.0f ? FMath::DegreesToRadians(5.0f) : FMath::DegreesToRadians(DotProductDegree);

	FQuat RotationQuat = FQuat(RotationAxis, AngleRadians);
	FVector RotatedVector = RotationQuat.RotateVector(MovementDirection);
	
	AddMovementInput(RotatedVector);

	if (!LastInputDirection.IsNearlyZero())
	{
		FVector DebugStartLocation = GetActorLocation() - FVector(0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		// 디버깅 시작위치
		float DotProduct = FVector::DotProduct(MovementDirection, LastInputDirection);
		// 아크코사인(Arccos)을 이용해 각도 구하기 (라디안)
		float RadianAngle = FMath::Acos(DotProduct);
		// 라디안을 각도로 변환
		float DegreeAngle = FMath::RadiansToDegrees(RadianAngle);


		FText DebugAxisText = UKismetTextLibrary::Conv_FloatToText(DegreeAngle, ERoundingMode::HalfToEven);
		FString DebugAxisString = DebugAxisText.ToString();

	// 디버깅용 길이
		float DebugLineLength = 100.0f;

		// 이동 방향 표시
		DrawDebugDirectionalArrow(
			GetWorld(),
			DebugStartLocation,
			DebugStartLocation + MovementDirection * DebugLineLength,
			50.0f,          // 화살표 크기
			FColor::Green,   // 이동 방향은 녹색
			false,           // 영구 표시 여부 (false: 짧은 시간만 표시)
			0.0f,            // 표시 시간
			0,               // 두께
			2.0f             // 선 두께
		);

		// 입력 방향 표시
		DrawDebugDirectionalArrow(
			GetWorld(),
			DebugStartLocation,
			DebugStartLocation + LastInputDirection * DebugLineLength,
			50.0f,
			FColor::Blue,    // 입력 방향은 파란색
			false,
			0.0f,
			0,
			2.0f
		);

		DrawDebugString(
			GetWorld(),
			DebugStartLocation,
			DebugAxisString,
			0,
			FColor::White,
			0.0f
		);
	}
}

void ARide::Look(const FInputActionValue& value)
{
	const FVector2D LookAxisValue = value.Get<FVector2D>();
	AddControllerPitchInput(LookAxisValue.Y * 0.5f);
	AddControllerYawInput(LookAxisValue.X * -0.5f);
}

void ARide::Stop()
{

}

void ARide::MoveSpeedToggle()
{

}

void ARide::Mount_Implementation(ACharacter* RiderCharacter, FVector InitVelocity)
{
	IRideInterface::Mount_Implementation(RiderCharacter, InitVelocity);

	SpringArm->bEnableCameraLag = false;
	SpringArm->bEnableCameraRotationLag = false;

	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepRelative,
		true
	);

	Rider = RiderCharacter;

	UE_LOG(LogTemp, Warning, TEXT("InitSpeed = %f"), InitVelocity.Length());

	Rider->AttachToComponent(RiderLocation, AttachmentRules);

	FTransform SpringArmTransform = IViewDataInterface::Execute_GetSpringArmTransform(Rider);
	SpringArm->SetWorldLocation(SpringArmTransform.GetLocation());
	SpringArm->SetWorldRotation(SpringArmTransform.GetRotation().Rotator());

	FRotator InitControllerRotator = IViewDataInterface::Execute_GetControllerRotation(Rider);

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(this);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetControlRotation(InitControllerRotator);

	GetCharacterMovement()->Velocity = InitVelocity;

	CanDismount = false;

	GetWorldTimerManager().SetTimer(CameraSettingTimerHandle, this, &ARide::CameraSettingTimer, 0.01f, true);
}

void ARide::DisMount()
{
	TryDisMount();
}

bool ARide::TryDisMount()
{
	if (!Rider || !RiderGetDownLoc || !CanDismount)
		return false;

	if (Rider->GetClass()->ImplementsInterface(UPlayerInterface::StaticClass()))
	{
		LastSpeed = GetVelocity();

		IPlayerInterface::Execute_DespawnRide(Rider, GetVelocity());

		bDismount = true;
		Rider = nullptr;
	}

	return true;
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

void ARide::CameraSettingTimer()
{
	float CurrentLength = SpringArm->TargetArmLength;
	float NewLength = FMath::FInterpTo(CurrentLength, 300.0f, GetWorld()->GetDeltaSeconds(), 1.0f);

	SpringArm->TargetArmLength = NewLength;

	if (FMath::IsNearlyEqual(NewLength, 300.0f, 1.0f))
	{
		SpringArm->TargetArmLength = 300.0f;
		GetWorld()->GetTimerManager().ClearTimer(CameraSettingTimerHandle);
		SpringArm->bEnableCameraLag = true;
		SpringArm->bEnableCameraRotationLag = true;
	}
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
	return RiderGetDownLoc;
}

FTransform ARide::GetCameraTransform_Implementation()
{
	return Camera->GetComponentTransform();
}

FTransform ARide::GetSpringArmTransform_Implementation()
{
	return SpringArm->GetComponentTransform();;
}

float ARide::GetTargetArmLength_Implementation()
{
	return SpringArm->TargetArmLength;
}

FRotator ARide::GetControllerRotation_Implementation()
{
	return GetController()->GetControlRotation();
}

float ARide::GetRideSpeed_Implementation()
{ 

	return GetVelocity().Length();;
}

float ARide::GetRideDirection_Implementation()
{
	return GetDirection();
}

bool ARide::GetMountDir_Implementation()
{
	return MountRight;
}

FTransform ARide::GetMountTransform_Implementation()
{
	return RiderLocation->GetComponentTransform();
}