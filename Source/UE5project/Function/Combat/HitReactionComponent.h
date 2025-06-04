// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// 엔진 헤더
#include "CoreMinimal.h"
#include "HitReactionData.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"

#include "HitReactionComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnMultiDelegate);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROJECT_API UHitReactionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHitReactionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitializeComponentLogic();

	void SetHitReactionDT(const UDataTable* HitReactionDT);
	void ExecuteHitResponse(const FHitReactionRequest ReactionData);
	void PlayReaction(const FHitReactionInfo* HitReaction, const FName SectionName = NAME_None);
	float CalculateHitAngle(const FVector HitPoint);
	HitResponse EvaluateHitResponse(const HitResponse& InputResponse, const bool CanBlocked, const bool CanParried, const bool CanAvoid, const float HitAngle);

	void OnHitReactionEnded(UAnimMontage* Montage, bool bInterrupted, const FHitReactionInfo* HitReaction, const FName SectionName);

	FOnMultiDelegate OnHitAirReaction;
	
private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
		const class UDataTable* HitReactionListDT;

	FHitReactionInfoList* CurHitReactionDTRow;
	FHitReactionInfo* CurHitReaction;
	//UAnimInstance* AnimInstance;

	FOnMontageEnded OnHitReactionDelegate;
};
