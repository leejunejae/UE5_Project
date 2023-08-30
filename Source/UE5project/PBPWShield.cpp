// Fill out your copyright notice in the Description page of Project Settings.


#include "PBPWShield.h"

APBPWShield::APBPWShield()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Shield(TEXT("/Game/Asset/Fallen_Knight/Mesh/Separated_Mesh/Weapon/SM_Shield.SM_Shield"));
	if (SM_Shield.Succeeded())
	{
		ItemMesh->SetStaticMesh(SM_Shield.Object);
	}
}