// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Components/HitReactionComponent.h"
#include "GameFramework/Character.h"
#include "Characters/Components/CharacterStatusComponent.h"
#include "Characters/Data/StatusData.h"
#include "Characters/Components/StatComponent.h"

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

	CachedCharacter = Cast<ACharacter>(GetOwner());

	if (!CachedCharacter.IsValid())
		return;
}


// Called every frame
void UHitReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHitReactionComponent::InitializeComponentLogic()
{
	
}

void UHitReactionComponent::SetHitReactionDT(const UDataTable* HitReactionDT)
{
	HitReactionListDT = HitReactionDT;
}

void UHitReactionComponent::ExecuteHitResponse(const FHitReactionRequest ReactionData)
{
	if (!HitReactionListDT) return;

	const UEnum* EnumPtr = StaticEnum<HitResponse>();
	if (!EnumPtr) return;

	FName ResponseRowName = FName(EnumPtr->GetNameStringByValue(static_cast<int64>(ReactionData.Response)));

	CurHitReactionDTRow = HitReactionListDT->FindRow<FHitReactionInfoList>(ResponseRowName, "");

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
		float AngleDiff = FMath::Abs(FMath::FindDeltaAngleDegrees(ReactionData.HitAngle, PointToAngle));

		if (AngleDiff < MatchScore)
		{
			MatchScore = AngleDiff;
			MatchInfo = &Info;
		}
	}

	if (MatchInfo)
	{
		PlayReaction(MatchInfo, MatchInfo->HitReactionAnimData.begin()->SectionName);
	}
}

void UHitReactionComponent::PlayReaction(const FHitReactionInfo* HitReaction, const FName SectionName)
{
	UAnimInstance* AnimInstance = Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance();
		//GetOwner()->GetComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();

	if (!AnimInstance) return;

	AnimInstance->Montage_Play(HitReaction->Anim, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, false);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *SectionName.ToString());

	FHitReactionAnimData DataForFind;
	DataForFind.SectionName = SectionName;
	const FHitReactionAnimData* FoundData = HitReaction->HitReactionAnimData.Find(DataForFind);

	if (!FoundData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Data Not Found"));
		return;
	}

	AnimInstance->Montage_JumpToSection(FoundData->SectionName);

	if (OnHitReactionDelegate.IsBound())
		OnHitReactionDelegate.Unbind();

	OnHitReactionDelegate.BindUObject(this, &UHitReactionComponent::OnHitReactionEnded, HitReaction, FoundData->SectionName);
	AnimInstance->Montage_SetBlendingOutDelegate(OnHitReactionDelegate);
}

void UHitReactionComponent::OnHitReactionEnded(UAnimMontage* Montage, bool bInterrupted, const FHitReactionInfo* HitReaction, const FName SectionName)
{
	if (Montage != HitReaction->Anim) return;

	FHitReactionAnimData DataForFind;
	DataForFind.SectionName = SectionName;
	const FHitReactionAnimData* FoundData = HitReaction->HitReactionAnimData.Find(DataForFind);

	if (!FoundData) return;

	if (FoundData->IsLoop)
	{
		PlayReaction(HitReaction, SectionName);
	}
	else if (FoundData->HasNextReaction)
	{
		PlayReaction(HitReaction, FoundData->NextSection);
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

HitResponse UHitReactionComponent::EvaluateHitResponse(const FAttackRequest& AttackRequest)
{
	/*
	UCharacterStatusComponent* StatusComp = GetOwner()->FindComponentByClass<UCharacterStatusComponent>();

	if (!StatusComp) return AttackRequest.Response;

	const ECharacterGroundState CombatState = StatusComp->GetGroundState();

	if (CombatState == ECharacterGroundState::Invincible) return HitResponse::None;

	HitResponse FinalResponse = AttackRequest.Response;

	switch (CombatState)
	{
	case ECharacterGroundState::Normal:
	case ECharacterGroundState::Jump:
	{
		if (StatusComp->IsInAir())
			FinalResponse = HitResponse::HitAir;
		break;
	}
	case ECharacterGroundState::Dodge:
	{
		if (AttackRequest.CanAvoid)
		{
			FinalResponse = HitResponse::None;
		}
		break;
	}
	case ECharacterGroundState::Invincible:
	{
		FinalResponse = HitResponse::None;
		break;
	}
	case ECharacterGroundState::Block:
	{
	float HitAngle = CalculateHitAngle(AttackRequest.HitPoint);
		if (AttackRequest.CanBlocked && (FMath::Abs(HitAngle) <= 60.0f))
		{
			switch (AttackRequest.Response)
			{
			case HitResponse::Flinch:
			case HitResponse::KnockBack:
				FinalResponse = HitResponse::Block;
				break;
			case HitResponse::KnockDown:
				FinalResponse = HitResponse::BlockLarge;
				break;
			}
		}
		break;
	}
	case ECharacterGroundState::Parry:
	{
		float HitAngle = CalculateHitAngle(AttackRequest.HitPoint);
		if (AttackRequest.CanParried && (HitAngle >= -60.0f || HitAngle <= 60.0f))
		{
			FinalResponse = HitResponse::None;
			// 패링시 발동될 델리게이트 추가
		}
		break;
	}
	}

	UStatComponent* StatComp = GetOwner()->FindComponentByClass<UStatComponent>();
	if (StatComp)
	{
		switch (FinalResponse)
		{
		case HitResponse::Flinch:
		case HitResponse::KnockBack:
		case HitResponse::KnockDown:
		case HitResponse::HitAir:
		{
			float CurHealth = StatComp->GetHealth();
			StatComp->ChangeHealth(AttackRequest.Damage, EHPChangeType::DirectDamage);
			break;
		}
		case HitResponse::Block:
		case HitResponse::BlockLarge:
		{
			float CurStamina = StatComp->GetStamina();
			bool IsStaminaEnough = StatComp->ChangeStamina(AttackRequest.StanceDamage, ESPChangeType::Blocked);
			if (IsStaminaEnough)
			{
				FinalResponse = FinalResponse == HitResponse::Block ? HitResponse::BlockBreak : HitResponse::BlockStun;
			}
			break;
		}
		}
	}

	return FinalResponse;
	*/
	return HitResponse();
}