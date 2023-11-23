// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEHHereticSoldier.h"
#include "PBEHHereticSoldierAI.h"
#include "PBEHHSAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "NiagaraComponent.h"

APBEHHereticSoldier::APBEHHereticSoldier()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MAIN_MESH(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Resource/SK_full_body.SK_full_body"));
	if (MAIN_MESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MAIN_MESH.Object);
	}

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BODY_MESH(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Resource/S_chain_armor.S_chain_armor"));
	if (BODY_MESH.Succeeded())
	{
		BodyMesh->SetSkeletalMesh(BODY_MESH.Object);
	}
	BodyMesh->SetupAttachment(GetMesh());
	BodyMesh->SetLeaderPoseComponent(GetMesh());

	PantMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pant"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PANT_MESH(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Resource/S_pants.S_pants"));
	if (PANT_MESH.Succeeded())
	{
		PantMesh->SetSkeletalMesh(PANT_MESH.Object);
	}
	PantMesh->SetupAttachment(GetMesh());
	PantMesh->SetLeaderPoseComponent(GetMesh());

	BootMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Boot"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BOOT_MESH(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Resource/S_boots.S_boots"));
	if (BOOT_MESH.Succeeded())
	{
		BootMesh->SetSkeletalMesh(BOOT_MESH.Object);
	}
	BootMesh->SetupAttachment(GetMesh());
	BootMesh->SetLeaderPoseComponent(GetMesh());

	BracerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bracer"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BRACER_MESH(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Resource/S_bracers.S_bracers"));
	if (BRACER_MESH.Succeeded())
	{
		BracerMesh->SetSkeletalMesh(BRACER_MESH.Object);
	}
	BracerMesh->SetupAttachment(GetMesh());
	BracerMesh->SetLeaderPoseComponent(GetMesh());
	
	GloveMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Glove"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GLOVE_MESH(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Resource/S_gloves.S_gloves"));
	if (GLOVE_MESH.Succeeded())
	{
		GloveMesh->SetSkeletalMesh(GLOVE_MESH.Object);
	}
	GloveMesh->SetupAttachment(GetMesh());
	GloveMesh->SetLeaderPoseComponent(GetMesh());

	CapeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Cape"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CAPE_MESH(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Resource/S_Cape_with_Hood.S_Cape_with_Hood"));
	if (CAPE_MESH.Succeeded())
	{
		CapeMesh->SetSkeletalMesh(CAPE_MESH.Object);
	}
	CapeMesh->SetupAttachment(GetMesh());
	CapeMesh->SetLeaderPoseComponent(GetMesh());

	ShoulderMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoulder"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SHOULDER_MESH(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Resource/S_shoulder.S_shoulder"));
	if (SHOULDER_MESH.Succeeded())
	{
		ShoulderMesh->SetSkeletalMesh(SHOULDER_MESH.Object);
	}
	ShoulderMesh->SetupAttachment(GetMesh());
	ShoulderMesh->SetLeaderPoseComponent(GetMesh());

	GorgetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gorget"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GORGET_MESH(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Resource/S_gorget.S_gorget"));
	if (GORGET_MESH.Succeeded())
	{
		GorgetMesh->SetSkeletalMesh(GORGET_MESH.Object);
	}
	GorgetMesh->SetupAttachment(GetMesh());
	GorgetMesh->SetLeaderPoseComponent(GetMesh());

	ArmorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Armor"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ARMOR_MESH(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Resource/S_breastplate.S_breastplate"));
	if (ARMOR_MESH.Succeeded())
	{
		ArmorMesh->SetSkeletalMesh(ARMOR_MESH.Object);
	}
	ArmorMesh->SetupAttachment(GetMesh());
	ArmorMesh->SetLeaderPoseComponent(GetMesh());

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WEAPON_MESH(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Resource/Sword.Sword"));
	if (WEAPON_MESH.Succeeded())
	{
		Weapon->SetStaticMesh(WEAPON_MESH.Object);
	}

	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));

	AIControllerClass = APBEHHereticSoldierAI::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance>HS_ANIM(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Animation/EHHS_BP/EHHS_AnimBlueprint1.EHHS_AnimBlueprint1_C"));
	if (HS_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(HS_ANIM.Class);
	}

	FName WeaponSocket(TEXT("S_LongSword"));

	if (Weapon)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
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

	Tags.Add("HereticSoldier");
}

void APBEHHereticSoldier::BeginPlay()
{
	Super::BeginPlay();

	HSoldierSpawn->Activate(true);

	if (DisORSpawnCurve)
	{	
		FOnTimelineFloat DisORSpawnCallback;
		FOnTimelineEventStatic DisORSpawnCallbackFin;
		DisORSpawnCallback.BindUFunction(this, FName("DisORSpawnUpdate"));
		DisORSpawnCallbackFin.BindUFunction(this, FName("DisOrSpawnFin"));
		DisORSpawnTimeline.AddInterpFloat(DisORSpawnCurve, DisORSpawnCallback);
		DisORSpawnTimeline.SetTimelineFinishedFunc(DisORSpawnCallbackFin);
	}
}

void APBEHHereticSoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DisORSpawnTimeline.TickTimeline(DeltaTime);
}

void APBEHHereticSoldier::DisORSpawnUpdate(float Value)
{
	float NewValue = FMath::Lerp(-6.0f, 1.0f, Value);

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
}

void APBEHHereticSoldier::DisORSpawnFin()
{
	if (HSoldierAura->IsActive())
	{
		HSoldierAura->Deactivate();
		Destroy();
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

	}

	HSoldierAnim->OnRush.AddUObject(this, &APBEHHereticSoldier::Rush);
	HSoldierAnim->OnTeleport.AddUObject(this, &APBEHHereticSoldier::Stealth);
}

void APBEHHereticSoldier::IsMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (nullptr == HSoldierAnim)
		return;
	MontageType Type = HSoldierAnim->CheckMontage(Montage);
	switch (Type)
	{
	case MontageType::Attack:
		IsAttack = false;
		OnAttackEnd.Broadcast();
		break;
	case MontageType::Appear:
		UE_LOG(LogTemp, Warning, TEXT("Appear2"));
		IsAppear = false;
		OnAppearEnd.Broadcast();
		break;
	case MontageType::Swoop:
		IsSwoop = false;
		OnSwoopEnd.Broadcast();
		break;
	}
}

void APBEHHereticSoldier::Attack()
{
	if (IsAttack)
		return;
	HSoldierDissolve->Activate(true);
	HSoldierAnim->PlayMontage(MontageType::Attack);
	IsAttack = true;
}

void APBEHHereticSoldier::Rush()
{
	ACharacter* Target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector TargetLocation = Target->GetActorLocation();
	FVector RushDirection = (GetActorLocation() - TargetLocation).GetSafeNormal();
	SetActorLocation(TargetLocation + RushDirection * 200.0f);
}

void APBEHHereticSoldier::Appear()
{
	if (IsAppear)
		return;
	DisORSpawnTimeline.ReverseFromEnd();
	HSoldierAnim->PlayMontage(MontageType::Appear);
	IsAppear = true;
}

void APBEHHereticSoldier::Death()
{
	DisORSpawnTimeline.PlayFromStart();
	HSoldierDissolve->Activate(true);
	APBEHHereticSoldierAI* HSoldierAI = Cast<APBEHHereticSoldierAI>(GetController());
	HSoldierAI->StopAI();
}

void APBEHHereticSoldier::Swoop()
{
	if (IsSwoop)
		return;
	ACharacter* Target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector TargetLocation = Target->GetActorLocation();
	FVector AppearDirection = (TargetLocation - GetActorLocation()).GetSafeNormal();
	SetActorLocation(TargetLocation + AppearDirection * 200.0f);
	HSoldierAnim->PlayMontage(MontageType::Swoop);
	IsSwoop = true;
}

void APBEHHereticSoldier::Stealth()
{
	ACharacter* Target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector TargetLocation = Target->GetActorLocation();
	FVector AppearDirection = (TargetLocation - GetActorLocation()).GetSafeNormal();
	SetActorLocation(TargetLocation + AppearDirection * 200.0f + GetActorUpVector() * 300.0f);
}