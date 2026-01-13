// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CustomMathUtility.generated.h"

/**
 * 
 */
namespace CustomMath
{
    namespace Spatial
    {
        FORCEINLINE float Distance2D(const AActor* A, const AActor* B)
        {
            if (!A || !B) return FLT_MAX;
            return FVector::Dist2D(A->GetActorLocation(), B->GetActorLocation());
        }

        FORCEINLINE float AbsDeltaYawDeg(const APawn* SelfPawn, const AActor* Target)
        {
            if (!SelfPawn || !Target) return 180.f;

            const FVector ToTarget = (Target->GetActorLocation() - SelfPawn->GetActorLocation()).GetSafeNormal2D();
            const FVector Forward = SelfPawn->GetActorForwardVector().GetSafeNormal2D();

            const float Dot = FVector::DotProduct(Forward, ToTarget);
            const float CrossZ = FVector::CrossProduct(Forward, ToTarget).Z;

            const float AngleRad = FMath::Atan2(CrossZ, Dot);
            return FMath::Abs(FMath::RadiansToDegrees(AngleRad));
        }
    }

    namespace Math
    {
        static float SmoothStepNormal(float T)
        {
            T = FMath::Clamp(T, 0.f, 1.f);
            return T * T * (3.f - 2.f * T);
        }

        static float Tent(float D, float Min, float Ideal, float Max)
        {
            if (D <= Min || D >= Max) return 0.f;
            if (D < Ideal) return (D - Min) / FMath::Max(KINDA_SMALL_NUMBER, (Ideal - Min));
            return (Max - D) / FMath::Max(KINDA_SMALL_NUMBER, (Max - Ideal));
        }

        static float RampNormal(float D, float Start, float Full)
        {
            return FMath::Clamp((D - Start) / FMath::Max(KINDA_SMALL_NUMBER, (Full - Start)), 0.f, 1.f);
        }

        static float AngleWindowNormal(float AbsDeltaYawDeg, float Good, float Bad)
        {
            const float T = (AbsDeltaYawDeg - Good) / FMath::Max(KINDA_SMALL_NUMBER, (Bad - Good));
            return 1.f - FMath::Clamp(T, 0.f, 1.f);
        }

        static float SoftRange(float X, float Min, float Max, float Feather)
        {
            if (Feather <= 0.f)
                return (X >= Min && X <= Max) ? 1.f : 0.f;

            if (X < Min - Feather) return 0.f;
            if (X < Min) return SmoothStepNormal((X - (Min - Feather)) / Feather);

            if (X <= Max) return 1.f;
            if (X <= Max + Feather) return 1.f - SmoothStepNormal((X - Max) / Feather);

            return 0.f;
        }

        static float Cooldown01(float Remaining, float Cooldown)
        {
            if (Cooldown <= 0.f) return 1.f;
            return 1.f - FMath::Clamp(Remaining / Cooldown, 0.f, 1.f);
        }
    }
}

UCLASS()
class UE5PROJECT_API UCustomMathUtility : public UObject
{
	GENERATED_BODY()
	
};
