// Fill out your copyright notice in the Description page of Project Settings.


#include "HitReactionComponent.h"
#include "GameFramework/Character.h"
#include "../CharacterStatusComponent.h"

// Sets default values for this component's properties
UHitReactionComponent::UHitReactionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHitReactionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	AnimInstance = GetOwner()->GetComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();
}


// Called every frame
void UHitReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHitReactionComponent::SetHitReactionDT(const UDataTable* HitReactionDT)
{
	HitReactionListDT = HitReactionDT;
}

void UHitReactionComponent::ExecuteHitResponse(const FHitReactionRequest ReactionData)
{
	if (!HitReactionListDT) return;

	float HitAngle = CalculateHitAngle(ReactionData.HitPoint);

	HitResponse Response = EvaluateHitResponse(ReactionData.Response, ReactionData.CanBlocked, ReactionData.CanParried, ReactionData.CanAvoid, HitAngle);

	const UEnum* EnumPtr = StaticEnum<HitResponse>();
	if (!EnumPtr) return;

	FName ResponseRowName = FName(EnumPtr->GetNameStringByValue(static_cast<int64>(Response)));

	CurHitReactionDTRow = HitReactionListDT->FindRow<FHitReactionInfoList>(ResponseRowName, "");

	UE_LOG(LogTemp, Warning, TEXT("%s"), *ResponseRowName.ToString());

	if (!CurHitReactionDTRow) return;

	static const TMap<EHitPointHorizontal, float> DirectionToYaw = {
		{ EHitPointHorizontal::Front, 0.0f },
		{ EHitPointHorizontal::FrontRight, 45.0f },
		{ EHitPointHorizontal::Right, 90.0f },
		{ EHitPointHorizontal::BackRight, 135.0f },
		{ EHitPointHorizontal::Back, 180.0f },
		{ EHitPointHorizontal::BackLeft, -135.0f },
		{ EHitPointHorizontal::Left, -90.0f },
		{ EHitPointHorizontal::FrontLeft, -45.0f }
	};

	float MatchScore = 180.0f;

	FHitReactionInfo* MatchInfo = nullptr;

	for (FHitReactionInfo& Info : CurHitReactionDTRow->HitReactionInfo)
	{
		EHitPointHorizontal CurrentPoint = Info.HitPointHorizontal;

		float PointToAngle = DirectionToYaw[CurrentPoint];
		float AngleDiff = FMath::Abs(FMath::FindDeltaAngleDegrees(HitAngle, PointToAngle));

		if (AngleDiff < MatchScore)
		{
			MatchScore = AngleDiff;
			MatchInfo = &Info;
		}
	}
	
	if (MatchInfo && AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f"), HitAngle);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *MatchInfo->SectionName.ToString());
		AnimInstance->Montage_Play(MatchInfo->Anim);
		AnimInstance->Montage_JumpToSection(MatchInfo->SectionName);
		
		if (MatchInfo->IsLoop)
		{

		}
	}
}

float UHitReactionComponent::CalculateHitAngle(const FVector HitPoint)
{
	FVector CharacterLocation = GetOwner()->GetActorLocation();
	FVector ImpactVector = HitPoint - CharacterLocation;
	FRotator HitRotator = ImpactVector.Rotation();

	float HitYaw = HitRotator.Yaw;
	float CharacterYaw = GetOwner()->GetActorRotation().Yaw;
	float HitAngle = FMath::FindDeltaAngleDegrees(CharacterYaw, HitYaw);

	return HitAngle;
}

HitResponse UHitReactionComponent::EvaluateHitResponse(const HitResponse& InputResponse, const bool CanBlocked, const bool CanParried, const bool CanAvoid, const float HitAngle)
{
	UCharacterStatusComponent* StatusComp = GetOwner()->FindComponentByClass<UCharacterStatusComponent>();

	const ECharacterCombatState CombatState = StatusComp->GetCombatState();

	switch (CombatState)
	{
	case ECharacterCombatState::Block:
	{
		if (CanBlocked && (HitAngle >= -60.0f && HitAngle <= 60.0f))
		{
			switch (InputResponse)
			{
			case HitResponse::Flinch: return HitResponse::Block;
			case HitResponse::KnockBack: return HitResponse::Block;
			case HitResponse::KnockDown: return HitResponse::BlockLarge;
			}
		}
		else
		{
			return InputResponse;
		}
	}
	default:
		return InputResponse;
	}
}

