// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Components/AttackComponent.h"
#include "GameFramework/Character.h"

#include "Combat/Interfaces/HitReactionInterface.h"
#include "Combat/Interfaces/CombatInterface.h"
#include "Combat/Interfaces/AttackSourceInterface.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Engine/StaticMeshSocket.h"
#include "Utils/AnimBoneTransformDataAsset.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

#include "Utils/CoreLog.h"


#include "Characters/Player/PlayerBase.h"
#include "Characters/Components/EquipmentComponent.h"


// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (!Character) return;


	if (Character->GetClass()->ImplementsInterface(UAttackSourceInterface::StaticClass()))
	{
		AttackSourceInterface = TScriptInterface<IAttackSourceInterface>(Character);
	}
	else
	{
		for (UActorComponent* Comp : Character->GetComponents())
		{
			if (Comp->GetClass()->ImplementsInterface(UAttackSourceInterface::StaticClass()))
			{
				AttackSourceInterface = TScriptInterface<IAttackSourceInterface>(Comp);
				break;
			}
		}
	}
}

void UAttackComponent::ExecuteAttack(FName AttackName, float Playrate)
{
	bool CanPlayAttack = false;

	// 실행중인 공격이 있으면 실행중인 공격과 입력이 같은지 확인	
	if (CurAttackContext.AttackName == AttackName)
	{
		CanPlayAttack = true;
		// 다음 콤보가 있으면 ComboIndex증가 아니면 첫콤보로 초기화
		ComboIndex = CurAttackContext.AttackDetail.IsValidIndex(ComboIndex + 1) ? ComboIndex + 1 : 0;
	}
	else
	{
		if (CurAttackContextSet.Contexts.IsEmpty()) return;
		FAttackContext DataForFind;
		DataForFind.AttackName = AttackName;
		const FAttackContext* FoundData = CurAttackContextSet.Contexts.Find(DataForFind);
		CurAttackContext = FoundData ? *FoundData : FAttackContext{};

		if (CurAttackContext.Anim && !CurAttackContext.AttackDetail.IsEmpty())
		{
			CanPlayAttack = true;
			ComboIndex = 0;
		}
	}

	if (CanPlayAttack)
	{
		HitActorListCurrentAttack.Empty();
		PlayAnimation(CurAttackContext, ComboIndex, Playrate);
	}
}

void UAttackComponent::PlayAnimation(FAttackContext AttackInfo, int32 index, float Playrate)
{
	ACharacter* Char = Cast<ACharacter>(GetOwner());
	if (!Char) return;

	UAnimInstance* Anim = Char->GetMesh()->GetAnimInstance();
	if (!Anim) return;

	Anim->Montage_Play(AttackInfo.Anim, Playrate);
	Anim->Montage_JumpToSection(AttackInfo.AttackDetail[index].SectionName, AttackInfo.Anim);
	
	FOnMontageEnded MontageEndDelegate;
	MontageEndDelegate.BindUObject(this, &UAttackComponent::OnMontageEnded);
	Anim->Montage_SetEndDelegate(MontageEndDelegate, AttackInfo.Anim);
}

void UAttackComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ACharacter* Char = Cast<ACharacter>(GetOwner());
	if (!Char) return;

	UAnimInstance* Anim = Char->GetMesh()->GetAnimInstance();
	if (!Anim) return;

	FString EndMontage = Montage ? Montage->GetName() : TEXT("None");
	UAnimMontage* CurrentMontage = Anim ? Anim->GetCurrentActiveMontage() : nullptr;
	FString CurMontage = CurrentMontage ? CurrentMontage->GetName() : TEXT("None");

	if (!(EndMontage.Equals(CurMontage)))
	{
		ComboIndex = 0;
		CurAttackContext = FAttackContext();
	}

	Anim->OnMontageEnded.RemoveDynamic(this, &UAttackComponent::OnMontageEnded);
}

void UAttackComponent::DetectAttackTarget(float StartTime, float EndTime)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (!AttackSourceInterface.GetInterface() || !AttackSourceInterface.GetObject() || !Character) return;

	// 실행하는 공격 애니메이션의 본 트랜스폼 데이터 가져오기
	UAnimBoneTransformDataAsset* TraceTargetBoneTransformData = CurAttackContext.AttackDetail[ComboIndex].TargetBoneTransformDataAsset.Get();

	// 이전프레임과 현재프레임 사이를 0.001초 간격으로 나눔
	int32 TraceCorrectionCount = (EndTime - StartTime) / 0.003f;

	// 현재 루트본의 위치
	FTransform CurrentRootWorldTransform = Character->GetMesh()->GetBoneTransform(0);

	FAttackTraceSource TraceSource = IAttackSourceInterface::Execute_GetAttackTraceSource(AttackSourceInterface.GetObject(), CurAttackContext.AttackDetail[ComboIndex].AttackSource);

	FTransform TargetWeaponOffset = TraceSource.TraceComponent->GetRelativeTransform();

	const FVector StartSocketRelativeLocation = TraceSource.TraceComponent->GetSocketTransform(TEXT("Start"), RTS_Component).GetLocation();
	const FVector EndSocketRelativeLocation = TraceSource.TraceComponent->GetSocketTransform(TEXT("End"), RTS_Component).GetLocation();

	const float subDT = (EndTime - StartTime) / TraceCorrectionCount;

	for (int32 i = 1; i <= TraceCorrectionCount; ++i)
	{
		const float PrevTime = StartTime + (i * 0.003f);

		const FTransform BoneData = TraceTargetBoneTransformData->GetTransformAtTime(PrevTime);

		// 데이터(손) -> 월드(root) 적용
		const FTransform HandWorld = BoneData * CurrentRootWorldTransform;

		// 손월드 -> 무기월드 오프셋 적용
		const FTransform WeaponWorld = TargetWeaponOffset * HandWorld;

		// 소켓 로컬(무기 기준) -> 월드
		const FVector StartLoc = WeaponWorld.TransformPosition(StartSocketRelativeLocation);
		const FVector EndLoc = WeaponWorld.TransformPosition(EndSocketRelativeLocation);

		float CurWeaponLength = FVector::Distance(StartLoc, EndLoc);
		float CurHalfHeight = (CurWeaponLength * 0.5f);

		FVector CurCapsuleCenter = (StartLoc + EndLoc) * 0.5f;
		FVector CurCapsuleAxis = (EndLoc - StartLoc).GetSafeNormal();
		FQuat CurCapsuleRotation = FRotationMatrix::MakeFromZ(CurCapsuleAxis).ToQuat();

		TArray<FHitResult> HitResults;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());

		float Radius = TraceSource.Radius;

		FCollisionShape DetectShape = FCollisionShape::MakeCapsule(Radius, CurHalfHeight);

		bool bHit = GetWorld()->SweepMultiByChannel(
			HitResults,
			StartLoc,
			EndLoc,
			FQuat::Identity,
			ECC_GameTraceChannel3,
			DetectShape,
			CollisionParams
		);
		
		if (bHit)
		{
			for (const FHitResult& Result : HitResults)
			{
				AActor* HitActor = Result.GetActor();
				if (HitActor && !HitActorListCurrentAttack.Contains(HitActor))
				{
					HitActorListCurrentAttack.Add(HitActor);

					UE_LOG(Log_Attack, Log, TEXT("[AttackComponent] %s Attacked %s"), *Character->GetName(), *HitActor->GetName());

					if (HitActor->Implements<UHitReactionInterface>())
					{
						FAttackDamageSource DamageSource = IAttackSourceInterface::Execute_GetAttackDamageSource(AttackSourceInterface.GetObject());

						float OutDamage = DamageSource.AttackRating * CurAttackContext.AttackDetail[ComboIndex].DamageMultiplier;
						float OutPoiseDamage = DamageSource.PoiseRating * CurAttackContext.AttackDetail[ComboIndex].PoiseDamageMultiplier;
						float OutStanceDamage = DamageSource.StanceRating * CurAttackContext.AttackDetail[ComboIndex].StanceDamageMultiplier;
						EHitResponse OutResponse = CurAttackContext.AttackDetail[ComboIndex].Response;
						EDamageType OutAttackType = CurAttackContext.AttackDetail[ComboIndex].DamageType;
						FVector OutHitPoint = Result.ImpactPoint;
						FString OutHitPointName = Result.PhysMaterial.IsValid() ? Result.PhysMaterial->GetName() : FString();
						bool OutCanBlocked = CurAttackContext.AttackDetail[ComboIndex].CanBlocked;
						bool OutCanParried = CurAttackContext.AttackDetail[ComboIndex].CanParried;
						bool OutCanAvoid = CurAttackContext.AttackDetail[ComboIndex].CanAvoid;
						TArray<FStatusEffect> OutStatusEffect = CurAttackContext.AttackDetail[ComboIndex].StatusEffectList;

						FAttackRequest OutAttackData(
							OutDamage,
							OutStanceDamage,
							OutPoiseDamage,
							OutResponse,
							OutHitPoint,
							OutHitPointName,
							OutCanBlocked,
							OutCanParried,
							OutCanAvoid,
							OutStatusEffect
						);

						IHitReactionInterface::Execute_OnHit(HitActor, OutAttackData);

					}
				}
			}
		}
		
		DrawDebugCapsule(GetWorld(), CurCapsuleCenter, CurHalfHeight, Radius, CurCapsuleRotation, FColor::Red, false, 5.0f);
	}
}

void UAttackComponent::ResetTraceData()
{
}

FTransform UAttackComponent::SamplingBoneTransformAnimTime(UAnimSequence* AnimSequence, const FName& BoneName, float Time)
{
	if (!AnimSequence || !AnimSequence->GetSkeleton())
	{
		return FTransform::Identity;
	}

	USkeleton* Skeleton = AnimSequence->GetSkeleton();
	const FReferenceSkeleton& RefSkeleton = Skeleton->GetReferenceSkeleton();

	const int32 BoneIndex = RefSkeleton.FindBoneIndex(BoneName);
	if (BoneIndex == INDEX_NONE)
	{
		return FTransform::Identity;
	}

	// 0) 루트→타깃 체인 인덱스 만들기 (Skeleton index)
	TArray<int32> Chain;
	Chain.Reset();

	int32 Cur = BoneIndex;
	while (Cur != INDEX_NONE)
	{
		Chain.Add(Cur);
		Cur = RefSkeleton.GetParentIndex(Cur);
	}
	Algo::Reverse(Chain);

	// Root(0) 보장 (방어)
	if (Chain.Num() == 0 || Chain[0] != 0)
	{
		Chain.Insert(0, 0);
	}

	// 1) RequiredBoneIndices = 체인만
	TArray<FBoneIndexType> RequiredBoneIndices;
	RequiredBoneIndices.Reserve(Chain.Num());
	for (int32 SkelIdx : Chain)
	{
		RequiredBoneIndices.Add((FBoneIndexType)SkelIdx);
	}

	// 2) Curve 평가 옵션
	FCurveEvaluationOption CurveOption(false);

	// 3) FBoneContainer 생성 (체인만 포함)
	FBoneContainer RequiredBones;
	RequiredBones.InitializeTo(RequiredBoneIndices, CurveOption, *Skeleton);

	// 4) Pose 준비
	FMemMark Mark(FMemStack::Get());

	FCompactPose Pose;
	FBlendedCurve Curve;
	UE::Anim::FStackAttributeContainer Attributes;
	FAnimationPoseData PoseData(Pose, Curve, Attributes);

	Pose.SetBoneContainer(&RequiredBones);

	// 5) 애니 포즈 추출
	const float ClampedTime = FMath::Clamp(Time, 0.f, AnimSequence->GetPlayLength());
	FAnimExtractContext ExtractContext(ClampedTime, false);
	AnimSequence->GetAnimationPose(PoseData, ExtractContext);

	// 6) RefSkeleton 인덱스 공간에서 Local(Parent Space) 배열을 준비
	//    - 체인만 평가했으니, 체인에 필요한 본들의 local만 채우면 됨
	//    - 부모가 체인에 포함되어 있으므로 누적 가능
	TArray<FTransform> LocalRefSpace;
	LocalRefSpace.SetNum(Chain.Num()); // 기본은 RefPose
	// (메모리 줄이고 싶으면 체인 본만 따로 담아도 되지만, 여기선 단순하게 유지)

	// Pose(compact) -> Skeleton index 매핑해서 LocalRefSpace에 덮어쓰기
	for (FCompactPoseBoneIndex CompactIdx(0); CompactIdx < Pose.GetNumBones(); ++CompactIdx)
	{
		const int32 SkelIdx = RequiredBones.GetSkeletonIndex(CompactIdx);
		if (SkelIdx != INDEX_NONE && LocalRefSpace.IsValidIndex(SkelIdx))
		{
			LocalRefSpace[SkelIdx] = Pose[CompactIdx];
		}
	}

	// 7) Root->Target 체인만 누적해서 Component Space(BoneCS) 계산
	//    RootCS는 루트 local과 동일(보통)
	const int32 RootIdx = 0;
	FTransform RootCS = LocalRefSpace[RootIdx];

	if (Chain.Num() <= 1)
	{
		// Bone == Root
		return FTransform::Identity;
	}

	FTransform ParentCS = RootCS;
	FTransform BoneCS = RootCS;

	for (int32 i = 1; i < Chain.Num(); ++i)
	{
		const int32 SkelIdx = Chain[i];
		const FTransform& Local = LocalRefSpace[SkelIdx];

		// ChildCS = Local * ParentCS
		BoneCS = Local * ParentCS;
		ParentCS = BoneCS;
	}

	// 8) 루트 기준 상대 Transform 반환
	return BoneCS.GetRelativeTransform(RootCS);
}
