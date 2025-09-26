 // Fill out your copyright notice in the Description page of Project Settings.
#include "Characters/Enemies/Human/HereticKnight/PBEHHereticSoldier.h"

// 이동
#include "GameFramework/CharacterMovementComponent.h"

// AI
#include "Characters/Enemies/Human/HereticKnight/PBEHHereticSoldierAI.h"

// 애니메이션
#include "Characters/Enemies/Human/HereticKnight/PBEHHSAnimInstance.h"

// Kismet 유틸리티
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// 머터리얼
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

// 컴포넌트
#include "Combat/Components/AttackComponent.h"
#include "Characters/Components/StatComponent.h"

// 파티클
#include "NiagaraComponent.h"

APBEHHereticSoldier::APBEHHereticSoldier(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MAIN_MESH(TEXT("/Game/Asset/Fallen_Knight/Mesh/SK_Fallen_Knight.SK_Fallen_Knight"));
	if (MAIN_MESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MAIN_MESH.Object);
	}

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WEAPON_MESH(TEXT("/Game/Asset/Fallen_Knight/Mesh/Separated_Mesh/Weapon/SM_Sword.SM_Sword"));
	if (WEAPON_MESH.Succeeded())
	{
		Weapon->SetStaticMesh(WEAPON_MESH.Object);
	}

	Weapon->SetCollisionProfileName("Weapon");
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));

	AIControllerClass = APBEHHereticSoldierAI::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance>HS_ANIM(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Animation/EHHS_BP/EHHS_AnimBlueprint.EHHS_AnimBlueprint_C"));
	if (HS_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(HS_ANIM.Class);
	}

	FName WeaponSocket(TEXT("S_LongSword"));

	if (Weapon)
	{
		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
		Weapon->SetCanEverAffectNavigation(false);
	}

	HSoldierAura = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HSoldierAura"));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>HS_AURA(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Resource/NS_HSoldierAura.NS_HSoldierAura"));
	if (HS_AURA.Succeeded())
	{
		HSoldierAura->SetAsset(HS_AURA.Object);
	}
	HSoldierAura->SetupAttachment(GetMesh());
	HSoldierAura->bAutoActivate = false;

	HSoldierDissolve = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HSoldierDissolve"));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>HS_DISSOLVE(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Resource/NS_DissolveSoldier.NS_DissolveSoldier"));
	if (HS_DISSOLVE.Succeeded())
	{
		HSoldierDissolve->SetAsset(HS_DISSOLVE.Object);
	}
	HSoldierDissolve->SetupAttachment(GetMesh());
	HSoldierDissolve->bAutoActivate = false;

	HSoldierSpawn = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HSoldierSpawn"));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>HS_SPAWN(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Resource/NS_SpawnSoldier2.NS_SpawnSoldier2"));
	if (HS_SPAWN.Succeeded())
	{
		HSoldierSpawn->SetAsset(HS_SPAWN.Object);
	}
	HSoldierSpawn->SetupAttachment(GetMesh());
	HSoldierSpawn->bAutoActivate = false;
	
	static ConstructorHelpers::FObjectFinder<UCurveFloat> DISORSPAWNCURVE (TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Resource/DissolveCurve.DissolveCurve"));
	if(DISORSPAWNCURVE.Succeeded())
	{
		DisORSpawnCurve = DISORSPAWNCURVE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> DashCurveAsset(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Resource/DashCurve.DashCurve"));
	if (DashCurveAsset.Succeeded())
	{
		DashCurve = DashCurveAsset.Object;
	}

	Tags.Add("HereticSoldier");
	ResetCombo();
}

void APBEHHereticSoldier::BeginPlay()
{
	Super::BeginPlay();

	//HSoldierSpawn->Activate(true);
	/*
	BootMesh->CreateDynamicMaterialInstance(0);
	BracerMesh->CreateDynamicMaterialInstance(0);
	BodyMesh->CreateDynamicMaterialInstance(0);
	CapeMesh->CreateDynamicMaterialInstance(0);
	CapeMesh->CreateDynamicMaterialInstance(1);
	ArmorMesh->CreateDynamicMaterialInstance(0);
	GloveMesh->CreateDynamicMaterialInstance(0);
	GorgetMesh->CreateDynamicMaterialInstance(0);
	ShoulderMesh->CreateDynamicMaterialInstance(0);
	PantMesh->CreateDynamicMaterialInstance(0);
	Weapon->CreateDynamicMaterialInstance(0);
	*/

	if (DisORSpawnCurve)
	{	
		FOnTimelineFloat DisORSpawnCallback;
		FOnTimelineEventStatic DisORSpawnCallbackFin;
		DisORSpawnCallback.BindUFunction(this, FName("DisORSpawnUpdate"));
		DisORSpawnCallbackFin.BindUFunction(this, FName("DisOrSpawnFin"));
		DisORSpawnTimeline.AddInterpFloat(DisORSpawnCurve, DisORSpawnCallback);
		DisORSpawnTimeline.SetTimelineFinishedFunc(DisORSpawnCallbackFin);
	}

	if (DashCurve)
	{
		PreviousCurveValue = 0.0f;
		FOnTimelineFloat DashUpdateCallback;
		FOnTimelineEventStatic DashUpdateFinCallback;
		DashUpdateCallback.BindUFunction(this, FName("DashUpdate"));
		DashUpdateFinCallback.BindUFunction(this, FName("DashUpdateFin"));
		DashTimeline.AddInterpFloat(DashCurve, DashUpdateCallback);
		DashTimeline.SetTimelineFinishedFunc(DashUpdateFinCallback);
		DashTimeline.SetLooping(false);
	}

	//GetController()->SetControlRotation
}

void APBEHHereticSoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DisORSpawnTimeline.TickTimeline(DeltaTime);
	DashTimeline.TickTimeline(DeltaTime);
}

void APBEHHereticSoldier::DisORSpawnUpdate(float Value)
{
	float NewValue = FMath::Lerp(-6.0f, 1.0f, Value);
	/*
	BootMesh->SetScalarParameterValueOnMaterials(FName("Amount"), NewValue);
	BracerMesh->SetScalarParameterValueOnMaterials(FName("Amount"), NewValue);
	BodyMesh->SetScalarParameterValueOnMaterials(FName("Amount"), NewValue);
	CapeMesh->SetScalarParameterValueOnMaterials(FName("Amount"), NewValue);
	ArmorMesh->SetScalarParameterValueOnMaterials(FName("Amount"), NewValue);
	GloveMesh->SetScalarParameterValueOnMaterials(FName("Amount"), NewValue);
	GorgetMesh->SetScalarParameterValueOnMaterials(FName("Amount"), NewValue);
	ShoulderMesh->SetScalarParameterValueOnMaterials(FName("Amount"), NewValue);
	PantMesh->SetScalarParameterValueOnMaterials(FName("Amount"), NewValue);
	Weapon->SetScalarParameterValueOnMaterials(FName("Amount"), NewValue);
	*/
}

void APBEHHereticSoldier::DisORSpawnFin()
{
	if (HSoldierAura->IsActive())
	{
		HSoldierAura->Deactivate();
	}
	else
		HSoldierAura->Activate();
}

void APBEHHereticSoldier::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	HSoldierAnim = Cast<UPBEHHSAnimInstance>(GetMesh()->GetAnimInstance());
	if (HSoldierAnim != nullptr)
	{
		HSoldierAnim->OnMontageEnded.AddDynamic(this, &APBEHHereticSoldier::IsMontageEnded);
		HSoldierAnim->OnPlayMontageNotifyBegin.AddDynamic(this, &APBEHHereticSoldier::OnMontageNotifyBegin);
		HSoldierAnim->OnPlayMontageNotifyEnd.AddDynamic(this, &APBEHHereticSoldier::OnMontageNotifyEnd);
		
		/*
		HSoldierAnim->OnNextCombo.AddLambda([this]()->void {
				CanAttack = true;
				CurrentCombo++;
				OnNextAttack.Broadcast();
				Attack(FName(" "));
			});
			*/

		HSoldierAnim->OnEndCombo.AddLambda([this]()->void {
			OnAttackFin.Broadcast();
			});


		HSoldierAnim->OnStartAttack.AddLambda([this]()->void {
			OnAttackStart.Broadcast();
			});

		HSoldierAnim->OnEndAttack.AddLambda([this]()->void {
			OnAttackEnd.Broadcast();
			});

		HSoldierAnim->OnHitEnd.AddLambda([this]()->void {
			IsHit = false;
			});

		HSoldierAnim->OnHitLargeEnd.AddLambda([this]()->void {
			IsHitLarge = false;
			});

		HSoldierAnim->OnBlockBreakEnd.AddLambda([this]()->void {
			IsBlockBreak = false;
			});

		HSoldierAnim->OnDashEnd.AddLambda([this]()->void {
			IsDash = false;
			OnDashEnd.Broadcast();
			GetWorldTimerManager().ClearTimer(DashTimerHandle);
			});

		HSoldierAnim->OnEnterLocomotion.AddLambda([this]()->void {
			IsLookAt = true;
			});

		HSoldierAnim->OnLeftLocomotion.AddLambda([this]()->void {
			IsLookAt = false;
			});
	}

	/*
	AttackComponent->OnMotionWarp.BindLambda([this]()->void {
		FVector TargetLoc = Target->GetActorLocation();
		FVector CharLoc = GetActorLocation();

		FVector TargetDirection = (TargetLoc - CharLoc).GetSafeNormal();
		FRotator TargetRotation = TargetDirection.Rotation();

		MotionWarpComp->AddOrUpdateWarpTargetFromLocation(TEXT("Warp"), Target->GetActorLocation());
		MotionWarpComp->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("Focus"), TargetDirection * 300.0f + TargetLoc, TargetRotation);
		});
		*/
}

void APBEHHereticSoldier::IsMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (nullptr == HSoldierAnim)
		return;
	MontageType Type = HSoldierAnim->CheckMontage(Montage);
	switch (Type)
	{
	case MontageType::Appear:
		IsAppear = false;
		OnAppearEnd.Broadcast();
		break;
	}
}

void APBEHHereticSoldier::ResetCombo()
{
	CanAttack = true;
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	CurSkill = HSoldierSkill::None;
}

void APBEHHereticSoldier::Attack(FName AttackName, ACharacter* Target)
{
	//HSoldierAnim->PlayMontage(FName("Combo1"));
	MotionWarpComp->AddOrUpdateWarpTargetFromLocation(TEXT("Warp"), Target->GetActorLocation());
	MotionWarpComp->AddOrUpdateWarpTargetFromLocation(TEXT("Smash"), Target->GetActorLocation());
	
	/*
	if (CanAttack)
	{
		CanAttack = false;
		if (!IsAttack)
		{
			CurrentCombo++;
			IsAttack = true;
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			int32 RandCombo = 0; //FMath::RandRange(2, 3);
			switch (RandCombo)
			{
			case 0:
				CurSkill = HSoldierSkill::Combo1;
				break;
			case 1:
				CurSkill = HSoldierSkill::Combo2;
				break;
			}
		}
	}
	*/
}

void APBEHHereticSoldier::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{

}

void APBEHHereticSoldier::OnMontageNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{

}

void APBEHHereticSoldier::Appear()
{
	if (IsAppear)
		return;
	//DisORSpawnTimeline.ReverseFromEnd();
	//HSoldierAnim->PlayMontage(MontageType::Appear);
	IsAppear = true;
}

void APBEHHereticSoldier::Death()
{
	DisORSpawnTimeline.PlayFromStart();
	HSoldierDissolve->Activate(true);
	APBEHHereticSoldierAI* HSoldierAI = Cast<APBEHHereticSoldierAI>(GetController());
	HSoldierAI->StopAI();
}

void APBEHHereticSoldier::Block(bool DefenseMode)
{
	IsBlock = true;
	IsDefenseMode = DefenseMode;
}

void APBEHHereticSoldier::Dash(FVector TargetLocation)
{
	//MotionWarpComp->AddOrUpdateWarpTargetFromLocation(TEXT("Dash"), TargetLocation);
	//HSoldierAnim->PlayMontage(FName("Dash"));
	DashDirection = TargetLocation - GetActorLocation();
	DashDirection.Z = 0.0f;

	DashTimerDelegate.BindUObject(this, &APBEHHereticSoldier::SetDashDirection, TargetLocation);
	GetWorldTimerManager().SetTimer(DashTimerHandle, DashTimerDelegate, 0.01f, true);

	DashTimeline.PlayFromStart();
	//GetCharacterMovement()->MoveUpdatedComponent(DashDirection, GetActorRotation(), false);
	IsDash = true;
}

void APBEHHereticSoldier::SetDashDirection(FVector TargetLocation)
{
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	FVector DirectionVector = TargetLocation - GetActorLocation();
	DirectionVector.Z = 0.0f;
	FVector DirectionVectorNormal = DirectionVector.GetSafeNormal();
	FVector ValueX = UKismetMathLibrary::GetForwardVector(YawRotation) * DirectionVectorNormal.X;
	FVector ValueY = UKismetMathLibrary::GetRightVector(YawRotation) * DirectionVectorNormal.Y;
	FVector Direction = ValueY + ValueX;
	FVector InputDegree = GetActorTransform().InverseTransformVectorNoScale(Direction);
	float InputY = InputDegree.X;
	float InputX = InputDegree.Y;

	DashVector = FVector(InputX, InputY, 0.0f);
}

void APBEHHereticSoldier::DashUpdate(float Value)
{
	float AdaptCurveValue = Value - PreviousCurveValue;
	AddActorWorldOffset(DashDirection * AdaptCurveValue);
	PreviousCurveValue = Value;
}

void APBEHHereticSoldier::DashUpdateFin()
{
	PreviousCurveValue = 0.0f;
}

void APBEHHereticSoldier::SetHSoldierMode(HSoldierMode NextMode)
{
	if (NextMode != CurMode)
	{
		if (NextMode == HSoldierMode::OffenseMode)
		{
			IsDefenseMode = false;
			IsBlock = false;
		}
		else
		{
			IsDefenseMode = true;
			IsBlock = true;
		}
		CurMode = NextMode;
	}
}

bool APBEHHereticSoldier::CheckBool(HSoldierVar CheckVar)
{
	switch (CheckVar)
	{
	case HSoldierVar::IsAttack:
		return false;
	case HSoldierVar::IsHit:
		return IsHit;
	case HSoldierVar::IsHitLarge:
		return IsHitLarge;
	case HSoldierVar::IsBlock:
		return IsBlock;
	case HSoldierVar::IsBlockBreak:
		return IsBlockBreak;
	case HSoldierVar::IsParried:
		return IsParried;
	case HSoldierVar::IsDefenseMode:
		return IsDefenseMode;
	default:
		return false;
	}
}

bool APBEHHereticSoldier::IsDashing()
{
	return IsDash;
}

HSoldierSkill APBEHHereticSoldier::CheckSkill()
{
	return CurSkill;
}

int32 APBEHHereticSoldier::CheckCombo()
{
	return CurrentCombo;
}

FVector APBEHHereticSoldier::GetDashDirection()
{
	return DashVector;
}
