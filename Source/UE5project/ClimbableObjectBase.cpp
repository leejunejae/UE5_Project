// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbableObjectBase.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "NavigationSystem.h"
#include "Components/BrushComponent.h"
#include "Function/PBPlayerInterface.h"

AClimbableObjectBase::AClimbableObjectBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("Climbable");

	ClimbStaticMesh = CreateDefaultSubobject<UStaticMesh>(TEXT("ClimbStaticMesh"));

	ClimbTopTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ClimbTopTrigger"));
	ClimbTopTrigger->SetupAttachment(ObjectRoot);

	ClimbBottomTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ClimbBottomTrigger"));
	ClimbBottomTrigger->SetupAttachment(ObjectRoot);

	ClimbTopLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ClimbTopLocation"));
	ClimbTopLocation->SetupAttachment(ObjectRoot);
	ClimbTopLocation->ComponentTags.Add(FName("Top"));

	ClimbBottomLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ClimbBottomLocation"));
	ClimbBottomLocation->SetupAttachment(ObjectRoot);
	ClimbBottomLocation->ComponentTags.Add(FName("Bottom"));
	//ClimbStartLocation->SetRelativeLocation(FVector(70.0f, 44.0f, 85.0f));
	//ClimbStartLocation->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
}

void AClimbableObjectBase::BeginPlay()
{
	Super::BeginPlay();
}

void AClimbableObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AClimbableObjectBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ClimbTopTrigger->OnComponentBeginOverlap.AddDynamic(this, &AClimbableObjectBase::TriggerBegin);
	ClimbTopTrigger->OnComponentEndOverlap.AddDynamic(this, &AClimbableObjectBase::TriggerEnd);
	ClimbBottomTrigger->OnComponentBeginOverlap.AddDynamic(this, &AClimbableObjectBase::TriggerBegin);
	ClimbBottomTrigger->OnComponentEndOverlap.AddDynamic(this, &AClimbableObjectBase::TriggerEnd);
}

USceneComponent* AClimbableObjectBase::GetEnterInteractLocation_Implementation(AActor* Target)
{
	IPBInteractInterface::GetEnterInteractLocation_Implementation(Target);

	FVector DistTopLoc = Target->GetActorLocation() - ClimbTopLocation->GetComponentLocation();
	FVector DistBottomLoc = Target->GetActorLocation() - ClimbBottomLocation->GetComponentLocation();

	return DistTopLoc.Length() < DistBottomLoc.Length() ? ClimbTopLocation : ClimbBottomLocation;
}

TArray<FGripNode1D> AClimbableObjectBase::GetGripList1D_Implementation()
{
	/*
	UE_LOG(LogTemp, Warning, TEXT("GetGripList"));

	for (int32 j = 0; j < GripList1D.Num(); j++)
	{
		UE_LOG(LogTemp, Warning, TEXT("GripList[%d] = %f"), j, GripList1D[j].Position.Z);
	}
	*/

	return GripList1D;
}

void AClimbableObjectBase::TriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		if (OtherActor->GetClass()->ImplementsInterface(UPBPlayerInterface::StaticClass()))
		{
			IPBPlayerInterface::Execute_RegisterInteractableActor(OtherActor, this);
			//UE_LOG(LogTemp, Warning, TEXT("Trigger In"));
		}
	}
}

void AClimbableObjectBase::TriggerEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		if (OtherActor->GetClass()->ImplementsInterface(UPBPlayerInterface::StaticClass()))
		{
			IPBPlayerInterface::Execute_DeRegisterInteractableActor(OtherActor, this);
			//UE_LOG(LogTemp, Warning, TEXT("Trigger Out"));
		}
	}
}
