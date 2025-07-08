// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Data/DataAsset/PlayerAttackDataAsset.h"

uint32 GetTypeHash(const FPlayerAttackDetail& AttackDetail)
{
	return GetTypeHash(AttackDetail.SectionName);
}

uint32 GetTypeHash(const FPlayerAttackInfo& AttackInfo)
{
	return GetTypeHash(AttackInfo.AttackName);
}

FPlayerAttackInfoList UPlayerAttackDataAsset::FindPlayerAttackInfo(const EWeaponType& WeaponType, bool bLogNotFound) const
{
	const FPlayerAttackInfoList* Info = AttackInfoList.Find(WeaponType);
	if (Info)
	{
		return *Info;
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Not SkillInfo"))
	}

	return FPlayerAttackInfoList();
}