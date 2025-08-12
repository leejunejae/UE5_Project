// Fill out your copyright notice in the Description page of Project Settings.

// 엔진 헤더
#include "Characters/Player/Warrior/FallenKnight.h"
#include "GameFramework/CharacterMovementComponent.h"

// 입력
#include "EnhancedInputComponent.h"

// Kismet 유틸리티
#include "Kismet/GameplayStatics.h"

// 애니메이션
#include "Characters/Player/Warrior/FallenKnightAnimInstance.h"


AFallenKnight::AFallenKnight()
{
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MAIN_MESH(TEXT("/Game/04_Animations/Player/SK_DC_Knight_UE4_full_Silver.SK_DC_Knight_UE4_full_Silver"));
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

	SheatheMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sheathe"));

	FName SheatheSocket(TEXT("S_Sheathe"));

	if (SheatheMesh)
	{
		SheatheMesh->SetupAttachment(GetMesh(), SheatheSocket);
	}
	
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>FallenKnight_ANIM(TEXT("/Game/00_Character/C_Warrior/CW_Animation/CWA_BP/Player_AnimBP.Player_AnimBP_C"));
	
	if (FallenKnight_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(FallenKnight_ANIM.Class);
	}

	//GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	//GetCharacterMovement()->MaxAcceleration = 2048.0f;
	//GetCharacterMovement()->GroundFriction = 0.1f;
	//GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;

	IsAttack = false;

	CanAttack = true;
}

void AFallenKnight::BeginPlay()
{
	Super::BeginPlay();
}

void AFallenKnight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFallenKnight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{

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
			//bUseControllerRotationYaw = true;
			});

		CharacterBaseAnim->OnParryEnd.AddLambda([this]()->void {
			//IsParry = false;
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

			});
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
}

void AFallenKnight::ResetAttackState()
{
	CurrentCombo = 0;
	IsAttack = false;
	CanAttack = true;
	bUseControllerRotationYaw = true;
}

bool AFallenKnight::IsAttacking()
{
	return IsAttack;
}

bool AFallenKnight::IsRolling()
{
	return IsRoll;
}

int32 AFallenKnight::CheckCombo()
{
	return CurrentCombo;
}

WarriorStance AFallenKnight::GetStance()
{
	return CurStance;
}
