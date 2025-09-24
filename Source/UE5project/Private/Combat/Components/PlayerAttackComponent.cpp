// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Components/PlayerAttackComponent.h"
#include "GameFramework/Character.h"
#include "Utils/AnimBoneTransformDataAsset.h"
#include "Engine/StaticMeshSocket.h"
#include "Combat/Interfaces/HitReactionInterface.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

void UPlayerAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	auto& WeaponChangeSig = CachedEquipment->OnWeaponSetChanged();//IEquipmentDataInterface::Execute_OnWeaponSetChanged(CachedEquipment);
	WeaponChangeSig.AddUObject(this, &UPlayerAttackComponent::SetCurAttackType);
}

void UPlayerAttackComponent::SetCurAttackType(EWeaponType WeaponData)
{
	const EWeaponType WeaponType = WeaponData;
	const UPlayerAttackDataAsset* TypedAsset = Cast<UPlayerAttackDataAsset>(AttackListDA);

	if (!TypedAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackListDA 캐스팅 실패"));
		return;
	}

	FPlayerAttackInfoList InfoList = TypedAsset->FindPlayerAttackInfo(WeaponType, /*bLogNotFound=*/true);

	// 2) 소프트들을 즉시 로드
	for (const FPlayerAttackInfo& Info : InfoList.AttackInfoList)
	{
		// Anim은 하드 포인터(UAnimMontage*)라면 이 시점에 이미 로드될 가능성이 큼(하드 참조 체인).
		// 확실히 하려면 Ensure() 등으로 검증.

		for (const FPlayerAttackDetail& D : Info.AttackDetail)
		{
			if (auto* Loaded = D.BaseAttackData.TargetBoneTransformDataAsset.LoadSynchronous())
			{
				UE_LOG(LogTemp, Warning, TEXT("AttackData LoadSuccessful"));
				// 캐시에 하드 포인터로 저장
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Set AttackData"));

	// 현재 
	CurAttackType = InfoList;
}