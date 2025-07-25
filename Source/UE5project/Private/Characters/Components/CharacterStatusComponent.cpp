// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/CharacterStatusComponent.h"
#include "GameFramework/Character.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/Components/StatComponent.h"

// Sets default values for this component's properties
UCharacterStatusComponent::UCharacterStatusComponent()
{

	// ...
}


// Called when the game starts
void UCharacterStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedCharacter = Cast<ACharacter>(GetOwner());

	UStatComponent* StatComp = GetOwner()->FindComponentByClass<UStatComponent>();
	if (StatComp)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Death DELEGATE"));
		StatComp->OnDeath.BindUObject(this, &UCharacterStatusComponent::HandleDeath);
	}
	// ...
}

bool UCharacterStatusComponent::IsInAir() const
{
	if (const ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		return Character->GetCharacterMovement()->IsFalling();
	}
	return false;
}

void UCharacterStatusComponent::HandleDeath()
{
	bIsDead = true;
	OnDeath.Broadcast();
}