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


AFallenKnight::AFallenKnight(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
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
	
	//CharacterBaseAnim = Cast<UFallenKnightAnimInstance>(GetMesh()->GetAnimInstance());
}