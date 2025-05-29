// Fill out your copyright notice in the Description page of Project Settings.

// 기본 헤더
#include "CharacterBase.h"

// 이동
#include "GameFramework/CharacterMovementComponent.h"

// 콜리전
#include "Components/CapsuleComponent.h"

// 카메라
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// 입력
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "InputMappingContext.h"

// Kismet 유틸리티
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// UI
#include "../Function/DefaultWidget.h"
#include "Blueprint/UserWidget.h"

// 애니메이션
#include "Warrior/CharacterBaseAnimInstance.h"

// 참조할 액터
#include "PlayerRide.h"

// 인터페이스
#include "../Function/Interact/InteractInterface.h" ///삭제 예정
#include "../Function/Interact/Ride/RideInterface.h"

// 유저 컴포넌트
#include "../Function/CharacterStatusComponent.h"
#include "../Function/Combat/StatComponent.h"
#include "../Function/Combat/AttackComponent.h"
#include "../Function/Combat/HitReactionComponent.h"
#include "../Function/Interact/ClimbComponent.h"
#include "../Function/Interact/InteractComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = GetCapsuleComponent();

	CharacterStatusComponent = CreateDefaultSubobject<UCharacterStatusComponent>(TEXT("CharacterStatusComponent"));
	CharacterStatusComponent->bAutoActivate = true;

	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	StatComponent->bAutoActivate = true;

	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComponent"));
	AttackComponent->bAutoActivate = true;

	HitReactionComponent = CreateDefaultSubobject<UHitReactionComponent>(TEXT("HitReactionComponent"));
	HitReactionComponent->bAutoActivate = true;

	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	InteractComponent->bAutoActivate = true;

	ClimbComponent = CreateDefaultSubobject<UClimbComponent>(TEXT("ClimbComponent"));
	ClimbComponent->bAutoActivate = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Character"));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	//GetMesh()->SetOwnerNoSee(true);

	static ConstructorHelpers::FObjectFinder<UInputMappingContext>PBC_Context(TEXT("/Game/00_Character/C_Input/C_BasicInput.C_BasicInput"));
	if (PBC_Context.Succeeded())
	{
		DefaultContext = PBC_Context.Object;
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

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Dodge(TEXT("/Game/00_Character/C_Input/C_Dodge.C_Dodge"));
	if (IP_Dodge.Succeeded())
	{
		DodgeAction = IP_Dodge.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Walk(TEXT("/Game/00_Character/C_Input/C_Walk.C_Walk"));
	if (IP_Walk.Succeeded())
	{
		WalkAction = IP_Walk.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Sprint(TEXT("/Game/00_Character/C_Input/C_Sprint.C_Sprint"));
	if (IP_Sprint.Succeeded())
	{
		SprintAction = IP_Sprint.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_CheckMoveInput(TEXT("/Game/00_Character/C_Input/C_CheckMoveInput.C_CheckMoveInput"));
	if (IP_CheckMoveInput.Succeeded())
	{
		CheckMoveAction = IP_CheckMoveInput.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Jump(TEXT("/Game/00_Character/C_Input/C_Jump.C_Jump"));
	if (IP_Jump.Succeeded())
	{
		JumpAction = IP_Jump.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Attack(TEXT("/Game/00_Character/C_Input/C_Attack.C_Attack"));
	if (IP_Attack.Succeeded())
	{
		AttackAction = IP_Attack.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_HeavyAttack(TEXT("/Game/00_Character/C_Input/C_HeavyAttack.C_HeavyAttack"));
	if (IP_HeavyAttack.Succeeded())
	{
		HeavyAttackAction = IP_HeavyAttack.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Block(TEXT("/Game/00_Character/C_Input/C_Block.C_Block"));
	if (IP_Block.Succeeded())
	{
		BlockAction = IP_Block.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Interact(TEXT("/Game/00_Character/C_Input/C_Interact.C_Interact"));
	if (IP_Interact.Succeeded())
	{
		InteractAction = IP_Interact.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_SpawnRide(TEXT("/Game/00_Character/C_Input/C_SpawnRide.C_SpawnRide"));
	if (IP_SpawnRide.Succeeded())
	{
		SpawnRideAction = IP_SpawnRide.Object;
	}

	static ConstructorHelpers::FClassFinder<UDefaultWidget> DEFAULTWIDGET(TEXT("/Game/00_Character/C_Source/DefaultWidget_BP"));
	if (!ensure(DEFAULTWIDGET.Class != nullptr)) return;

	DefaultWidgetClass = DEFAULTWIDGET.Class;

	static ConstructorHelpers::FObjectFinder<UDataTable> HitReactionDT_Asset(TEXT("DataTable'/Game/00_Character/C_Source/FallenKnightHitReaction_DT.FallenKnightHitReaction_DT'"));
	if (HitReactionDT_Asset.Succeeded())
	{
		HitReactionComponent->SetHitReactionDT(HitReactionDT_Asset.Object);
	}

	CanMovementInput = true;
	CurGroundStance = EGroundStance::Jog;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 800.0f;

	ClimbComponent->SetMinGripInterval(MinGripInterval);
	ClimbComponent->SetMaxGripInterval(MaxGripInterval);
	ClimbComponent->SetMinFirstGripHeight(MinFirstGripHeight);

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
		DefaultWidget = CreateWidget<UDefaultWidget>(PlayerController, DefaultWidgetClass);
	}

	StatComponent->OnDeath.BindUObject(this, &ACharacterBase::Death);
	StatComponent->InitializeStats();

	InitSpringArmLocation = SpringArm->GetRelativeLocation();
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

		EnhancedInputComponent->BindAction(SpawnRideAction, ETriggerEvent::Triggered, this, &ACharacterBase::SpawnRide);
	}
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterBaseAnim = Cast<UCharacterBaseAnimInstance>(GetMesh()->GetAnimInstance());

	InteractComponent->OnArrivedInteractionPoint.BindUObject(this, &ACharacterBase::HandleArrivedInteractionPoint);

	CharacterBaseAnim->OnEnterLocomotion.BindUObject(this, &ACharacterBase::EnterLocomotion);

	CharacterBaseAnim->OnLeftLocomotion.BindUObject(this, &ACharacterBase::LeftLocomotion);

	CharacterBaseAnim->OnDodgeEnd.AddLambda([this]()->void {
		CanDodge = true;
		//IsDodge = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		//FallenKnightAnim->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
		});

	CharacterBaseAnim->OnDodgeStart.AddLambda([this]()->void {
		//IsDodge = true;
		IsRoll = false;
		});


	CharacterBaseAnim->OnEnterWalkState.AddLambda([this]()->void {
		CurrentState = ECharacterState::Ground;
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CanRide = true;
		});

	CharacterBaseAnim->OnEnterLadderState.AddLambda([this]()->void {
		CurrentState = ECharacterState::Ladder;
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		});

	CharacterBaseAnim->OnClimbEnd.AddUObject(this, &ACharacterBase::DecideLadderStance);
	CharacterBaseAnim->OnMountEnd.AddUObject(this, &ACharacterBase::MountEnd);
	CharacterBaseAnim->OnDisMountEnd.AddUObject(this, &ACharacterBase::DisMountEnd);
}

/* Input Action */
void ACharacterBase::Move(const FInputActionValue& value)
{
	IsMovementInput = true;

	if (!CanMovementInput)
		return;

	const FVector2D DirectionValue = value.Get<FVector2D>();
	
	switch (CurrentState)
	{
	case ECharacterState::Ground:
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		InputVector = FVector(DirectionValue.X, DirectionValue.Y, 0.0f);


		FVector2D MovementScale = DirectionValue;
		MovementScale.Normalize();

		if (!IsLocomotion)
			return;

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
					if (ClimbComponent->GetGripNeighborUp(Grip1D_Hand_L, 2) == nullptr)
					{
						// Exit From Top
						if (ClimbComponent->GetGripNeighborUp(Grip1D_Hand_R, 2) == nullptr)
						{
							
							ClimbDistance = FMath::Abs((ClimbComponent->GetInitTopPosition().GetValue().GetLocation().Z + GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) - GetActorLocation().Z);
							ClimbComponent->SetGripNeighborUp(Grip1D_Hand_R, 1);
							ClimbComponent->SetGripNeighborUp(Grip1D_Foot_R, 2);
							ClimbComponent->SetGripNeighborUp(Grip1D_Foot_L, 2);
						}
						else
						{
							ClimbDistance = Grip1D_Foot_L->NeighborUp.Distance + Grip1D_Foot_R->NeighborUp.Distance;
							ClimbComponent->SetGripNeighborUp(Grip1D_Hand_R, 2);
							ClimbComponent->SetGripNeighborUp(Grip1D_Hand_L, 1);
							ClimbComponent->SetGripNeighborUp(Grip1D_Foot_R, 2);
							ClimbComponent->SetGripNeighborUp(Grip1D_Foot_L, 2);
						}

						//ClimbDistance -= 140.0f;
						GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::ProbeOnly);
						CurLadderStance = ELadderStance::Exit_From_Top;
						//CurrentState = ECharacterState::LadderToGround;
						//OnExitTopLadder.ExecuteIfBound();
							
					}
					else
					{
						ClimbDistance = ((Grip1D_Hand_R->NeighborUp.Distance + Grip1D_Hand_L->NeighborUp.Distance + Grip1D_Foot_L->NeighborUp.Distance + Grip1D_Foot_R->NeighborUp.Distance) / 2.0f);
						ClimbComponent->SetGripNeighborUp(Grip1D_Hand_R, 2);
						ClimbComponent->SetGripNeighborUp(Grip1D_Hand_L, 2);
						ClimbComponent->SetGripNeighborUp(Grip1D_Foot_R, 2);
						ClimbComponent->SetGripNeighborUp(Grip1D_Foot_L, 2);
						CurLadderStance = ELadderStance::ClimbUp;
					}
				}
				else
				{
					if (ClimbComponent->GetGripNeighborDown(Grip1D_Foot_R, 2) == nullptr || Grip1D_Foot_R->Tag.Contains(FName("LowestGrip")))
					{
						// Idle -> Idle_OneStep
						ClimbDistance = Grip1D_Hand_R->NeighborDown.Distance;
						ClimbComponent->SetGripNeighborDown(Grip1D_Hand_L, 2);
						CurLadderStance = ELadderStance::ClimbDown_OneStep;
					}
					else
					{
						ClimbDistance = ((Grip1D_Hand_R->NeighborDown.Distance + Grip1D_Hand_L->NeighborDown.Distance + Grip1D_Foot_L->NeighborDown.Distance + Grip1D_Foot_R->NeighborDown.Distance) / 2.0f);
						ClimbComponent->SetGripNeighborDown(Grip1D_Hand_R, 2);
						ClimbComponent->SetGripNeighborDown(Grip1D_Hand_L, 2);
						ClimbComponent->SetGripNeighborDown(Grip1D_Foot_R, 2);
						ClimbComponent->SetGripNeighborDown(Grip1D_Foot_L, 2);
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
					ClimbComponent->SetGripNeighborUp(Grip1D_Hand_L, 2);
					Grip1D_Foot_L = ClimbComponent->GetGripNeighborUp(Grip1D_Foot_R);
					CurLadderStance = ELadderStance::ClimbUp_OneStep;
				}
				else
				{
					// Exit from Bottom
					GetCharacterMovement()->SetMovementMode(MOVE_Walking);
					CurLadderStance = ELadderStance::Exit_From_Bottom;
				}
				break;	
			}
			default:
			{

			}
			}
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
	AddControllerYawInput(LookAxisValue.X * -0.5f);
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

void ACharacterBase::EnterLocomotion()
{
	IsLocomotion = true;
	//IsAttack = false;
	//CurResponse = HitResponse::None;
	CanDodge = true;
	CanRide = true;
	//UE_LOG(LogTemp, Warning, TEXT("EnterLocomotion"));
}

void ACharacterBase::LeftLocomotion()
{
	IsLocomotion = false;

	CanRide = false;
	UE_LOG(LogTemp, Warning, TEXT("LeftLocomotion"));
}

void ACharacterBase::Dodge()
{
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	FVector ValueX = UKismetMathLibrary::GetForwardVector(YawRotation) * InputVector.Y;
	FVector ValueY = UKismetMathLibrary::GetRightVector(YawRotation) * InputVector.X;
	FVector DirectionVector = ValueY + ValueX;
	FVector InputDegree = GetActorTransform().InverseTransformVectorNoScale(DirectionVector);
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

	float OriginalLength = FMath::Sqrt(OriginalLocX * OriginalLocX + OriginalLocY * OriginalLocY);

	float AdjustFactorX = (OriginalLocX / OriginalLength) * 460.0f;
	float AdjustFactorY = (OriginalLocY / OriginalLength) * 460.0f;


	float TargetLocX = AdjustFactorX - OriginalLocX;
	float TargetLocY = AdjustFactorY - OriginalLocY;;

	DodgeDirection = GetActorForwardVector() * TargetLocY + GetActorRightVector() * TargetLocX;
}

void ACharacterBase::Block()
{
	//IsBlock = true;
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
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

void ACharacterBase::Sprint()
{
	CurGroundStance = EGroundStance::Sprint;
	GetCharacterMovement()->MaxWalkSpeed = 800.0f;
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
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	//GetCharacterMovement()->BrakingDecelerationWalking = 800.0f;
}

float ACharacterBase::GetDirection()
{
	return Direction;
}

void ACharacterBase::SetCanMovementInput(bool CanMove)
{
	CanMovementInput = CanMove;
}

void ACharacterBase::Interact()
{
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
		bool InteractTargetValid = InteractComponent->SetInteractActorByDegree(this, 60.0f);

		if (!InteractTargetValid)
			return;

		GetController()->SetIgnoreMoveInput(true);
		IsInteraction = InteractComponent->MovetoInteractPos();
		
		/*
		if (InteractComponent->GetInteractActor()->ActorHasTag("Ride"))
		{
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(Cast<APawn>(InteractComponent->GetInteractActor()));
			IsInteraction = true;
		}
		else
		{
			IsInteraction = InteractComponent->MovetoInteractPos();
		}
		*/
	}
}

void ACharacterBase::MountEnd()
{
	//IInteractInterface::Execute_Interact(Ride, this);

	FTransform MountTransform = IRideInterface::Execute_GetMountTransform(Ride);
	SetActorLocation(MountTransform.GetLocation());
	SetActorRotation(MountTransform.GetRotation().Rotator());

	if (GetWorldTimerManager().IsTimerActive(MountTimerHandle))
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetWorldTimerManager().ClearTimer(MountTimerHandle);
	}
	CurRideStance = ERideStance::Riding;
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

float ACharacterBase::GetRideSpeed()
{
	if (Ride==nullptr)
		return 0.0f;

	return IRideInterface::Execute_GetRideSpeed(Ride);
}

float ACharacterBase::GetRideDirection()
{
	if (Ride == nullptr)
		return 0.0f;

	return IRideInterface::Execute_GetRideDirection(Ride);
}

FVector ACharacterBase::GetInputDirection()
{
	return DodgeVector;
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

ERideStance ACharacterBase::GetCurRideStance()
{
	return CurRideStance;
}

float ACharacterBase::GetClimbDistance()
{
	return ClimbDistance;
}

UClimbComponent* ACharacterBase::GetClimbComponent()
{
	return ClimbComponent;
}

void ACharacterBase::SetNextGripDown_Implementation(FName BoneName, int32 Count)
{
	if (BoneName == FName("Hand_L"))
	{
		ClimbComponent->SetGripNeighborDown(Grip1D_Hand_L, Count);
	}
	else if (BoneName == FName("Hand_R"))
	{
		ClimbComponent->SetGripNeighborDown(Grip1D_Hand_R, Count);
	}
	else if (BoneName == FName("Foot_L"))
	{
		ClimbComponent->SetGripNeighborDown(Grip1D_Foot_L, Count);
	}
	else if (BoneName == FName("Foot_R"))
	{
		ClimbComponent->SetGripNeighborDown(Grip1D_Foot_R, Count);
	}
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
		CurrentState = ECharacterState::Ground;
		CurLadderStance = ELadderStance::Idle;
		break;
	}
	case ELadderStance::Exit_From_Bottom:
	{
		CurrentState = ECharacterState::Ground;
		CurLadderStance = ELadderStance::Idle;
		break;
	}
	default:
		break;
	}

	//GetController()->SetIgnoreMoveInput(false);
}

TOptional<TTuple<FVector, FVector>> ACharacterBase::GetLadderIKTargetLoc(EBodyType BoneType)
{
	AActor* LadderObject = ClimbComponent->GetClimbObject();

	if (LadderObject == nullptr)
		return TOptional<TTuple<FVector, FVector>>();

	FVector LadderRight = LadderObject->GetActorForwardVector();
	float DirectionScale = 20.0f;

	LadderRight *= DirectionScale;

	switch (BoneType)
	{
	case EBodyType::Hand_R:
	{
		if(Grip1D_Hand_R == nullptr)
			return TOptional<TTuple<FVector, FVector>>();

		if (Grip1D_Hand_R->PrevGrip == nullptr)
			return TTuple<FVector, FVector>(Grip1D_Hand_R->Position + LadderRight, GetMesh()->GetSocketLocation(FName("Hand_R_Init")));

		return TTuple<FVector, FVector>(Grip1D_Hand_R->Position + LadderRight, Grip1D_Hand_R->PrevGrip->Position + LadderRight);
	}
	case EBodyType::Hand_L:
	{
		if (Grip1D_Hand_L == nullptr)
			return TOptional<TTuple<FVector, FVector>>();

		if (Grip1D_Hand_L->PrevGrip == nullptr)
			return TTuple<FVector, FVector>(Grip1D_Hand_L->Position - LadderRight, GetMesh()->GetSocketLocation(FName("Hand_L_Init")));

		return TTuple<FVector, FVector>(Grip1D_Hand_L->Position - LadderRight, Grip1D_Hand_L->PrevGrip->Position - LadderRight);
	}
	case EBodyType::Foot_R:
	{
		if (Grip1D_Foot_R == nullptr)
			return TOptional<TTuple<FVector, FVector>>();

		if (Grip1D_Foot_R->PrevGrip == nullptr)
			return TTuple<FVector, FVector>(Grip1D_Foot_R->Position + LadderRight, GetMesh()->GetSocketLocation(FName("Foot_R_Init")));

		return TTuple<FVector, FVector>(Grip1D_Foot_R->Position + LadderRight, Grip1D_Foot_R->PrevGrip->Position + LadderRight);
	}
	case EBodyType::Foot_L:
	{
		if (Grip1D_Foot_L == nullptr)
			return TOptional<TTuple<FVector, FVector>>();

		if (Grip1D_Foot_L->PrevGrip == nullptr)
			return TTuple<FVector, FVector>(Grip1D_Foot_L->Position - LadderRight, GetMesh()->GetSocketLocation(FName("Foot_L_Init")));

		return TTuple<FVector, FVector>(Grip1D_Foot_L->Position - LadderRight, Grip1D_Foot_L->PrevGrip->Position - LadderRight);
	}
	default:
		return TOptional<TTuple<FVector, FVector>>();
	}
}

TOptional<FVector> ACharacterBase::GetRideIKTargetLoc(EBodyType BoneType)
{
	if (Ride == nullptr)
		return TOptional<FVector>();

	switch (BoneType)
	{
	case EBodyType::Hand_L:
	{
		return Ride->GetMesh()->DoesSocketExist(FName("Reins_Bn_Hand_L"))
			? Ride->GetMesh()->GetSocketLocation(FName("Reins_Bn_Hand_L"))
			: TOptional<FVector>();
	}
	case EBodyType::Hand_R:
	{
		return Ride->GetMesh()->DoesSocketExist(FName("Reins_Bn_Hand_R"))
			? Ride->GetMesh()->GetSocketLocation(FName("Reins_Bn_Hand_R"))
			: TOptional<FVector>();
	}
	case EBodyType::Foot_L:
	{
		return Ride->GetMesh()->DoesSocketExist(FName("SaddleLeftFootPlace"))
			? Ride->GetMesh()->GetSocketLocation(FName("SaddleLeftFootPlace"))
			: TOptional<FVector>();
	}
	case EBodyType::Foot_R:
	{
		return Ride->GetMesh()->DoesSocketExist(FName("SaddleRightFootPlace"))
			? Ride->GetMesh()->GetSocketLocation(FName("SaddleRightFootPlace"))
			: TOptional<FVector>();
	}
	default:
	{
		return TOptional<FVector>();
	}
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
	IPlayerInterface::IsPlayer_Implementation();

	return false;
}

void ACharacterBase::RegisterInteractableActor_Implementation(AActor* Interactable)
{
	IPlayerInterface::RegisterInteractableActor_Implementation(Interactable);

	InteractComponent->AddInteractObject(Interactable);
	//InteractActorList.Add(Interactable);
}

void ACharacterBase::DeRegisterInteractableActor_Implementation(AActor* Interactable)
{
	IPlayerInterface::DeRegisterInteractableActor_Implementation(Interactable);

	InteractComponent->RemoveInteractObject(Interactable);
}

void ACharacterBase::EndInteraction_Implementation(AActor* Interactable)
{
	IPlayerInterface::EndInteraction_Implementation(Interactable);

	if (Interactable->ActorHasTag("Ride"))
	{
		//IsRide = false;

		//GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	}
}

void ACharacterBase::HandleArrivedInteractionPoint()
{
	AActor* InteractActor = InteractComponent->GetInteractActor();
	USceneComponent* InteractionPoint = IInteractInterface::Execute_GetEnterInteractLocation(InteractActor, this);

	GetController()->SetIgnoreMoveInput(false);

	if (InteractActor->ActorHasTag("Ride"))
	{
		IInteractInterface::Execute_RegisterInteractActor(InteractActor, this);

		DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		Ride = Cast<ACharacter>(InteractActor);
		CurRideStance = ERideStance::Mount;
		CurrentState = ECharacterState::Ride;
	}
	else if (InteractActor->ActorHasTag("Ladder"))
	{
		CanMovementInput = false;
		ClimbComponent->RegisterClimbObject(InteractActor);
		float ComparisonHeight = ClimbComponent->GetInitBottomPosition().GetValue().GetLocation().Z;
		TOptional<int32> GripInterval = ClimbComponent->FindGripLevelDifference(ClimbComponent->GetLowestGrip1D(), ClimbComponent->GetGripByHeightUpWard(130.0f, ComparisonHeight));

		if (GripInterval.IsSet())
		{
			if (InteractionPoint->ComponentHasTag("Bottom"))
			{
				CurLadderStance = ELadderStance::Enter_From_Bottom;
				Grip1D_Foot_R = ClimbComponent->GetLowestGrip1D();
				Grip1D_Hand_L = ClimbComponent->GetGripByHeightUpWard(130.0f, ComparisonHeight);
				Grip1D_Hand_R = Grip1D_Hand_L != nullptr ? ClimbComponent->GetGripNeighborUp(Grip1D_Hand_L) : nullptr;
			}
			else
			{
				CurLadderStance = ELadderStance::Enter_From_Top;
				Grip1D_Hand_L = ClimbComponent->GetHighestGrip1D();
				Grip1D_Hand_R = ClimbComponent->GetHighestGrip1D();
				Grip1D_Foot_L = ClimbComponent->GetGripNeighborDown(Grip1D_Hand_L, GripInterval.GetValue() - 1);
				Grip1D_Foot_R = ClimbComponent->GetGripNeighborDown(Grip1D_Foot_L);
			}

			CurrentState = ECharacterState::Ladder;
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::ProbeOnly);
			IsClimb = true;
		}
	}
	else if (InteractActor->ActorHasTag("NPC"))
	{
		IInteractInterface::Execute_Interact(InteractActor, this);
	}

	IsInteraction = true;
}

void ACharacterBase::OnBlock()
{
	CharacterStatusComponent->SetCombatState(ECharacterCombatState::Block);
}

void ACharacterBase::OffBlock()
{
	CharacterStatusComponent->SetCombatState(ECharacterCombatState::Normal);
}

void ACharacterBase::OnHit_Implementation(const FAttackInfo& AttackInfo, const FVector HitPoint)
{
	if (HitReactionComponent != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Hit"));
		FHitReactionRequest InputReaction = {
			AttackInfo.Feature.begin()->Response,
			AttackInfo.Feature.begin()->CanBlocked,
			AttackInfo.Feature.begin()->CanParried,
			AttackInfo.Feature.begin()->CanAvoid,
			HitPoint
		};
		HitReactionComponent->ExecuteHitResponse(InputReaction);
	}
}

void ACharacterBase::SpawnRide()
{
	if (!CanRide)
		return;

	Ride = GetWorld()->SpawnActor<APlayerRide>(GetActorLocation(), GetActorRotation());
	if (!Ride)
	{
		UE_LOG(LogTemp, Warning, TEXT("Horse Not Spawn"));
		return;
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	IRideInterface::Execute_Mount(Ride, this, GetVelocity());

	GetCharacterMovement()->SetMovementMode(MOVE_None);
	//GetCharacterMovement()->DisableMovement();

	CurRideStance = ERideStance::Mount;
	CurrentState = ECharacterState::Ride;

	CanMovementInput = false;
	CanRide = false;

	GetWorldTimerManager().SetTimer(MountTimerHandle, this, &ACharacterBase::MountTimer, 0.01f, true);
}

void ACharacterBase::DespawnRide_Implementation(FVector InitVelocity)
{
	if (!Ride)
	{
		return;
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SpringArm->bEnableCameraLag = false;
	SpringArm->bEnableCameraRotationLag = false;

	if (Ride->GetClass()->ImplementsInterface(UViewDataInterface::StaticClass()))
	{
		FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules(
			EDetachmentRule::KeepWorld,
			false
		);

		DetachFromActor(DetachmentRules);

		FTransform SpringArmTransform = IViewDataInterface::Execute_GetSpringArmTransform(Ride);
		float InitTargetArmLength = IViewDataInterface::Execute_GetTargetArmLength(Ride);

		SpringArm->TargetArmLength = InitTargetArmLength;
		SpringArm->SetWorldLocation(SpringArmTransform.GetLocation());
		SpringArm->SetWorldRotation(SpringArmTransform.GetRotation().Rotator());

		FRotator InitControllerRotator = IViewDataInterface::Execute_GetControllerRotation(Ride);

		UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(this);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetControlRotation(InitControllerRotator);
	}
	else
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(this);
	}

	CurRideStance = ERideStance::DisMount;
	//GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	FVector DisMountVelocity = InitVelocity * 0.4f;
	DisMountVelocity.Z = 600.0f;

	LaunchCharacter(DisMountVelocity, true, true);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}, 0.01f, false);

	GetWorldTimerManager().SetTimer(CameraSettingTimerHandle, this, &ACharacterBase::CameraSettingTimer, 0.01f, true);
}

void ACharacterBase::CameraSettingTimer()
{
	bool CheckTargetArmLength = false;

	float CurrentLength = SpringArm->TargetArmLength;
	float NewLength = FMath::FInterpTo(CurrentLength, 200.0f, GetWorld()->GetDeltaSeconds(), 1.0f);

	SpringArm->TargetArmLength = NewLength;

	if (FMath::IsNearlyEqual(NewLength, 200.0f, 1.0f))
	{
		SpringArm->TargetArmLength = 200.0f;
		CheckTargetArmLength = true;
	}

	bool CheckSpringArmLocation = false;

	FVector CurrentLocation = SpringArm->GetRelativeLocation();
	FVector NewLocation = FMath::VInterpTo(CurrentLocation, InitSpringArmLocation, GetWorld()->GetDeltaSeconds(), 1.0f);

	SpringArm->SetRelativeLocation(NewLocation);

	if (SpringArm->GetRelativeLocation().Equals(InitSpringArmLocation))
	{
		SpringArm->SetRelativeLocation(InitSpringArmLocation);
		CheckSpringArmLocation = true;
	}

	if (CheckTargetArmLength && CheckSpringArmLocation)
	{
		SpringArm->bEnableCameraLag = true;
		SpringArm->bEnableCameraRotationLag = true;
		GetWorld()->GetTimerManager().ClearTimer(CameraSettingTimerHandle);
	}
}

void ACharacterBase::JumpDismountTimer()
{
}

void ACharacterBase::MountTimer()
{
	FVector StartLocation = Ride->GetActorLocation();
	FVector TargetLocation = IRideInterface::Execute_GetMountTransform(Ride).GetLocation();

	FVector CurLocation = FMath::Lerp(StartLocation, TargetLocation, CharacterBaseAnim->GetCurveValue(FName("Char_Translation_Y")));
	CurLocation.Z = FMath::Lerp(StartLocation.Z, TargetLocation.Z, CharacterBaseAnim->GetCurveValue(FName("Char_Translation_Z")));

	SetActorLocation(CurLocation);
}

FTransform ACharacterBase::GetCameraTransform_Implementation()
{
	return Camera->GetComponentTransform();
}

FTransform ACharacterBase::GetSpringArmTransform_Implementation()
{
	return SpringArm->GetComponentTransform();
}

float ACharacterBase::GetTargetArmLength_Implementation()
{
	return SpringArm->TargetArmLength;
}

FRotator ACharacterBase::GetControllerRotation_Implementation()
{
	return GetController()->GetControlRotation();
}

TOptional<FVector> ACharacterBase::GetCharBoneLocation(FName BoneName)
{
	return GetMesh()->DoesSocketExist(BoneName) ? TOptional<FVector>(GetMesh()->GetSocketLocation(BoneName)) : TOptional<FVector>();
}

void ACharacterBase::DisMountEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("Dismountend"));
	CurrentState = ECharacterState::Ground;
	/*
	USceneComponent* GetDownLoc = IRideInterface::Execute_GetLeftInteractLocation(Ride);
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

	FComponentTransform CameraTransform = IViewDataInterface::Execute_GetCameraTransform(Ride);
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
	*/
}

/*
void ACharacterBase::TakeDamage_Implementation(FAttackInfo DamageInfo)
{
	IHitReactionInterface::TakeDamage_Implementation(DamageInfo);
}

float ACharacterBase::GetMaxHealth_Implementation()
{
	IHitReactionInterface::GetMaxHealth_Implementation();

	return StatComponent->GetfloatValue("MaxHealth");
}

float ACharacterBase::GetHealth_Implementation()
{
	IHitReactionInterface::GetHealth_Implementation();

	return StatComponent->GetfloatValue("Health");
}

float ACharacterBase::Heal_Implementation(float amount)
{
	IHitReactionInterface::Heal_Implementation(amount);

	return StatComponent->Heal(amount);
}
*/