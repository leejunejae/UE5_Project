// Fill out your copyright notice in the Description page of Project Settings.


#include "PBPWSword.h"

APBPWSword::APBPWSword()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Sword(TEXT("/Game/Asset/Fallen_Knight/Mesh/Separated_Mesh/Weapon/SM_Sword.SM_Sword"));
	if (SM_Sword.Succeeded())
	{
		ItemMesh->SetStaticMesh(SM_Sword.Object);
	}

	WeaponStat.AttackRange = 200.0f;
	WeaponStat.StrikingPower = 10.0f;
}