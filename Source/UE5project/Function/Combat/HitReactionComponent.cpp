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

	//AnimInstance = GetOwner()->GetComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();

	USkeletalMeshComponent* Mesh = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
	UE_LOG(LogTemp, Warning, TEXT("Found mesh: %s"), *Mesh->GetName());

	if (!Mesh)
	{
		UE_LOG(LogTemp, Error, TEXT("Mesh is nullptr"));
	}
	else if (!Mesh->SkeletalMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("Mesh->SkeletalMesh is nullptr"));
	}
	else if (!Mesh->AnimClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Mesh->AnimClass is nullptr"));
	}
	else if (!Mesh->IsRegistered())
	{
		UE_LOG(LogTemp, Error, TEXT("Mesh is not registered"));
	}
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

	//float HitAngle = CalculateHitAngle(ReactionData.HitPoint);

	//HitResponse Response = EvaluateHitResponse(ReactionData.Response, ReactionData.CanBlocked, ReactionData.CanParried, ReactionData.CanAvoid, HitAngle);
	if (ReactionData.Response == HitResponse::HitAir)
	{
		if(OnHitAirReaction.IsBound()) OnHitAirReaction.Broadcast();
		return;
	}

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