// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/Climb/Data/ClimbHeader.h"

uint32 GetTypeHash(const FGrabData& GrabData)
{
	return GetTypeHash(GrabData.MainBone);
}