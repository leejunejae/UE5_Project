// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PBasicItem.h"

// Sets default values
APBasicItem::APBasicItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshItem"));

	RootComponent = ItemMesh;
}