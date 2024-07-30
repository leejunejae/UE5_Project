// Fill out your copyright notice in the Description page of Project Settings.
#include "Warrior.h"
#include "PCWAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

AWarrior::AWarrior()
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

	FName WeaponSocket(TEXT("S_Sword"));
	FName ShieldSocket(TEXT("S_Shield"));

	if (WeaponMesh)
	{
		WeaponMesh->SetupAttachment(GetMesh(), WeaponSocket);
	}
	
	if (SubEquipMesh)
	{
		SubEquipMesh->SetupAttachment(GetMesh(), ShieldSocket);
	}
	
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>WARRIOR_ANIM(TEXT("/Game/Character/C_Warrior/CW_Animation/CWA_BP/ABP_Warrior.ABP_Warrior_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
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

void AWarrior::BeginPlay()
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

void AWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DodgeTimeline.TickTimeline(DeltaTime);
}

void AWarrior::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Ongoing, this, &AWarrior::Attack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AWarrior::AttackInputEnd);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Ongoing, this, &AWarrior::OnBlock);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Triggered, this, &AWarrior::OffBlock);
		EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Triggered, this, &AWarrior::Parrying);
	}
}

void AWarrior::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	WarriorAnim = Cast<UPCWAnimInstance>(GetMesh()->GetAnimInstance());

	if (WarriorAnim)
	{
		WarriorAnim->OnMontageEnded.AddDynamic(this, &AWarrior::IsMontageEnded);
		WarriorAnim->OnMontageBlendingOut.AddDynamic(this, &AWarrior::IsMontageBlendingOut);

		WarriorAnim->OnSetAttackDirection.AddLambda([this]()->void {
			FVector LastMovementInput = GetLastMovementInputVector();
			// 이동 방향이 0 벡터가 아닌 경우에만 회전을 수행합니다.
			if (!LastMovementInput.IsNearlyZero())
			{
				// 이동 방향을 회전으로 변환합니다.
				TargetRotation = LastMovementInput.Rotation();
				// 캐릭터의 회전 값을 설정합니다.
				SetActorRotation(TargetRotation);
				UE_LOG(LogTemp, Warning, TEXT("SetRotation"));
			}
			});

		WarriorAnim->OnNextAttackCheck.AddLambda([this]()->void {
			CanAttack = true;
			});

		WarriorAnim->OnEndAttack.AddLambda([this]()->void {
			CurrentCombo = 0;
			IsAction = false;
			IsAttack = false;
			CanAttack = true;
			CanDodge = true;
			//bUseControllerRotationYaw = true;
			});

		WarriorAnim->OnParryEnd.AddLambda([this]()->void {
			IsParry = false;
			CanDodge = true;
			});



		WarriorAnim->OnEnterLocomotion.AddLambda([this]()->void {
			IsLocomotion = true;
			//IsAttack = false;
			CurResponse = HitResponse::None;
			CanDodge = true;
			//UE_LOG(LogTemp, Warning, TEXT("EnterLocomotion"));
			});

		WarriorAnim->OnLeftLocomotion.AddLambda([this]()->void {
			IsLocomotion = false;
			//UE_LOG(LogTemp, Warning, TEXT("LeftLocomotion"));
			});

		WarriorAnim->OnResetHurt.AddLambda([this]()->void {
			IsAttack = false;
			CurResponse = HitResponse::None;
			IsDodge = false;
			IsBlock = false;
			IsParry = false;
			IsInvincible = false;
			});

		WarriorAnim->OnDodgeEnd.AddLambda([this]()->void {
			CanDodge = true;
			IsDodge = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			//WarriorAnim->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
			});

		WarriorAnim->OnDodgeStart.AddLambda([this]()->void {
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

		WarriorAnim->OnAttackStart.AddLambda([this]()->void {
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

		WarriorAnim->OnCanDodge.AddLambda([this]()->void {
			IsDodge = false;
			CanDodge = true;
			});

		/*
		WarriorAnim->OnEquipEnd.AddLambda([this]()->void {
			CanAttack = true;
			CanDodge = true;
			});

		WarriorAnim->OnHolsterEnd.AddLambda([this]()->void {
			CanAttack = true;
			CanDodge = true;
			});
		*/

		WarriorAnim->OnMountEnd.AddUObject(this, &AWarrior::MountEnd);
		WarriorAnim->OnDisMountEnd.AddUObject(this, &AWarrior::DisMountEnd);
	}
}

void AWarrior::IsMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (nullptr==WarriorAnim)
		return;
	MontageType Type = WarriorAnim->CheckMontage(Montage);
	
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

void AWarrior::IsMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (nullptr == WarriorAnim)
		return;
	MontageType Type = WarriorAnim->CheckMontage(Montage);

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

void AWarrior::MoveSpeedToggle()
{
	Super::MoveSpeedToggle();
	/*
	if (!IsRun)
	{
		GetCharacterMovement()->MaxAcceleration = 0.0f;
		GetCharacterMovement()->BrakingDecelerationWalking = 1024.0f;
		GetWorldTimerManager().SetTimer(MoveSpeedTimerHandle, this, &AWarrior::MoveSpeedTimer, 0.1f, true);
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

void AWarrior::MoveSpeedTimer()
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


void AWarrior::Attack()
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

void AWarrior::AttackInputEnd()
{
	IsAttack = false;
	AttackInput = false;
	UE_LOG(LogTemp, Warning, TEXT("inputend"));
}

void AWarrior::AttackTimer()
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

void AWarrior::OnBlock()
{
	IsBlock = true;
	//UE_LOG(LogTemp, Warning, TEXT("Block on"));
}

void AWarrior::OffBlock()
{
	IsBlock = false;
	//UE_LOG(LogTemp, Warning, TEXT("Block off"));
}

void AWarrior::SwitchStance()
{
	CurStance = CurStance == WarriorStance::UA ? WarriorStance::SNS : WarriorStance::UA;
	CanAttack = false;
	CanDodge = false;
}

void AWarrior::ResetAttackState()
{
	CurrentCombo = 0;
	IsAttack = false;
	CanAttack = true;
	bUseControllerRotationYaw = true;
}

void AWarrior::Dodge()
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
	WarriorAnim->PlayMontage(MontageType::Dodge);
	switch (CurStance)
	{
	case WarriorStance::UA:
		WarriorAnim->Montage_JumpToSection("Default");
		break;
	default:
		WarriorAnim->Montage_JumpToSection("Combat");
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

void AWarrior::DodgeUpdate(float Value)
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

void AWarrior::DodgeUpdateFin()
{
	PreviousCurveValue = 0.0f;
}

void AWarrior::Death()
{
	Super::Death();
	if (nullptr == WarriorAnim)
		return;
	WarriorAnim->PlayMontage(MontageType::Death);
}

void AWarrior::Block(bool CanParried)
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

void AWarrior::TakeDamage_Implementation(FAttackInfo DamageInfo)
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

HitResponse AWarrior::CharResponse(bool CanBlocked, bool CanAvoid, bool CanParry)
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

CharState AWarrior::GetCharState()
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

HitResponse AWarrior::GetCharResponse()
{
	return CurResponse;
}

void AWarrior::ResetResponse()
{
	CurResponse = HitResponse::None;
}

void AWarrior::Parrying()
{
	IsParry = true;
	CanDodge = false;
}

int AWarrior::GetAttackSeed()
{
	return AttackSeed;
}

bool AWarrior::IsAttackInput()
{
	return AttackInput;
}

bool AWarrior::IsAttacking()
{
	return IsAttack;
}

bool AWarrior::IsRolling()
{
	return IsRoll;
}

bool AWarrior::IsBlocking()
{
	return IsBlock;
}

bool AWarrior::IsParrying()
{
	return IsParry;
}

bool AWarrior::IsHit()
{
	return Hit;
}

bool AWarrior::IsGuard()
{
	return Guard;
}

bool AWarrior::GetCurHand()
{
	return CurHandRight;
}

bool AWarrior::GetNextDodge()
{
	return NextDodge;
}

int32 AWarrior::CheckCombo()
{
	return CurrentCombo;
}

WarriorStance AWarrior::GetStance()
{
	return CurStance;
}
