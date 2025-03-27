// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "MapObjectBase.h"
#include "Function/Interact/InteractInterface.h"
#include "Function/Interact/ClimbObjectInterface.h"
#include "GameplayTagContainer.h"
#include "ClimbHeader.h"
#include "ClimbableObjectBase.generated.h"

/**
 * 
 */
UCLASS()
class UE5PROJECT_API AClimbableObjectBase : public AMapObjectBase, public IInteractInterface, public IClimbObjectInterface
{
	GENERATED_BODY()
	
public:
	AClimbableObjectBase();

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	virtual USceneComponent* GetEnterInteractLocation_Implementation(AActor* Target);


	void GetInteractionTags_Implementation(FGameplayTagContainer& OutTags) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ClimbObjectTags;

#pragma region ClimbObjectInterface
	virtual TArray<FGripNode1D> GetGripList1D_Implementation();

#pragma endregion

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void TriggerBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void TriggerEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, Category = Interact)
		UBoxComponent* ClimbTopTrigger;

	UPROPERTY(VisibleAnywhere, Category = Interact)
		UBoxComponent* ClimbBottomTrigger;

	UPROPERTY(VisibleAnywhere, Category = Interact)
		USceneComponent* ClimbTopLocation;

	UPROPERTY(VisibleAnywhere, Category = Interact)
		USceneComponent* ClimbBottomLocation;

	UPROPERTY(EditAnywhere, Category = Mesh)
		UStaticMesh* ClimbStaticMesh;

	TArray<FGripNode1D> GripList1D;
	TArray<FGripNode2D> GripList2D;
};
