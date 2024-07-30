// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "CombatStruct.h"
#include "CombatComponent.generated.h"

class UPBEHAnimInstance;
class UPBDamagableInterface;

DECLARE_DELEGATE(FOnSingleDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5PROJECT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
		void OnMontageNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	//void SetMovementMode();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetAttackDT(UDataTable* InitDialogueDT);
	void AnalysisAttackData(FName RowName, FName StartSection = FName("None"));
	void ExecuteAttack(FName SectionName);
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted, const FAttackData* CurAnimData);
	void Detect_LineTrace(FAttackInfo AttackFeature, FVector StartLoc, FVector EndLoc, bool IsDrawLine = false);
	void Detect_Circular(FAttackInfo AttackFeature, FVector Center, FVector Direction, FVector VerticalVector, float StartAngle, float EndAngle, float Radius, int TraceNum = 1, bool IsDrawLine = false);
	void Detect_Circular(FName AttackName, FVector Center, FVector Direction, FVector VerticalVector, float StartAngle, float EndAngle, float Radius, int TraceNum = 1, bool IsDrawLine = false);
	void Detect_Sphere(FVector StartLoc, FVector EndLoc, float Radius);
	void Detect_Capsule(FVector StartLoc, FVector EndLoc, FVector Extent);
	void Detect_Box(FVector StartLoc, FVector EndLoc, FVector HalfExtent);
	void Detect_Collision(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void InflictDamage(AActor* Target);
	void SetDashDistance();
	void SetWeaponData(FVector Start, FVector End, FVector Add = FVector::ZeroVector);

public:
	FOnSingleDelegate OnMotionWarp;

private:
	UFUNCTION()
		void TriggerBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
		//void TriggerEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		class UDataTable* AttackListDT;

	FAttackDataStruct* CurAttackDTRow;
	FAttackData* CurAttack;
	FAttackRange CurRange;

	UAnimInstance* AnimInstance;

	ACharacter* CurTarget;

	FTimerHandle AttackTimerHandle;

	FOnMontageEnded OnMontageEndedDelegate;
};
