// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Engine/DataTable.h"
#include "../Function/Interact/PBInteractInterface.h"
#include "../Function/Interact/Dialogue/PBDialogueSystem.h"
#include "../Function/Interact/Dialogue/PBScriptWidget.h"
#include "PBNPCBase.generated.h"

class UPBDialogueSystem;
class UBoxComponent;

UCLASS()
class UE5PROJECT_API APBNPCBase : public ACharacter, public IPBInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APBNPCBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void TriggerBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void TriggerEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual void Interact_Implementation(ACharacter* InteractActor);

	virtual void EndInteract_Implementation();

protected:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UPBScriptWidget> DialogueWidgetClass;

	UPROPERTY(EditAnywhere)
		class UPBScriptWidget* DialogueWidget;

	UPROPERTY(VisibleAnywhere, Category = Interact)
		UBoxComponent* InteractTrigger;

	UPROPERTY(VisibleAnywhere, Category = Dialog)
		UPBDialogueSystem* DialogueSystem;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		FName NPCName;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		class UDataTable* DialogueDT;
};