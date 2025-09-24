// Fill out your copyright notice in the Description page of Project Settings.

// 기본 헤더
#include "Characters/Player/CharacterBase.h"

// 이동
#include "Characters/Components/BaseCharacterMovementComponent.h"

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
#include "UI/DefaultWidget.h"
#include "Blueprint/UserWidget.h"

// 애니메이션
#include "Characters/Player/CharacterBaseAnimInstance.h"

// 참조할 액터
#include "Characters/Player/PlayerRide.h"

// 인터페이스
#include "Interaction/Interfaces/InteractInterface.h" ///삭제 예정
#include "Characters/Rideable/Interfaces/RideInterface.h"

// 유저 컴포넌트
#include "Characters/Components/BaseCharacterMovementComponent.h"
#include "Characters/Player/Components/PlayerStatusComponent.h"
#include "Characters/Components/StatComponent.h"
#include "Characters/Components/EquipmentComponent.h"
#include "Combat/Components/CombatComponent.h"
#include "Combat/Components/PlayerAttackComponent.h"
#include "Combat/Components/PlayerHitReactionComponent.h"
#include "Interaction/Climb/Components/ClimbComponent.h"
#include "Interaction/Components/InteractComponent.h"

// Sets default values
ACharacterBase::ACharacterBase(const FObjectInitializer& ObejctInitializer)
	: ACharacter(ObejctInitializer.SetDefaultSubobjectClass<UBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = GetCapsuleComponent();

	CharacterStatusComponent = CreateDefaultSubobject<UPlayerStatusComponent>(TEXT("CharacterStatusComponent"));
	CharacterStatusComponent->bAutoActivate = true;

	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	StatComponent->bAutoActivate = true;

	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));
	EquipmentComponent->bAutoActivate = true;

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->bAutoActivate = true;

	AttackComponent = CreateDefaultSubobject<UPlayerAttackComponent>(TEXT("AttackComponent"));
	AttackComponent->bAutoActivate = true;

	HitReactionComponent = CreateDefaultSubobject<UPlayerHitReactionComponent>(TEXT("HitReactionComponent"));
	HitReactionComponent->bAutoActivate = true;

	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	InteractComponent->bAutoActivate = true;

	ClimbComponent = CreateDefaultSubobject<UClimbComponent>(TEXT("ClimbComponent"));
	ClimbComponent->bAutoActivate = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Character"));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));

	GetCapsuleComponent()->SetCapsuleHalfHeight(90.0f);
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

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Parry(TEXT("/Game/00_Character/C_Input/C_Parry.C_Parry"));
	if (IP_Parry.Succeeded())
	{
		ParryAction = IP_Parry.Object;
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

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Modifier(TEXT("/Game/00_Character/C_Input/C_Modifier.C_Modifier"));
	if (IP_Modifier.Succeeded())
	{
		ModifierAction = IP_Modifier.Object;
	}

	static ConstructorHelpers::FClassFinder<UDefaultWidget>Class_DefualtWidget(TEXT("/Game/00_Character/Data/DefaultWidget_BP"));
	if (Class_DefualtWidget.Succeeded()) DefaultWidgetClass = Class_DefualtWidget.Class;

	static ConstructorHelpers::FObjectFinder<UHitReactionDataAsset> HitReactionDA_Asset(TEXT("/Game/00_Character/Data/HitReactionData/PlayerHitReactionDA.PlayerHitReactionDA"));
	if (HitReactionDA_Asset.Succeeded())
	{
		HitReactionComponent->SetHitReactionDA(HitReactionDA_Asset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UDataAsset> AttackDA_Asset(TEXT("/Game/00_Character/Data/AttackData/PlayerAttack_DA.PlayerAttack_DA"));
	if (AttackDA_Asset.Succeeded())
	{
		AttackComponent->SetAttackDA(AttackDA_Asset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RollMontageAsset(TEXT("/Game/00_Character/C_Warrior/CW_Animation/Animation/Sequence/SSH/Roll/Roll_Montage.Roll_Montage"));
	if (RollMontageAsset.Succeeded())
	{
		RollMontage = RollMontageAsset.Object;
	}

	GetMesh()->SetGenerateOverlapEvents(true);

	CanMovementInput = true;
	CurGroundStance = ELocomotionState::Jog;

	GetCharacterMovement()->MaxWalkSpeed = 450.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 800.0f;
	GetCharacterMovement()->MaxAcceleration = 800.0f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 0.2f; // 1.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->GravityScale = 1.2f;
	GetCharacterMovement()->GroundFriction = 10.0f;
	//GetCharacterMovement()->MaxStepHeight

	EquipmentComponent->SetWeaponSocketName(FName("S_Sword"));
	EquipmentComponent->SetSubEquipSocketName(FName("S_SubEquip"));

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

	//StatComponent->OnDeath.BindUObject(this, &ACharacterBase::Death);
	StatComponent->InitializeStats();
	AttackComponent->SetCurAttackType(EWeaponType::SwordAndShield);

	InitSpringArmLocation = SpringArm->GetRelativeLocation();
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FString EnumName = UEnum::GetValueAsString(CharacterStatusComponent->GetGroundStance_Native());
	//UE_LOG(LogTemp, Warning, TEXT("GroundState = %s"), *EnumName);
	//UE_LOG(LogTemp, Warning, TEXT("EnableDodge = %f"), CharacterBaseAnim->GetCurveValue(FName("EnableDodge")));
	
	/*
	if (CurGroundStance == ELocomotionState::Sprint)
	{
		GetCharacterMovement()->BrakingDecelerationWalking = FMath::FInterpConstantTo(GetCharacterMovement()->BrakingDecelerationWalking, 1300.0f, DeltaTime, 1000.0f);
	}
	else
	{
		GetCharacterMovement()->BrakingDecelerationWalking = FMath::FInterpConstantTo(GetCharacterMovement()->BrakingDecelerationWalking, 800.0f, DeltaTime, 1000.0f);
	}
	*/

	if (bForcedRotatingInputDirection)
	{
		FRotator CurrentRot = GetActorRotation();
		FRotator NewRot = FMath::RInterpConstantTo(CurrentRot, InputRotation, DeltaTime, ForcedRotationSpeed);

		// 일정 각도 이하로 차이 나면 고정
		if (FMath::Abs((NewRot - InputRotation).Yaw) < 1.0f)
		{
			SetActorRotation(InputRotation);
			bForcedRotatingInputDirection = false;
		}
		else
		{
			SetActorRotation(NewRot);
		}
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
}

void ACharacterBase::CameraSetting()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 200.0f;
	SpringArm->SocketOffset = FVector(0.0f, 60.0f, 0.0f);
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f);
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
		
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Ongoing, this, &ACharacterBase::OnBlock);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Triggered, this, &ACharacterBase::OffBlock);
		EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Triggered, this, &ACharacterBase::Parry);
		
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ACharacterBase::Dodge);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACharacterBase::Interact);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ACharacterBase::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ACharacterBase::Jog);

		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Started, this, &ACharacterBase::Walk);
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Triggered, this, &ACharacterBase::Jog);

		EnhancedInputComponent->BindAction(SpawnRideAction, ETriggerEvent::Triggered, this, &ACharacterBase::SpawnRide);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ACharacterBase::AttackInput);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ACharacterBase::AttackInputEnd);

		EnhancedInputComponent->BindAction(ModifierAction, ETriggerEvent::Ongoing, this, &ACharacterBase::ModifierInput);
		EnhancedInputComponent->BindAction(ModifierAction, ETriggerEvent::Triggered, this, &ACharacterBase::ModifierInputEnd);
	}
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterBaseAnim = Cast<UCharacterBaseAnimInstance>(GetMesh()->GetAnimInstance());

	InteractComponent->OnArrivedInteractionPoint.BindUObject(this, &ACharacterBase::HandleArrivedInteractionPoint);


	if (CharacterBaseAnim)
	{
		CharacterBaseAnim->OnEnterLocomotion.BindUObject(this, &ACharacterBase::EnterLocomotion);

		CharacterBaseAnim->OnLeftLocomotion.BindUObject(this, &ACharacterBase::LeftLocomotion);

		CharacterBaseAnim->OnEnterWalkState.AddLambda([this]()->void {
			CharacterStatusComponent->SetCharacterState_Native(ECharacterState::Ground);
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			CanRide = true;
			});

		//CharacterBaseAnim->OnClimbEnd.AddUObject(this, &ACharacterBase::DecidEClimbPhase); 삭제
		CharacterBaseAnim->OnMountEnd.AddUObject(this, &ACharacterBase::MountEnd);
		CharacterBaseAnim->OnDisMountEnd.AddUObject(this, &ACharacterBase::DisMountEnd);
	}

	if (CharacterStatusComponent)
	{
		CharacterStatusComponent->OnDeath.AddUObject(this, &ACharacterBase::OnDeath);
	}

	if (HitReactionComponent)
	{
		HitReactionComponent->OnHitAirReaction.AddUObject(this, &ACharacterBase::HandleHitAir);
	}
}

/* Input Action */
void ACharacterBase::Move(const FInputActionValue& value)
{
	IsMovementInput = true;

	if (!CanMovementInput)
		return;

	const FVector2D DirectionValue = value.Get<FVector2D>();
	
	switch (GetCharacterStatusComponent()->GetCharacterState_Native())
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
			DirectionValue.Y > 0.0f ? ClimbComponent->ClimbUpLadder() : ClimbComponent->ClimbDownLadder();
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
	CanRide = true;
}

void ACharacterBase::LeftLocomotion()
{
	IsLocomotion = false;
	CanRide = false;
}

void ACharacterBase::Dodge()
{
	if (CharacterBaseAnim->GetCurveValue(FName("EnableDodge")) < 0.9f) return;

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

	if (DodgeVector.X == 0.0f && DodgeVector.Y == 0.0f)
		DodgeDegree = 180.0f;
	else
		DodgeDegree = UKismetMathLibrary::DegAtan2(DodgeVector.X, DodgeVector.Y);

	TArray<float> Angles = { -180.0f, -135.0f, -90.0f, -45.0f, 0.0f, 45.0f, 90.0f, 135.0f, 180.0f };

	float Closest = Angles[0];
	float MinDiff = FMath::Abs(FMath::FindDeltaAngleDegrees(DodgeDegree, Closest));

	for (int32 i = 1; i < Angles.Num(); ++i)
	{
		float Diff = FMath::Abs(FMath::FindDeltaAngleDegrees(DodgeDegree, Angles[i]));
		if (Diff < MinDiff)
		{
			MinDiff = Diff;
			Closest = Angles[i];
		}
	}

	static const TMap<float, FName> AngleToDirection = {
		{ 0.0f, FName("Roll_F")},
		{ 45.0f, FName("Roll_FR")},
		{ 90.0f, FName("Roll_R")},
		{ 135.0f, FName("Roll_BR")},
		{ 180.0f, FName("Roll_B")},
		{ -45.0f, FName("Roll_FL")},
		{ -90.0f, FName("Roll_L")},
		{ -135.0f, FName("Roll_BL")},
		{ -180.0f, FName("Roll_B")},
	};

	FName RollDirectionName = AngleToDirection[Closest];

	CharacterBaseAnim->Montage_Stop(0.1f);
	CharacterBaseAnim->Montage_Play(RollMontage);
	CharacterBaseAnim->Montage_JumpToSection(RollDirectionName, RollMontage);

	CharacterStatusComponent->SetGroundStance_Native(EGroundStance::Dodge);
}

void ACharacterBase::Jump()
{
	if (!IsLocomotion || CharacterBaseAnim->GetCurveValue(FName("EnableJump")) < 0.9f)
		return;

	if (CharacterBaseAnim->GetCurrentActiveMontage())
	{
		CharacterBaseAnim->Montage_Stop(0.1f);
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &Super::Jump);
	}
	else
	{
		CharacterStatusComponent->SetGroundStance_Native(EGroundStance::Jump);
		Super::Jump();
	}
	//GetCharacterMovement()->bOrientRotationToMovement = false;
}

void ACharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	CharacterStatusComponent->SetGroundStance_Native(EGroundStance::Normal);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	const bool bNoMoveInput = false;
		//GetInputAxisValue("MoveAction") == 0.f;

	if (bNoMoveInput)
	{
		FVector V = GetCharacterMovement()->Velocity;
		V.X *= 0.1f;
		V.Y *= 0.1f;
		GetCharacterMovement()->Velocity = V;
	}
}

void ACharacterBase::Walk()
{
	CurGroundStance = ELocomotionState::Walk;
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

void ACharacterBase::Jog()
{
	CurGroundStance = ELocomotionState::Jog;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->BrakingFriction = 0.3f;
}

void ACharacterBase::Sprint()
{
	CurGroundStance = ELocomotionState::Sprint;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->BrakingFriction = 0.1f;
}

float ACharacterBase::GetDirection()
{
	return Direction;
}

void ACharacterBase::SetRotationInputDirection_Implementation()
{
	FVector LastMovementInput = GetLastMovementInputVector();
	if (!LastMovementInput.IsNearlyZero())
	{
		InputRotation = LastMovementInput.Rotation();
		bForcedRotatingInputDirection = true;
	}
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

void ACharacterBase::Attack(FName AttackName)
{
	AttackComponent->ExecuteAttack(AttackName);
}

void ACharacterBase::AttackInput()
{
	if (CharacterBaseAnim->GetCurveValue(FName("EnableAttack")) < 0.9f || !EquipmentComponent->CheckWeaponValid(false)) return;

	IsAttackInput = true;

	AttackComponent->ExecuteAttack(FName("DefaultCombo"));
}

void ACharacterBase::ChargeAttackTimer()
{
	AttackChargeTime += 0.1f;
	UE_LOG(LogTemp, Warning, TEXT("AttackTimer : %f"), AttackChargeTime);
	if (AttackChargeTime >= 1.0f || !IsModifierInput || !IsAttackInput)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reset Timer"));
		AttackComponent->ExecuteAttack(FName("ChargeAttack"), 2.0f - AttackChargeTime);
		AttackChargeTime = 0.0f;
		GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	}
}

void ACharacterBase::AttackInputEnd()
{
	IsAttackInput = false;
	//UE_LOG(LogTemp, Warning, TEXT("inputend"));
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

ELocomotionState ACharacterBase::GetCurGroundStance()
{
	return CurGroundStance;
}

ERideStance ACharacterBase::GetCurRideStance()
{
	return CurRideStance;
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
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		Ride = Cast<ACharacter>(InteractActor);
		CurRideStance = ERideStance::Mount;
		CharacterStatusComponent->SetCharacterState_Native(ECharacterState::Ride);
	}
	else if (InteractActor->ActorHasTag("Ladder"))
	{
		bool IsRequestSuccess = ClimbComponent->RequestEnterLadder(InteractActor);
		/*
		CanMovementInput = false;
		ClimbComponent->RegisterClimbObject(InteractActor);
		float ComparisonHeight = ClimbComponent->GetInitBottomPosition().GetValue().GetLocation().Z;
		TOptional<int32> GripInterval = ClimbComponent->FindGripLevelDifference(ClimbComponent->GetLowestGrip1D(), ClimbComponent->GetGripByHeightUpWard(130.0f, ComparisonHeight));

		if (GripInterval.IsSet())
		{
			if (InteractionPoint->ComponentHasTag("Bottom"))
			{
				CurLadderStance = EClimbPhase::Enter_From_Bottom;

				Grip1D_Foot_R = ClimbComponent->GetLowestGrip1D();
				Grip1D_Hand_L = ClimbComponent->GetGripByHeightUpWard(130.0f, ComparisonHeight);
				Grip1D_Hand_R = Grip1D_Hand_L != nullptr ? ClimbComponent->GetGripNeighborUp(Grip1D_Hand_L) : nullptr;
			}
			else
			{
				CurLadderStance = EClimbPhase::Enter_From_Top;

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
		*/
	}
	else if (InteractActor->ActorHasTag("NPC"))
	{
		IInteractInterface::Execute_Interact(InteractActor, this);
	}

	IsInteraction = true;
}

void ACharacterBase::OnBlock()
{
	if (CharacterBaseAnim->GetCurveValue(FName("EnableBlock")) < 0.9f) return;
	IsBlockInput = true;
	CharacterStatusComponent->SetGroundStance_Native(EGroundStance::Block);
}

void ACharacterBase::OffBlock()
{
	IsBlockInput = false;
	CharacterStatusComponent->SetGroundStance_Native(EGroundStance::Normal);
}

void ACharacterBase::Parry()
{
	//CharacterStatusComponent
}

void ACharacterBase::HandleHitAir()
{
	CharacterBaseAnim->SetHitAir(true);
	UE_LOG(LogTemp, Warning, TEXT("Hit Air"));
}

void ACharacterBase::OnHit_Implementation(const FAttackRequest& AttackInfo)
{
	float HitAngle = HitReactionComponent->CalculateHitAngle(AttackInfo.HitPoint);
	
	EHitResponse Response = HitReactionComponent->EvaluateHitResponse(AttackInfo);

	switch (Response)
	{
	case EHitResponse::NoStagger:
	{
		StatComponent->ApplyDamage(AttackInfo.Damage, AttackInfo.AttackType);
		StatComponent->ChangePoise(AttackInfo.PoiseDamage, EStatChangeType::Damage);
	}
	case EHitResponse::Flinch:
	case EHitResponse::KnockBack:
	case EHitResponse::KnockDown:
	{
		StatComponent->ApplyDamage(AttackInfo.Damage, AttackInfo.AttackType);
		bool IsPoiseEnough = StatComponent->ChangePoise(AttackInfo.PoiseDamage, EStatChangeType::Damage);
		if (!IsPoiseEnough && !CharacterStatusComponent->IsDead())
		{
			FHitReactionRequest InputReaction = { Response,HitAngle };
			CharacterStatusComponent->SetGroundStance_Native(EGroundStance::Hit);
			HitReactionComponent->ExecuteHitResponse(InputReaction);
		}
		break;
	}
	case EHitResponse::HitAir:
	{
		StatComponent->ApplyDamage(AttackInfo.Damage, AttackInfo.AttackType);
		bool IsPoiseEnough = StatComponent->ChangePoise(AttackInfo.PoiseDamage, EStatChangeType::Damage);
		if (StatComponent->GetBaseStats_Native().Poise <= 0.0f || CharacterStatusComponent->IsDead())
		{
			CharacterBaseAnim->SetHitAir(true);
			CharacterStatusComponent->SetGroundStance_Native(EGroundStance::Hit);
		}
		break;
	}
	case EHitResponse::Block:
	case EHitResponse::BlockLarge:
	{
		float ApplyGuardBoost = AttackInfo.StanceDamage * (1.0f - EquipmentComponent->GetWeaponSetsData_Native().GuardBoost / 100.0f);
		bool IsStaminaEnough = StatComponent->ChangeStamina(ApplyGuardBoost, EStatChangeType::Damage);
		if (IsStaminaEnough)
		{
			float ApplyNegationDamage = AttackInfo.Damage * (1.0f - EquipmentComponent->GetWeaponSetsData_Native().GuardNegation / 100.0f);
			StatComponent->ApplyDamage(ApplyNegationDamage, AttackInfo.AttackType);
			if (!CharacterStatusComponent->IsDead())
			{
				FHitReactionRequest InputReaction = { Response, HitAngle };
				HitReactionComponent->ExecuteHitResponse(InputReaction);
			}
		}
		else
		{
			StatComponent->ApplyDamage(AttackInfo.Damage, AttackInfo.AttackType);
			Response = Response == EHitResponse::Block ? EHitResponse::BlockBreak : EHitResponse::BlockStun;
		}
		break;
	}
	}
}

void ACharacterBase::OnDeathEnd_Implementation()
{
	// 사망 시 UI 호출
	// 스탯 초기화
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true); // 일단 꺼줌
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 사망효과로 교체
	SetLifeSpan(5.0f);
}

void ACharacterBase::OnDeath()
{
	// 기본 UI 비활성화
	APlayerController* MyController = Cast<APlayerController>(GetController());
	DisableInput(MyController);
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
	CharacterStatusComponent->SetCharacterState_Native(ECharacterState::Ride);

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
	CharacterStatusComponent->SetCharacterState_Native(ECharacterState::Ground);
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