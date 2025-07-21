// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/PBWeapon.h"

APBWeapon::APBWeapon()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Hatchet(TEXT("/Game/Asset/Bjorn_Viking/Mesh/SM_Bjorn_Viking_Axe.SM_Bjorn_Viking_Axe"));
	if (SM_Hatchet.Succeeded())
	{
		ItemMesh->SetStaticMesh(SM_Hatchet.Object);
	}

	ItemMesh->SetCollisionProfileName(TEXT("NoCollision"));
}