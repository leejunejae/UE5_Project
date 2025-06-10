// Fill out your copyright notice in the Description page of Project Settings.

// 엔진 헤더
#include "FallenKnight.h"
#include "GameFramework/CharacterMovementComponent.h"

// 입력
#include "EnhancedInputComponent.h"

// Kismet 유틸리티
#include "Kismet/GameplayStatics.h"

// 애니메이션
#include "FallenKnightAnimInstance.h"


AFallenKnight::AFallenKnight()
{
	PrimaryActorTick.bCanEverTick = true;
	
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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WEAPON_MESH(TEXT("/Game/Asset/SnSAnimsetPro/Models/Sword/Sword.Sword"));
	if (WEAPON_MESH.Succeeded())
	{
		WeaponMesh->SetStaticMesh(WEAPON_MESH.Object);
	}
	
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
	static ConstructorHelpers::FClassFinder<UAnimInstance>FallenKnight_ANIM(TEXT("/Game/00_Character/C_Warrior/CW_Animation/CWA_BP/ABP_FallenKnight.ABP_FallenKnight_C"));
	if (FallenKnight_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(FallenKnight_ANIM.Class);
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> DodgeCurveAsset(TEXT("/Game/00_Character/C_Source/DodgeCurve.DodgeCurve"));
	if (DodgeCurveAsset.Succeeded())
	{
		DodgeCurve = DodgeCurveAsset.Object;
	}

	//GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	//GetCharacterMovement()->MaxAcceleration = 2048.0f;
	//GetCharacterMovement()->GroundFriction = 0.1f;
	//GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;

	IsAttack = false;

	CanAttack = true;
	CanDodge = true;
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
	}
}

void AFallenKnight::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	CharacterBaseAnim = Cast<UFallenKnightAnimInstance>(GetMesh()->GetAnimInstance());
	//CharacterBaseAnim = Cast<UCharacterBaseAnimInstance>(GetMesh()->GetAnimInstance());

	if (CharacterBaseAnim)
	{
		CharacterBaseAnim->OnMontageEnded.AddDynamic(this, &AFallenKnight::IsMontageEnded);
		CharacterBaseAnim->OnMontageBlendingOut.AddDynamic(this, &AFallenKnight::IsMontageBlendingOut);

		CharacterBaseAnim->OnSetAttackDirection.AddLambda([this]()->void {
			FVector LastMovementInput = GetLastMovementInputVector();
			if (!LastMovementInput.IsNearlyZero())
			{
				TargetRotation = LastMovementInput.Rotation();
				SetActorRotation(TargetRotation);
			}
			});

		CharacterBaseAnim->OnNextAttackCheck.AddLambda([this]()->void {
			CanAttack = true;
			});

		CharacterBaseAnim->OnEndAttack.AddLambda([this]()->void {
			CurrentCombo = 0;
			IsAction = false;
			IsAttack = false;
			CanAttack = true;
			CanDodge = true;
			//bUseControllerRotationYaw = true;
			});

		CharacterBaseAnim->OnParryEnd.AddLambda([this]()->void {
			//IsParry = false;
			CanDodge = true;
			});

		CharacterBaseAnim->OnResetHurt.AddLambda([this]()->void {
			IsAttack = false;
			IsInvincible = false;
			});

		CharacterBaseAnim->OnAttackStart.AddLambda([this]()->void {
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

		CharacterBaseAnim->OnCanDodge.AddLambda([this]()->void {
			//IsDodge = false;
			CanDodge = true;
			});

		/*
		CharacterBaseAnim->OnEquipEnd.AddLambda([this]()->void {
			CanAttack = true;
			CanDodge = true;
			});

		CharacterBaseAnim->OnHolsterEnd.AddLambda([this]()->void {
			CanAttack = true;
			CanDodge = true;
			});
		*/
	}
}

void AFallenKnight::IsMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (nullptr==CharacterBaseAnim)
		return;

}

void AFallenKnight::IsMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (nullptr == CharacterBaseAnim)
		return;
/*	MontageType Type = CharacterBaseAnim->CheckMontage(Montage);

	switch (Type)
	{
	case MontageType::Dodge:
		IsDodge = false;
		CanAttack = true;
		break;
	default:
		break;
	}*/
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
			IHitReactionInterface* GetDamagedEnemy = Cast<IHitReactionInterface>(HitResult.GetActor());
			GetDamagedEnemy->TakeDamage_Implementation(AttackInfo);
			if (GetWorldTimerManager().IsTimerActive(AttackTimerHandle))
				GetWorldTimerManager().ClearTimer(AttackTimerHandle);
		}
	}
	*/
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
	
	CanDodge = false;
	//IsDodge = true;
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
}

void AFallenKnight::DodgeUpdateFin()
{
	PreviousCurveValue = 0.0f;
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
