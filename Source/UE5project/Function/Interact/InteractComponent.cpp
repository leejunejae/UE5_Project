// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComponent.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UInteractComponent::AddInteractObject(AActor* InteractObject)
{
	InteractActorList.Add(InteractObject);
}

void UInteractComponent::RemoveInteractObject(AActor* InteractObject)
{
	InteractActorList.Remove(InteractObject);
}

AActor* UInteractComponent::FindInteractActorByDegree(AActor* StartActor, float SearchDegrees)
{
	return nullptr;
}
