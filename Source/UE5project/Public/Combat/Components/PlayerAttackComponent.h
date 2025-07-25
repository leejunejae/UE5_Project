// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Components/AttackComponent.h"
#include "Combat/Data/DataAsset/PlayerAttackDataAsset.h"
#include "PlayerAttackComponent.generated.h"



/**
 * 
*/

UCLASS()
class UE5PROJECT_API UPlayerAttackComponent : public UAttackComponent
{
	GENERATED_BODY()
	
public:
	//void AnalysisAttackData(FName RowName, FName StartSection = FName("None")) ;
	//void ExecuteAttack(FName SectionName);

	void ExecuteAttack(FName AttackName, float Playrate = 1.0f);
	void PlayAnimation(FPlayerAttackInfo AttackInfo, int32 index, float Playrate = 1.0f);
	
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void SetCurAttackType(EWeaponType WeaponType);

	void DetectAttackTarget(UStaticMeshComponent* WeaponMesh, FWeaponSetsInfo WeaponInfo, float StartTime, float EndTime, bool IsSubWeaponAttack = false) override;

private:
	TWeakObjectPtr<ACharacter> CachedCharacter;
	//TScriptInterface<IStatDataInterface> CachedStat;

	UPROPERTY(VisibleAnywhere, Category = Attack)
		FPlayerAttackInfoList CurAttackType;

	UPROPERTY(VisibleAnywhere, Category = Attack)
		FPlayerAttackInfo CurAttackInfo;

	UPROPERTY(VisibleAnywhere, Category = Attack)
		int32 ComboIndex = 0;

	bool bAttackMontageExecute = false;
};
