// Fill out your copyright notice in the Description page of Project Settings.


#include "PSupply.h"
#include "Components/BoxComponent.h"

// Sets default values
APSupply::APSupply()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(RootComponent);

	Trigger->SetCollisionProfileName(TEXT("SupplyBox"));
	Trigger->SetBoxExtent(FVector(206.0f, 248.0f, 114.0f));
	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
}

// Called when the game starts or when spawned
void APSupply::BeginPlay()
{
	Super::BeginPlay();
	
}