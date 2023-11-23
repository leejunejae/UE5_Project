// Fill out your copyright notice in the Description page of Project Settings.


#include "PBEHKnight.h"
#include "PBEHHereticSoldierAI.h"
#include "PBEHHSAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"

APBEHKnight::APBEHKnight()
{
	PrimaryActorTick.bCanEverTick = true;

	//HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MAIN_MESH(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Resource/SK_full_body.SK_full_body"));
	if (MAIN_MESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MAIN_MESH.Object);
	}
	//HeadMesh->SetupAttachment(GetMesh());
	//HeadMesh->SetLeaderPoseComponent(GetMesh());

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

	BracerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bracer"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BRACER_MESH(TEXT("/Game/Asset/Fallen_Knight/Mesh/Separated_Mesh/Armor/S_bracers.S_bracers"));
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
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WEAPON_MESH(TEXT("/Game/Asset/SnSAnimsetPro/Models/Sword/Sword.Sword"));
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
	}

	HSoldierAura = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HSoldierAura"));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>HS_AURA(TEXT("/Game/Enemy/E_Human/EH_HereticSoldier/EHHS_Resource/NS_HSoldierAura.NS_HSoldierAura"));
	if (HS_AURA.Succeeded())
	{
		HSoldierAura->SetAsset(HS_AURA.Object);
	}
	HSoldierAura->SetupAttachment(GetMesh());

	Tags.Add("HereticSoldier");
}

void APBEHKnight::BeginPlay()
{
	Super::BeginPlay();


}

void APBEHKnight::Tick(float DeltaTime)
{

}

void APBEHKnight::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	HSoldierAnim = Cast<UPBEHHSAnimInstance>(GetMesh()->GetAnimInstance());
	if (HSoldierAnim != nullptr)
	{
		HSoldierAnim->OnMontageEnded.AddDynamic(this, &APBEHKnight::IsMontageEnded);

	}

	HSoldierAnim->OnRush.AddLambda([this]()->void {
		ACharacter* Target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		FVector TargetLocation = Target->GetActorLocation();
		FVector RushDirection = (GetActorLocation() - TargetLocation).GetSafeNormal();
		LaunchCharacter(RushDirection * 100.0f, false, false);
		});
}

void APBEHKnight::IsMontageEnded(UAnimMontage* Montage, bool bInterrupted)
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
	}
}

void APBEHKnight::Attack()
{
	if (IsAttack)
		return;
	HSoldierAnim->PlayMontage(MontageType::Attack);
	IsAttack = true;
}

void APBEHKnight::Appear()
{
	if (IsAppear)
		return;
	HSoldierAnim->PlayMontage(MontageType::Appear);
	IsAppear = true;
}

void APBEHKnight::Unsheathe()
{

}