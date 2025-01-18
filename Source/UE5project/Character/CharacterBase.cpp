// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "../Function/PBDefaultWidget.h"
#include "Warrior/CharacterBaseAnimInstance.h"
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
	ClimbComponent = CreateDefaultSubobject<UClimbComponent>(TEXT("ClimbComponent"));
	ClimbComponent->bAutoActivate = true;

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

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Walk(TEXT("/Game/Character/C_Input/C_Walk.C_Walk"));
	if (IP_Walk.Succeeded())
	{
		WalkAction = IP_Walk.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Sprint(TEXT("/Game/Character/C_Input/C_Sprint.C_Sprint"));
	if (IP_Sprint.Succeeded())
	{
		SprintAction = IP_Sprint.Object;
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

	CanMovementInput = true;
	CurGroundStance = EGroundStance::Jog;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 800.0f;

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

	/*
	if (CurrentState == ECharacterState::Ladder)
	{
		switch (CurLadderStance)
		{
		case ELadderStance::Idle_OneStep:
		{
			UE_LOG(LogTemp, Error, TEXT("Idle_OneStep"));
			break;
		}
		case ELadderStance::Idle:
		{
			UE_LOG(LogTemp, Error, TEXT("Idle"));
			break;
		}
		case ELadderStance::ClimbUp_OneStep:
		{
			UE_LOG(LogTemp, Error, TEXT("ClimbUp_OneStep"));
			break;
		}
		case ELadderStance::ClimbUp:
		{
			UE_LOG(LogTemp, Error, TEXT("ClimbUp"));
			break;
		}
		case ELadderStance::ClimbDown_OneStep:
		{
			UE_LOG(LogTemp, Error, TEXT("ClimbDown_OneStep"));
			break;
		}
		case ELadderStance::ClimbDown:
		{
			UE_LOG(LogTemp, Error, TEXT("ClimbDown"));
			break;
		}
		case ELadderStance::Enter_From_Bottom:
		{
			UE_LOG(LogTemp, Error, TEXT("Enter_From_Bottom"));
			break;
		}
		case ELadderStance::Enter_From_Top:
		{
			UE_LOG(LogTemp, Error, TEXT("Enter_From_Top"));
			break;
		}
		}
	}
	*/
	//UE_LOG(LogTemp, Error, TEXT("%f"), GetCharacterMovement()->MaxWalkSpeed);

	if (CurGroundStance == EGroundStance::Sprint)
	{
		GetCharacterMovement()->BrakingDecelerationWalking = FMath::FInterpConstantTo(GetCharacterMovement()->BrakingDecelerationWalking, 1300.0f, DeltaTime, 1000.0f);
	}
	else
	{
		GetCharacterMovement()->BrakingDecelerationWalking = FMath::FInterpConstantTo(GetCharacterMovement()->BrakingDecelerationWalking, 800.0f, DeltaTime, 1000.0f);
	}
	
	FVector LastInputDirection = GetLastMovementInputVector().GetSafeNormal();
	if (!LastInputDirection.IsNearlyZero())
	{
		FVector MovementDirection = GetVelocity().GetSafeNormal();
			//GetActorForwardVector();
		// 입력 방향 (Movement Input)
		// 캐릭터 위치
		FVector DebugStartLocation = GetActorLocation() - FVector(0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		// 디버깅 시작위치
		float DotProduct = FVector::DotProduct(MovementDirection, LastInputDirection);
		// 아크코사인(Arccos)을 이용해 각도 구하기 (라디안)
		float RadianAngle = FMath::Acos(DotProduct);
		// 라디안을 각도로 변환
		float DegreeAngle = FMath::RadiansToDegrees(RadianAngle);


		FText DebugAxisText = UKismetTextLibrary::Conv_FloatToText(DegreeAngle, ERoundingMode::HalfToEven);
		FString DebugAxisString = DebugAxisText.ToString();
		//FString::Append(DebugAxisText);

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
	
	/*
	TimeSinceLastDebugUpdate += DeltaTime;
	if (TimeSinceLastDebugUpdate >= DebugUpdateInterval)
	{
		

		TimeSinceLastDebugUpdate = 0.0f;
	}
	*/
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
	//GetCharacterMovement()->bOrientRotationToMovement = true;
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
		//EnhancedInputComponent->BindAction(SwitchStanceAction, ETriggerEvent::Triggered, this, &ACharacterBase::SwitchStance);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ACharacterBase::Dodge);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACharacterBase::Interact);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ACharacterBase::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ACharacterBase::SprintEnd);

		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Started, this, &ACharacterBase::Walk);
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Triggered, this, &ACharacterBase::WalkEnd);
	}
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterBaseAnim = Cast<UCharacterBaseAnimInstance>(GetMesh()->GetAnimInstance());

	DamageSystem->OnDeath.BindUFunction(this, FName("Death"));
}

/* Input Action */
void ACharacterBase::Move(const FInputActionValue& value)
{
	if (!CanMovementInput)
		return;

	const FVector2D DirectionValue = value.Get<FVector2D>();
	//const FVector forward = GetActorForwardVector();
	//const FVector right = GetActorRightVector();
	
	switch (CurrentState)
	{
	case ECharacterState::Ground:
	{
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

		FVector2D MovementScale = DirectionValue;
		MovementScale.Normalize();

		//if (IsDodge)
			//return;
		if (!IsLocomotion)
			return;

		//AddMovementInput(forward, DirectionValue.Y);
		//AddMovementInput(right, DirectionValue.X);
		AddMovementInput(UKismetMathLibrary::GetForwardVector(YawRotation), MovementScale.Y);
		AddMovementInput(UKismetMathLibrary::GetRightVector(YawRotation), MovementScale.X);
		break;
	}
	case ECharacterState::Ladder:
	{
		IsMovementInput = FMath::IsNearlyZero(DirectionValue.Y) ? false : true;
		if (IsMovementInput)
		{
			IsClimbUp = (DirectionValue.Y > 0.0f) ? true : false;
			switch (CurLadderStance) {
			case ELadderStance::Idle:
			{
				if (IsClimbUp)
				{
					if (ClimbComponent->FindGripNeighborUp(Grip1D_Hand_L, 1) == nullptr)
					{
						// Exit From Top
						if (ClimbComponent->FindGripNeighborUp(Grip1D_Hand_R, 1) == nullptr)
						{
							ClimbDistance = FMath::Abs((ClimbComponent->GetInitTopPosition().GetValue().GetLocation().Z + GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) - GetActorLocation().Z);
							Grip1D_Hand_R = Grip1D_Hand_L;
							Grip1D_Foot_R = ClimbComponent->FindGripNeighborUp(Grip1D_Foot_R, 1);
							Grip1D_Foot_L = ClimbComponent->FindGripNeighborUp(Grip1D_Foot_L, 1);
						}
						else
						{
							ClimbDistance = Grip1D_Foot_L->NeighborUp.Distance + Grip1D_Foot_R->NeighborUp.Distance;
							Grip1D_Hand_R = ClimbComponent->FindGripNeighborUp(Grip1D_Hand_R, 1);
							Grip1D_Hand_L = Grip1D_Hand_R;
							Grip1D_Foot_R = ClimbComponent->FindGripNeighborUp(Grip1D_Foot_R, 1);
							Grip1D_Foot_L = ClimbComponent->FindGripNeighborUp(Grip1D_Foot_L, 1);
						}

						//ClimbDistance -= 140.0f;
						GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::ProbeOnly);
						CurLadderStance = ELadderStance::Exit_From_Top;
						CurrentState = ECharacterState::LadderToGround;
						//OnExitTopLadder.ExecuteIfBound();
							
					}
					else
					{
						ClimbDistance = ((Grip1D_Hand_R->NeighborUp.Distance + Grip1D_Hand_L->NeighborUp.Distance + Grip1D_Foot_L->NeighborUp.Distance + Grip1D_Foot_R->NeighborUp.Distance) / 2.0f);
						Grip1D_Hand_R = ClimbComponent->FindGripNeighborUp(Grip1D_Hand_R, 1);
						Grip1D_Hand_L = ClimbComponent->FindGripNeighborUp(Grip1D_Hand_L, 1);
						Grip1D_Foot_R = ClimbComponent->FindGripNeighborUp(Grip1D_Foot_R, 1);
						Grip1D_Foot_L = ClimbComponent->FindGripNeighborUp(Grip1D_Foot_L, 1);
						CurLadderStance = ELadderStance::ClimbUp;
					}
				}
				else
				{
					if (ClimbComponent->FindGripNeighborDown(Grip1D_Foot_R, 1) == nullptr)
					{
						// Idle -> Idle_OneStep
						ClimbDistance = Grip1D_Hand_R->NeighborDown.Distance;
						Grip1D_Hand_L = ClimbComponent->FindGripNeighborDown(Grip1D_Hand_L, 1);
						CurLadderStance = ELadderStance::ClimbDown_OneStep;
					}
					else
					{
						ClimbDistance = ((Grip1D_Hand_R->NeighborDown.Distance + Grip1D_Hand_L->NeighborDown.Distance + Grip1D_Foot_L->NeighborDown.Distance + Grip1D_Foot_R->NeighborDown.Distance) / 2.0f);
						Grip1D_Hand_R = ClimbComponent->FindGripNeighborDown(Grip1D_Hand_R, 1);
						Grip1D_Hand_L = ClimbComponent->FindGripNeighborDown(Grip1D_Hand_L, 1);
						Grip1D_Foot_R = ClimbComponent->FindGripNeighborDown(Grip1D_Foot_R, 1);
						Grip1D_Foot_L = ClimbComponent->FindGripNeighborDown(Grip1D_Foot_L, 1);
						CurLadderStance = ELadderStance::ClimbDown;
					}
					ClimbDistance *= -1.0f;
				}
				break;
			}
			case ELadderStance::Idle_OneStep:
			{
				if (IsClimbUp)
				{
					ClimbDistance = ((Grip1D_Hand_R->NeighborUp.Distance + Grip1D_Foot_R->NeighborUp.Distance) / 2.0f);
					Grip1D_Hand_L = ClimbComponent->FindGripNeighborUp(Grip1D_Hand_L, 1);
					Grip1D_Foot_L = ClimbComponent->FindGripNeighborUp(Grip1D_Foot_R);
					CurLadderStance = ELadderStance::ClimbUp_OneStep;
				}
				else
				{
					// Exit from Bottom
					GetCharacterMovement()->SetMovementMode(MOVE_Walking);
					//GetController()->SetIgnoreMoveInput(true);
					CurLadderStance = ELadderStance::Exit_From_Bottom;
					CurrentState = ECharacterState::LadderToGround;
				}
				break;	
			}
			default:
			{

			}
			}
			//CanMovementInput = false;
		}
		break;
	}
	default:
	{
		break;
	}
	}
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
	switch (CurrentState)
	{
	case ECharacterState::Ground:
	{
		IsMovementInput = true;
		break;
	}
	case ECharacterState::Ladder:
	{
		break;
	}
	}
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

void ACharacterBase::Walk()
{
	CurGroundStance = EGroundStance::Walk;
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

void ACharacterBase::WalkEnd()
{
	CurGroundStance = EGroundStance::Jog;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

void ACharacterBase::Sprint()
{
	CurGroundStance = EGroundStance::Sprint;
	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
	//GetCharacterMovement()->BrakingDecelerationWalking = 1300.0f;
	//UE_LOG(LogTemp, Error, TEXT("TOGGLING"));
	//IsRun = !IsRun;
	//TargetSpeed = IsRun ? 700.0f : 300.0f;

	/*
	switch (CurGroundStance)
	{
	case EGroundStance::Walk:
	{
		CurGroundStance = EGroundStance::Jog;
		GetCharacterMovement()->MaxWalkSpeed = 400.0f;
		break;
	}
	case EGroundStance::Jog:
	{
		CurGroundStance = EGroundStance::Walk;
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		break;
	}
	}
	*/
}

void ACharacterBase::SprintEnd()
{
	CurGroundStance = EGroundStance::Jog;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	//GetCharacterMovement()->BrakingDecelerationWalking = 800.0f;
}

void ACharacterBase::SetCanMovementInput(bool CanMove)
{
	CanMovementInput = CanMove;
}

void ACharacterBase::Interact()
{
	if (InteractActorList.IsEmpty())
		return;

	if (IsInteraction)
	{
		//bUseControllerRotationYaw = true;
		//CurrentState = ECharacterState::Ground;
		GetController()->SetIgnoreMoveInput(false);
		GetController()->StopMovement();
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		IsInteraction = false;
		IsClimb = false;
		return;
	}
	else
	{
		AActor* InteractActor = nullptr;

		FVector CharLocation = GetActorLocation();
		FVector ForwardVector = GetActorForwardVector();

		float CurDotVal = 0.0f;
		float Threshold = 0.5f;
		for (auto& Act : InteractActorList)
		{
			FVector ActorLocation = Act->GetActorLocation();

			FVector ToTargetDir = ActorLocation - CharLocation;
			ToTargetDir.Z = 0.0f;
			ToTargetDir.Normalize();

			float DotToTarget = FVector::DotProduct(ForwardVector, ToTargetDir);
			float RadianToTarget = FMath::Acos(DotToTarget);
			float AngleDegrees = FMath::RadiansToDegrees(RadianToTarget);

			if (AngleDegrees > -60.0f && AngleDegrees < 60.0f)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Set Interact Actor"));
				//CurDotVal = DotToTarget;
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
				//UE_LOG(LogTemp, Warning, TEXT("Interact To NPC"));
				IPBInteractInterface::Execute_Interact(InteractActor, this);
				//GetController()->SetIgnoreMoveInput(true);
			}
			else if (InteractActor->ActorHasTag("Ride"))
			{
				IsInteraction = true;
				bUseControllerRotationYaw = false;
				IPBInteractInterface::Execute_RegisterInteractActor(InteractActor, this);
				MovetoInteractPos(InteractActor);
			}
			else if (InteractActor->ActorHasTag("Climbable"))
			{
				IsInteraction = true;
				//GetController()->SetIgnoreMoveInput(true);
				ClimbComponent->RegisterClimbObject(InteractActor);
				if (ClimbComponent->CheckGripListValid())
				{
					//UE_LOG(LogTemp, Warning, TEXT("GripList is Valid"));
					ClimbComponent->SetGrip1DRelation(15.0f, 50.0f);
				}
				//UE_LOG(LogTemp, Warning, TEXT("Interact With Climbable Actor"));
				MovetoInteractPos(InteractActor);
			}
		}
	}
}

void ACharacterBase::MovetoInteractPos(AActor* InteractActor)
{
	USceneComponent* Target = IPBInteractInterface::Execute_GetEnterInteractLocation(InteractActor, this);
	FVector DestLoc = Target->GetComponentLocation();
	FRotator DestRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DestLoc);
	SetActorRotation(FRotator(0.0f, DestRot.Yaw, 0.0f));
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), DestLoc);
	
	InteractTimerDelegate.BindUObject(this, &ACharacterBase::InteractTimer, Target, InteractActor);
	GetWorldTimerManager().SetTimer(InteractTimerHandle, InteractTimerDelegate, 0.1f, true);
}

void ACharacterBase::InteractTimer(USceneComponent* Target, AActor* InteractActor)
{
	FVector2D CharLoc = FVector2D(GetActorLocation().X, GetActorLocation().Y);
	FVector2D TargetLoc = FVector2D(Target->GetComponentLocation().X, Target->GetComponentLocation().Y);
	float Distance = FVector2D::Distance(CharLoc, TargetLoc);
	if (Distance < 50.0f)
	{
		GetController()->StopMovement();
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		LatentInfo.Linkage = 0;
		LatentInfo.UUID = __LINE__;

		if (InteractActor->ActorHasTag("Ride"))
		{
			DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			Ride = Cast<ACharacter>(InteractActor);
			IsRide = true;
			//GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		}
		else if (InteractActor->ActorHasTag("Ladder"))
		{
			CanMovementInput = false;
			LatentInfo.ExecutionFunction = FName("OnMoveEndToLadder");

			if (Target->ComponentHasTag("Bottom"))
			{
				CurLadderStance = ELadderStance::Enter_From_Bottom;
				float ComparisonHeight = GetActorLocation().Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
				Grip1D_Foot_R = ClimbComponent->FindGripByHeight(40.0f, ComparisonHeight);
				Grip1D_Hand_L = ClimbComponent->FindGripByHeight(120.0f, ComparisonHeight);
				Grip1D_Hand_R = Grip1D_Hand_L != nullptr ? ClimbComponent->FindGripNeighborUp(Grip1D_Hand_L) : nullptr;

				UE_LOG(LogTemp, Warning, TEXT("Bottom"));
			}
			else
			{
				CurLadderStance = ELadderStance::Enter_From_Top;
				//OnEnterTopLadder.ExecuteIfBound();
				ClimbDistance = ClimbComponent->GetEnterTopPosition().GetValue().GetLocation().Z - GetActorLocation().Z;
				Grip1D_Hand_L = ClimbComponent->GetHighestGrip1D();
				Grip1D_Hand_R = ClimbComponent->GetHighestGrip1D();
				Grip1D_Foot_L = ClimbComponent->FindGripNeighborDown(Grip1D_Hand_R, 1);
				Grip1D_Foot_R = ClimbComponent->FindGripNeighborDown(Grip1D_Foot_L);
				UE_LOG(LogTemp, Warning, TEXT("ClimbDistance = %f"), ClimbDistance);
				//ClimbComponent->
				//Grip1D_Foot_R = 
				UE_LOG(LogTemp, Warning, TEXT("Top"));
			}
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *ELadderStanceLog);
		}

		UKismetSystemLibrary::MoveComponentTo(
			GetCapsuleComponent(),
			FVector(Target->GetComponentLocation().X, Target->GetComponentLocation().Y, Target->GetComponentLocation().Z + 92.0f),
			Target->GetComponentRotation(),
			false,
			false,
			0.1f,
			false,
			EMoveComponentAction::Type::Move,
			LatentInfo
		);

		//SetActorLocation(Target->GetComponentLocation());
		//SetActorRotation(Target->GetComponentRotation());
		
		GetWorldTimerManager().ClearTimer(InteractTimerHandle);
	}
}

void ACharacterBase::OnMoveEndToLadder()
{
	UE_LOG(LogTemp, Warning, TEXT("Move completed!"));
	CurrentState = ECharacterState::GroundToLadder;
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::ProbeOnly);
	//CanMovementInput = false;
	IsClimb = true;

	/*
	if (CurLadderStance == ELadderStance::Enter_From_Top)
	{
		UKismetSystemLibrary::MoveComponentTo(
			GetCapsuleComponent(),
			ClimbComponent->getex,
			Target->GetComponentRotation(),
			false,
			false,
			0.1f,
			false,
			EMoveComponentAction::Type::Move,
			LatentInfo
		);
	}
	*/
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

ECharacterState ACharacterBase::GetCurrentState()
{
	return CurrentState;
}

ELadderStance ACharacterBase::GetCurLadderStance()
{
	return CurLadderStance;
}

EGroundStance ACharacterBase::GetCurGroundStance()
{
	return CurGroundStance;
}

float ACharacterBase::GetClimbDistance()
{
	return ClimbDistance;
}

UClimbComponent* ACharacterBase::GetClimbComponent()
{
	return ClimbComponent;
}

void ACharacterBase::DecideLadderStance()
{
	//UE_LOG(LogTemp, Warning, TEXT("DecideLadderStance"));
	switch (CurLadderStance)
	{
	case ELadderStance::ClimbUp_OneStep:
	{
		CurLadderStance = ELadderStance::Idle;
		//UE_LOG(LogTemp, Warning, TEXT("ClimbUp_Onestep to Idle"));
		break;
	}
	case ELadderStance::ClimbUp:
	{
		CurLadderStance = ELadderStance::Idle;
		//UE_LOG(LogTemp, Warning, TEXT("ClimbUp to Idle"));
		break;
	}
	case ELadderStance::ClimbDown_OneStep:
	{
		CurLadderStance = ELadderStance::Idle_OneStep;
		break;
	}
	case ELadderStance::ClimbDown:
	{
		CurLadderStance = ELadderStance::Idle;
		break;
	}
	case ELadderStance::Enter_From_Bottom:
	{
		CurLadderStance = ELadderStance::Idle_OneStep;
		break;
	}
	case ELadderStance::Enter_From_Top:
	{
		CurLadderStance = ELadderStance::Idle;
		break;
	}
	case ELadderStance::Exit_From_Top:
	{
		CurLadderStance = ELadderStance::Idle;
		break;
	}
	case ELadderStance::Exit_From_Bottom:
	{
		CurLadderStance = ELadderStance::Idle;
		break;
	}
	default:
		break;
	}

	//GetController()->SetIgnoreMoveInput(false);
}

TOptional<FVector> ACharacterBase::GetBoneTargetLoc(EBodyType BoneType)
{
	AActor* LadderObject = ClimbComponent->GetClimbObject();

	if (LadderObject == nullptr)
		return TOptional<FVector>();

	FVector LadderRight = LadderObject->GetActorForwardVector();

	switch (BoneType)
	{
	case EBodyType::Hand_R:
	{
		if(Grip1D_Hand_R == nullptr)
			return TOptional<FVector>();

		//UE_LOG(LogTemp, Warning, TEXT("LadderRight Value = X : %f, Y : %f, Z : %f"), LadderRight.X, LadderRight.Y, LadderRight.Z);
		//UE_LOG(LogTemp, Warning, TEXT("HandRightGrip Value = X : %f, Y : %f, Z : %f"), Grip1D_Hand_R->Position.X, Grip1D_Hand_R->Position.Y, Grip1D_Hand_R->Position.Z);

		return Grip1D_Hand_R->Position + (LadderRight * 20.0f);
	}
	case EBodyType::Hand_L:
	{
		if (Grip1D_Hand_L == nullptr)
			return TOptional<FVector>();

		return Grip1D_Hand_L->Position - (LadderRight * 20.0f);
	}
	case EBodyType::Foot_R:
	{
		if (Grip1D_Foot_R == nullptr)
			return TOptional<FVector>();

		FVector AdjustGrip1D_Foot_R = Grip1D_Foot_R->Position + (LadderRight * 20.0f);
		//AdjustGrip1D_Foot_R.Z += 20.0f;
		return AdjustGrip1D_Foot_R;
	}
	case EBodyType::Foot_L:
	{
		if (Grip1D_Foot_L == nullptr)
			return TOptional<FVector>();

		FVector AdjustGrip1D_Foot_L = Grip1D_Foot_L->Position - (LadderRight * 20.0f);
		//AdjustGrip1D_Foot_L.Z += 5.0f;
		return AdjustGrip1D_Foot_L;
	}
	default:
		return TOptional<FVector>();
	}
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

void ACharacterBase::RegisterInteractableActor_Implementation(AActor* Interactable)
{
	IPBPlayerInterface::RegisterInteractableActor_Implementation(Interactable);

	InteractActorList.Add(Interactable);
}

void ACharacterBase::DeRegisterInteractableActor_Implementation(AActor* Interactable)
{
	IPBPlayerInterface::DeRegisterInteractableActor_Implementation(Interactable);

	InteractActorList.Remove(Interactable);
}

void ACharacterBase::EndInteraction_Implementation(AActor* Interactable)
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

TOptional<FVector> ACharacterBase::GetCharBoneLocation(FName BoneName)
{
	return GetMesh()->DoesSocketExist(BoneName) ? TOptional<FVector>(GetMesh()->GetSocketLocation(BoneName)) : TOptional<FVector>();
}

void ACharacterBase::DisMountEnd()
{
	USceneComponent* GetDownLoc = IPBInteractInterface::Execute_GetLeftInteractLocation(Ride);
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