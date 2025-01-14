// Fill out your copyright notice in the Description page of Project Settings.
#include "FallenKnight.h"
#include "FallenKnightAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

AFallenKnight::AFallenKnight()
{
	PrimaryActorTick.bCanEverTick = true;
	
	/*
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>WARRIOR_MESH(TEXT("/Game/Asset/Fallen_Knight/Mesh/SK_Fallen_Knight.SK_Fallen_Knight"));
	if (WARRIOR_MESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(WARRIOR_MESH.Object);
	}
	*/

	static ConstructorHelpers::FObjectFinder<UInputAction>IP_Parry(TEXT("/Game/Character/C_Input/C_Parry.C_Parry"));
	if (IP_Parry.Succeeded())
	{
		ParryAction = IP_Parry.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MAIN_MESH(TEXT("/Game/Asset/Fallen_Knight/Mesh/Separated_Mesh/Character/SK_full_body.SK_full_body"));
	if (MAIN_MESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MAIN_MESH.Object);
	}

	TopMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Top"));
	TopMesh->SetupAttachment(GetMesh());
	TopMesh->SetLeaderPoseComponent(GetMesh());

	BottomMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bottom"));
	BottomMesh->SetupAttachment(GetMesh());
	BottomMesh->SetLeaderPoseComponent(GetMesh());

	ShoesMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoes"));
	ShoesMesh->SetupAttachment(GetMesh());
	ShoesMesh->SetLeaderPoseComponent(GetMesh());

	GlovesMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gloves"));
	GlovesMesh->SetupAttachment(GetMesh());
	GlovesMesh->SetLeaderPoseComponent(GetMesh());

	HatMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hat"));
	HatMesh->SetupAttachment(GetMesh());
	HatMesh->SetLeaderPoseComponent(GetMesh());

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WEAPON_MESH(TEXT("/Game/Asset/SnSAnimsetPro/Models/Sword/Sword.Sword"));
	if (WEAPON_MESH.Succeeded())
	{
		WeaponMesh->SetStaticMesh(WEAPON_MESH.Object);
	}

	SubEquipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SubEquip"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SUBEQUIP_MESH(TEXT("/Game/Asset/Bjorn_Viking/Mesh/SM_Bjorn_Viking_Shield.SM_Bjorn_Viking_Shield"));
	if (SUBEQUIP_MESH.Succeeded())
	{
		SubEquipMesh->SetStaticMesh(SUBEQUIP_MESH.Object);
	}

	SheatheMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sheathe"));

	FName WeaponSocket(TEXT("S_Sword"));
	FName ShieldSocket(TEXT("S_Shield"));
	FName SheatheSocket(TEXT("S_Sheathe"));

	if (WeaponMesh)
	{
		WeaponMesh->SetupAttachment(GetMesh(), WeaponSocket);
	}
	
	if (SubEquipMesh)
	{
		SubEquipMesh->SetupAttachment(GetMesh(), ShieldSocket);
	}

	if (SheatheMesh)
	{
		SheatheMesh->SetupAttachment(GetMesh(), SheatheSocket);
	}
	
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>FallenKnight_ANIM(TEXT("/Game/Character/C_Warrior/CW_Animation/CWA_BP/ABP_FallenKnight.ABP_FallenKnight_C"));
	if (FallenKnight_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(FallenKnight_ANIM.Class);
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> DodgeCurveAsset(TEXT("/Game/Character/C_Source/DodgeCurve.DodgeCurve"));
	if (DodgeCurveAsset.Succeeded())
	{
		DodgeCurve = DodgeCurveAsset.Object;
	}

	//GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	//GetCharacterMovement()->MaxAcceleration = 2048.0f;
	//GetCharacterMovement()->GroundFriction = 0.1f;
	//GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;

	IsAttack = false;

	AttackRange = 200.0f;
	CanAttack = true;
	CanDodge = true;
	CurHandRight = true;

	CharInfo.SetName("Warrior");
	CharInfo.SetMaxHP(100);
	CharInfo.SetOffensePower(10);
	CharInfo.SetDefenseCap(10);
}

void AFallenKnight::BeginPlay()
{
	Super::BeginPlay();
	
	if (DodgeCurve)
	{
		PreviousCurveValue = 0.0f;
		FOnTimelineFloat DodgeUpdateCallback;
		FOnTimelineEventStatic DodgeUpdateFinCallback;
		DodgeUpdateCallback.BindUFunction(this, FName("DodgeUpdate"));
		DodgeUpdateFinCallback.BindUFunction(this, FName("DodgeUpdateFin"));
		DodgeTimeline.AddInterpFloat(DodgeCurve, DodgeUpdateCallback);
		DodgeTimeline.SetTimelineFinishedFunc(DodgeUpdateFinCallback);
		DodgeTimeline.SetLooping(false);
	}
}

void AFallenKnight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DodgeTimeline.TickTimeline(DeltaTime);
}

void AFallenKnight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Ongoing, this, &AFallenKnight::Attack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AFallenKnight::AttackInputEnd);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Ongoing, this, &AFallenKnight::OnBlock);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Triggered, this, &AFallenKnight::OffBlock);
		EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Triggered, this, &AFallenKnight::Parrying);
	}
}

void AFallenKnight::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	FallenKnightAnim = Cast<UFallenKnightAnimInstance>(GetMesh()->GetAnimInstance());
	//FallenKnightAnim = Cast<UCharacterBaseAnimInstance>(GetMesh()->GetAnimInstance());

	if (FallenKnightAnim)
	{
		FallenKnightAnim->OnMontageEnded.AddDynamic(this, &AFallenKnight::IsMontageEnded);
		FallenKnightAnim->OnMontageBlendingOut.AddDynamic(this, &AFallenKnight::IsMontageBlendingOut);

		FallenKnightAnim->OnSetAttackDirection.AddLambda([this]()->void {
			FVector LastMovementInput = GetLastMovementInputVector();
			if (!LastMovementInput.IsNearlyZero())
			{
				TargetRotation = LastMovementInput.Rotation();
				SetActorRotation(TargetRotation);
			}
			});

		FallenKnightAnim->OnNextAttackCheck.AddLambda([this]()->void {
			CanAttack = true;
			});

		FallenKnightAnim->OnEndAttack.AddLambda([this]()->void {
			CurrentCombo = 0;
			IsAction = false;
			IsAttack = false;
			CanAttack = true;
			CanDodge = true;
			//bUseControllerRotationYaw = true;
			});

		FallenKnightAnim->OnParryEnd.AddLambda([this]()->void {
			IsParry = false;
			CanDodge = true;
			});


		FallenKnightAnim->OnEnterLocomotion.AddLambda([this]()->void {
			IsLocomotion = true;
			//IsAttack = false;
			CurResponse = HitResponse::None;
			CanDodge = true;
			//UE_LOG(LogTemp, Warning, TEXT("EnterLocomotion"));
			});

		FallenKnightAnim->OnLeftLocomotion.AddLambda([this]()->void {
			IsLocomotion = false;
			//UE_LOG(LogTemp, Warning, TEXT("LeftLocomotion"));
			});

		FallenKnightAnim->OnResetHurt.AddLambda([this]()->void {
			IsAttack = false;
			CurResponse = HitResponse::None;
			IsDodge = false;
			IsBlock = false;
			IsParry = false;
			IsInvincible = false;
			});

		FallenKnightAnim->OnDodgeEnd.AddLambda([this]()->void {
			CanDodge = true;
			IsDodge = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			//FallenKnightAnim->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
			});

		FallenKnightAnim->OnDodgeStart.AddLambda([this]()->void {
			/*
			if (RollSounds.Num() > 0)
			{
				int32 RandomIndex = FMath::RandRange(0, RollSounds.Num() - 1);
				USoundCue* SelectedSound = RollSounds[RandomIndex];
				if (SelectedSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, SelectedSound, GetActorLocation());
				}
			}
			*/
			IsDodge = true;
			IsRoll = false;
			});

		FallenKnightAnim->OnAttackStart.AddLambda([this]()->void {
			FVector LastMovementInput = GetLastMovementInputVector();
			// 이동 방향이 0 벡터가 아닌 경우에만 회전을 수행합니다.
			if (!LastMovementInput.IsNearlyZero())
			{
				// 이동 방향을 회전으로 변환합니다.
				TargetRotation = LastMovementInput.Rotation();
				// 캐릭터의 회전 값을 설정합니다.
				SetActorRotation(TargetRotation);
				//UE_LOG(LogTemp, Warning, TEXT("SetRotation"));
			}

			CanDodge = false;
			});

		FallenKnightAnim->OnCanDodge.AddLambda([this]()->void {
			IsDodge = false;
			CanDodge = true;
			});

		/*
		FallenKnightAnim->OnEquipEnd.AddLambda([this]()->void {
			CanAttack = true;
			CanDodge = true;
			});

		FallenKnightAnim->OnHolsterEnd.AddLambda([this]()->void {
			CanAttack = true;
			CanDodge = true;
			});
		*/

		FallenKnightAnim->OnMountEnd.AddUObject(this, &AFallenKnight::MountEnd);
		FallenKnightAnim->OnDisMountEnd.AddUObject(this, &AFallenKnight::DisMountEnd);

		// Ladder Binding
		FallenKnightAnim->OnClimbEnd.AddUObject(this, &AFallenKnight::DecideLadderStance);

		FallenKnightAnim->OnEnterWalkState.AddLambda([this]()->void {
			CurrentState = ECharacterState::Ground;
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			});

		FallenKnightAnim->OnEnterLadderState.AddLambda([this]()->void {
			CurrentState = ECharacterState::Ladder;
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			});
	}
}

void AFallenKnight::IsMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (nullptr==FallenKnightAnim)
		return;
	MontageType Type = FallenKnightAnim->CheckMontage(Montage);
	
	switch (Type)
	{
	case MontageType::Attack:
		break;
	case MontageType::Death:
		break;
	case MontageType::Hit:
		break;
	default:
		break;
	}
}

void AFallenKnight::IsMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (nullptr == FallenKnightAnim)
		return;
	MontageType Type = FallenKnightAnim->CheckMontage(Montage);

	switch (Type)
	{
	case MontageType::Dodge:
		IsDodge = false;
		CanAttack = true;
		break;
	default:
		break;
	}
}

void AFallenKnight::OnMoveEndToLadder()
{
	Super::OnMoveEndToLadder();
	CurStance = WarriorStance::UA;
}

void AFallenKnight::Sprint()
{
	Super::Sprint();
	/*
	if (!IsRun)
	{
		GetCharacterMovement()->MaxAcceleration = 0.0f;
		GetCharacterMovement()->BrakingDecelerationWalking = 1024.0f;
		GetWorldTimerManager().SetTimer(MoveSpeedTimerHandle, this, &AFallenKnight::MoveSpeedTimer, 0.1f, true);
	}
	else
	{
		if(GetWorldTimerManager().IsTimerActive(MoveSpeedTimerHandle))
			GetWorldTimerManager().ClearTimer(MoveSpeedTimerHandle);
		GetCharacterMovement()->MaxAcceleration = 2048.0f;
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;
	}
	*/
}

void AFallenKnight::MoveSpeedTimer()
{
	/*
	if (GetVelocity().Size() <= 300.0f)
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		GetCharacterMovement()->MaxAcceleration = 2048.0f;
		GetWorldTimerManager().ClearTimer(MoveSpeedTimerHandle);
		GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;
	}*/
}


void AFallenKnight::Attack()
{
	/*
	if (!CanAttack)
	{
		if (IsAction)
			return;
		else
			CanAttack = true;
	}
	*/

	Super::Attack();

	IsAttack = true;
	AttackInput = true;
	CurHandRight = !CurHandRight;

	UE_LOG(LogTemp, Warning, TEXT("input"));
	//}
}

void AFallenKnight::AttackInputEnd()
{
	IsAttack = false;
	AttackInput = false;
	UE_LOG(LogTemp, Warning, TEXT("inputend"));
}

void AFallenKnight::AttackTimer()
{
	FVector StartLoc = GetMesh()->GetSocketLocation("S_RangeStart");
	FVector EndLoc = GetMesh()->GetSocketLocation("S_RangeEnd");

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Red, false, 1.0f);
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult, 
		StartLoc, 
		EndLoc, 
		ECC_Visibility, 
		CollisionParams
	);

	/*
	if (bHit)
	{
		if (HitResult.GetActor()->ActorHasTag("Enemy"))
		{
			IPBDamagableInterface* GetDamagedEnemy = Cast<IPBDamagableInterface>(HitResult.GetActor());
			GetDamagedEnemy->TakeDamage_Implementation(AttackInfo);
			if (GetWorldTimerManager().IsTimerActive(AttackTimerHandle))
				GetWorldTimerManager().ClearTimer(AttackTimerHandle);
		}
	}
	*/
}

void AFallenKnight::OnBlock()
{
	IsBlock = true;
	//UE_LOG(LogTemp, Warning, TEXT("Block on"));
}

void AFallenKnight::OffBlock()
{
	IsBlock = false;
	//UE_LOG(LogTemp, Warning, TEXT("Block off"));
}

void AFallenKnight::SwitchStance()
{
	CurStance = CurStance == WarriorStance::UA ? WarriorStance::SNS : WarriorStance::UA;
	CanAttack = false;
	CanDodge = false;
}

void AFallenKnight::ResetAttackState()
{
	CurrentCombo = 0;
	IsAttack = false;
	CanAttack = true;
	bUseControllerRotationYaw = true;
}

void AFallenKnight::Dodge()
{
	if (!CanDodge)
		return;

	Super::Dodge();

	// 8-Way Dodge
	/*
	if (IsDodge)
	{
		NextDodge = true;
	}

	IsAction=true;
	
	CanDodge = false;
	CanAttack = false;
	*/
	//CanDodge = false;
	
	/*
	FallenKnightAnim->PlayMontage(MontageType::Dodge);
	switch (CurStance)
	{
	case WarriorStance::UA:
		FallenKnightAnim->Montage_JumpToSection("Default");
		break;
	default:
		FallenKnightAnim->Montage_JumpToSection("Combat");
		break;
	}
	*/
	CanDodge = false;
	IsDodge = true;
	IsRoll = true;
	//GetCharacterMovement()->bOrientRotationToMovement = false;
	InitPosition = GetActorLocation();
	DodgeTimeline.PlayFromStart();
}

void AFallenKnight::DodgeUpdate(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("DodgeUpdateFunc"));

	float AdaptCurveValue = Value - PreviousCurveValue;
	AddActorWorldOffset(DodgeDirection * AdaptCurveValue);
	PreviousCurveValue = Value;

	/*
	FVector NewLocation;
	FVector Start = GetActorLocation();
	FVector End = Start + (GetActorUpVector() * -200.0f);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
	{
		FVector Normal = HitResult.ImpactNormal;
		FVector CorrectedDirection = FVector::VectorPlaneProject(DodgeDirection, Normal).GetSafeNormal();
		NewLocation = (CorrectedDirection * 100.0f * Value);
	}
	else
	{
		NewLocation = (DodgeDirection * 100.0f * Value);
	}
	*/

	//AddMovementInput(NewLocation, Value);
	//GetCharacterMovement()->Velocity = NewLocation;
	//GetCharacterMovement()->RootMotion
	//SetAnimRootMotionTranslationScale
	//GetCharacterMovement()->AddImpulse(NewLocation, true);
	
	//AddActorWorldOffset(NewLocation, true, &BlockResult);
	//SetActorLocation(InitPosition + NewLocation, true, &BlockResult);
}

void AFallenKnight::DodgeUpdateFin()
{
	PreviousCurveValue = 0.0f;
}

void AFallenKnight::Death()
{
	Super::Death();
	if (nullptr == FallenKnightAnim)
		return;
	FallenKnightAnim->PlayMontage(MontageType::Death);
}

void AFallenKnight::Block(bool CanParried)
{
	Super::Block(CanParried);
	if (CanParried)
	{
		UE_LOG(LogTemp, Warning, TEXT("Your Character Parried"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Your Character Blocked"));
	}
}

void AFallenKnight::TakeDamage_Implementation(FAttackInfo DamageInfo)
{
	//UE_LOG(LogTemp, Warning, TEXT("Take Damage"));
	const HitResponse Response = CharResponse(DamageInfo.CanBlocked, DamageInfo.CanAvoid, DamageInfo.CanParried);
	switch (Response)
	{
	case HitResponse::Hit:
		if (GetCharacterMovement()->IsFalling())
		{
			CurResponse = HitResponse::HitAir;
			UE_LOG(LogTemp, Warning, TEXT("HitAir"));
		}
		else
		{
			CurResponse = DamageInfo.Type == AttackType::Light ? HitResponse::Hit : HitResponse::HitLarge;
			UE_LOG(LogTemp, Warning, TEXT("Hit"));
		}
		IsInvincible = true;
		break;
	case HitResponse::Block:
		CurResponse = DamageInfo.Type == AttackType::Light ? HitResponse::Block : HitResponse::BlockLarge;
		IsInvincible = true;
		break;
	case HitResponse::Parry:
		IsInvincible = true;
		break;
	case HitResponse::Invincible:
		UE_LOG(LogTemp, Warning, TEXT("Invincible"));
		break;
	default:
		break;
	}
}

HitResponse AFallenKnight::CharResponse(bool CanBlocked, bool CanAvoid, bool CanParry)
{
	const CharState CurState = GetCharState();
	switch (CurState)
	{
	case CharState::None:
		return HitResponse::Hit;
	case CharState::Block:
		if (!CanBlocked || GetCharacterMovement()->IsFalling())
			return HitResponse::Hit;
		else
			return HitResponse::Block;
	case CharState::Parry:
		if (!CanParry || GetCharacterMovement()->IsFalling())
			return HitResponse::Hit;
		else
			return HitResponse::Parry;
	case CharState::Dodge:
		if (!CanAvoid)
		{
			UE_LOG(LogTemp, Warning, TEXT("State : Hit"));
			return HitResponse::Hit;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("State : Dodge"));
			return HitResponse::Invincible;
		}
	case CharState::Invincible:
		return HitResponse::Invincible;
	default:
		return HitResponse::None;
	}
}

CharState AFallenKnight::GetCharState()
{
	if (IsInvincible)
		return CharState::Invincible;
	else if (IsDodge)
		return CharState::Dodge;
	else if (IsParry)
		return CharState::Parry;
	else if (IsBlock)
		return CharState::Block;
	else
		return CharState::None;
}

HitResponse AFallenKnight::GetCharResponse()
{
	return CurResponse;
}

void AFallenKnight::ResetResponse()
{
	CurResponse = HitResponse::None;
}

void AFallenKnight::Parrying()
{
	IsParry = true;
	CanDodge = false;
}

int AFallenKnight::GetAttackSeed()
{
	return AttackSeed;
}

bool AFallenKnight::IsAttackInput()
{
	return AttackInput;
}

bool AFallenKnight::IsAttacking()
{
	return IsAttack;
}

bool AFallenKnight::IsRolling()
{
	return IsRoll;
}

bool AFallenKnight::IsBlocking()
{
	return IsBlock;
}

bool AFallenKnight::IsParrying()
{
	return IsParry;
}

bool AFallenKnight::IsHit()
{
	return Hit;
}

bool AFallenKnight::IsGuard()
{
	return Guard;
}

bool AFallenKnight::GetCurHand()
{
	return CurHandRight;
}

bool AFallenKnight::GetNextDodge()
{
	return NextDodge;
}

int32 AFallenKnight::CheckCombo()
{
	return CurrentCombo;
}

WarriorStance AFallenKnight::GetStance()
{
	return CurStance;
}
