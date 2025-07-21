// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Components/AttackComponent.h"
#include "GameFramework/Character.h"
#include "Combat/Interfaces/HitReactionInterface.h"
#include "Combat/Interfaces/CombatInterface.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedCharacter = Cast<ACharacter>(GetOwner());

	if (!CheckOwnerExist())
		return;

	for (UActorComponent* Comp : CachedCharacter->GetComponents())
	{
		if (Comp->GetClass()->ImplementsInterface(UEquipmentDataInterface::StaticClass()))
		{
			CachedEquipment = TScriptInterface<IEquipmentDataInterface>(Comp);
			break;
		}
	}

	// ...
	AnimInstance = GetOwner()->GetComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UAttackComponent::OnMontageNotifyBegin);
		AnimInstance->OnPlayMontageNotifyEnd.AddDynamic(this, &UAttackComponent::OnMontageNotifyEnd);
	}
}

// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*
	if (IsDetectAttackTarget)
	{
		UStaticMeshComponent* WeaponMeshComponent = IEquipmentDataInterface::Execute_GetMainWeaponMeshComponent(CachedEquipment.GetObject());
		FWeaponPartInfo WeaponInfo = IEquipmentDataInterface::Execute_GetMainWeaponData(CachedEquipment.GetObject());

		if (WeaponMeshComponent != nullptr)
		{
			DetectAttackTarget(WeaponMeshComponent, WeaponInfo);
		}
	}
	*/
	// ...
}

bool UAttackComponent::ReCastOwner()
{
	CachedCharacter = Cast<ACharacter>(GetOwner());

	return CachedCharacter.IsValid() ? true : false;
}

bool UAttackComponent::CheckOwnerExist()
{
	bool IsExist = true;

	if (!CachedCharacter.IsValid())
	{
		IsExist = ReCastOwner();
	}

	return IsExist;
}

ACharacter* UAttackComponent::GetMyCharacter()
{
	if (!CachedCharacter.IsValid())
	{
		return CachedCharacter.Get();
	}

	return nullptr;
}

void UAttackComponent::SetAttackDT(const UDataTable* AttackDT)
{
	AttackListDT = AttackDT;
}

void UAttackComponent::AnalysisAttackData(FName RowName, FName StartSection)
{
	if (!AttackListDT)
		return;

	CurAttackDTRow = AttackListDT->FindRow<FAttackInfoList>(RowName, "");
	if (CurAttackDTRow == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("DT Not Found"));
		return;
	}

	ExecuteAttack(FName("Start"));

	/*
	if (StartSection == FName("None"))
		ExecuteAttack(CurAttackDTRow->AttackDataList.begin()->SectionName);
	else
		ExecuteAttack(StartSection);
		*/
}

void UAttackComponent::ExecuteAttack(FName SectionName, float Playrate)
{
	//AnimInstance->Montage_Play();

	FAttackInfo DataForFind;
	DataForFind.SectionName = SectionName;
	CurAttack = CurAttackDTRow->AttackInfo.Find(DataForFind);
	const FAttackInfo* FoundData = CurAttack;

	if (FoundData != nullptr)
	{
		ACharacter* CurChar = Cast<ACharacter>(GetOwner());
		if (CurChar && CurChar->GetCharacterMovement())
		{
			FoundData->IsJumpAttack ? CurChar->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying) : CurChar->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}
		AnimInstance->Montage_Play(FoundData->Anim);
		//AnimInstance->PlayMontage(CurAttackDTRow->ID);
		AnimInstance->Montage_JumpToSection(FoundData->SectionName);
		//UE_LOG(LogTemp, Warning, TEXT("Current Section : %s"), *FoundData->SectionName.ToString());

		if(OnMontageEndedDelegate.IsBound())
			OnMontageEndedDelegate.Unbind();

		OnMontageEndedDelegate.BindUObject(this, &UAttackComponent::OnMontageEnded, FoundData);
		AnimInstance->Montage_SetBlendingOutDelegate(OnMontageEndedDelegate);
		//AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UAttackComponent::OnMontageNotifyBegin);
		//AnimInstance->OnPlayMontageNotifyEnd.AddDynamic(this, &UAttackComponent::OnMontageNotifyEnd);
		//UE_LOG(LogTemp, Warning, TEXT("Cur Attack : %s"), *CurAttack->SectionName.ToString());
	}
	else
	{
		FName CheckSection = SectionName;
		FString CheckString = CheckSection.ToString();
		UE_LOG(LogTemp, Warning, TEXT("Failed to Find Section: %s"), *CheckString);
	}
}

void UAttackComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted, const FAttackInfo* CurAnimData)
{
	// 몽타주가 종료되었을 때의 처리
	if (!(CurAnimData->Anim == Montage))
		return;

	if (CurAnimData->NextIDList.IsEmpty())
	{
		ACharacter* CurChar = Cast<ACharacter>(GetOwner());
		if (CurChar && CurChar->GetCharacterMovement())
		{
			CurChar->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}
		// 공격 끝(비헤이비어트리에 끝낫다고 전달)
	}
	else
	{
		//CheckConditon(현재 상황 확인하고 그에 맞는 공격 실행)
		/*
		* FName Condition = CheckCondition();
		* const NextIDList NextAttackID = CurAnimData->NextIDList.Find(Condition);
		* ExecuteAttack(NextAttackID->NextID);
		*/
		//UE_LOG(LogTemp, Warning, TEXT("Next Attack Exist"));
		ExecuteAttack(CurAnimData->NextIDList.begin()->NextID);
	}
}

void UAttackComponent::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (BranchingPointPayload.SequenceAsset)
	{
		if (NotifyName == FName("WeaponAttack"))
		{
			if (CachedEquipment.GetInterface() && CachedEquipment.GetObject())
			{
				IsDetectAttackTarget = true;
				UStaticMeshComponent* WeaponMeshComponent = IEquipmentDataInterface::Execute_GetMainWeaponMeshComponent(CachedEquipment.GetObject());
				FWeaponSetsInfo WeaponInfo = IEquipmentDataInterface::Execute_GetWeaponSetsData(CachedEquipment.GetObject());

				if (WeaponMeshComponent != nullptr)
				{
					float AnimSectionTime = AnimInstance->GetCurveValue(FName("AnimSectionTime"));
					float StartTime = BranchingPointPayload.NotifyEvent->GetTriggerTime() - AnimSectionTime;
					float EndTime = BranchingPointPayload.NotifyEvent->GetEndTriggerTime() - AnimSectionTime;
					UE_LOG(LogTemp, Log, TEXT("NotifyState: %s, Start: %f, End: %f"),
						*BranchingPointPayload.NotifyEvent->NotifyName.ToString(),
						StartTime,
						EndTime);

					FTimerDelegate AttackTimerDelegate;
					AttackTimerDelegate.BindUObject(this, &UAttackComponent::DetectAttackTarget, WeaponMeshComponent, WeaponInfo, StartTime, EndTime, false);
					GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, AttackTimerDelegate, 0.01f, true);
				}
			}
		}
	}
}

void UAttackComponent::OnMontageNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName("WeaponAttack"))
	{
		DetectTracePrevTime.Key = false;
		UE_LOG(LogTemp, Warning, TEXT("Reset Detect"));
		HitActorListCurrentAttack.Empty();
		if(GetWorld()->GetTimerManager().IsTimerActive(AttackTimerHandle))
			GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	}
}

void UAttackComponent::DetectAttackTarget(UStaticMeshComponent* WeaponMesh, FWeaponSetsInfo WeaponInfo, float StartTime, float EndTime, bool IsSubWeaponAttack)
{
	/*
	FVector StartLoc = WeaponMesh->GetSocketLocation("Start");
	FVector EndLoc = WeaponMesh->GetSocketLocation("End");
	float Radius = WeaponInfo.HitBoxRadius;

	float CurWeaponLength = FVector::Distance(StartLoc, EndLoc);
	float CurHalfHeight = (CurWeaponLength * 0.5f);

	FVector CurCapsuleCenter = (StartLoc + EndLoc) * 0.5f;
	FVector CurCapsuleAxis = (EndLoc - StartLoc).GetSafeNormal();
	FQuat CurCapsuleRotation = FRotationMatrix::MakeFromZ(CurCapsuleAxis).ToQuat();

	//DrawDebugCapsule(GetWorld(), CurCapsuleCenter, CurHalfHeight, Radius, CurCapsuleRotation, FColor::Green, false, 3.0f);

	UE_LOG(LogTemp, Warning, TEXT("DetectTarget"));
	
	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	float WeaponLength = FVector::Distance(StartLoc, EndLoc);
	float HalfHeight = (WeaponLength * 0.5f);

	FCollisionShape DetectShape = FCollisionShape::MakeCapsule(Radius, HalfHeight);

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		StartLoc,
		EndLoc,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		DetectShape,
		CollisionParams
	);
	*/
}


void UAttackComponent::Detect_Collision(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap Detect"));
	if (OtherActor->GetClass()->ImplementsInterface(UHitReactionInterface::StaticClass()))
	{
		IHitReactionInterface* GetDamagedActor = Cast<IHitReactionInterface>(OtherActor);
		//UE_LOG(LogTemp, Warning, TEXT("Damagable Actor"));
		//GetDamagedActor->TakeDamage_Implementation(CurAttack->Feature);
	}
}

void UAttackComponent::Detect_LineTrace(FAttackFeature AttackFeature, FVector StartLoc, FVector EndLoc, bool IsDrawLine)
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLoc,
		EndLoc,
		ECC_GameTraceChannel3,
		CollisionParams
	);

	if (IsDrawLine)
	{
		FColor DrawColor = bHit ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), StartLoc, EndLoc, DrawColor, false, 1.0f);
	}

	if (bHit)
	{
		if (HitResult.GetActor()->GetClass()->ImplementsInterface(UHitReactionInterface::StaticClass()))
		{
			IHitReactionInterface* GetDamagedActor = Cast<IHitReactionInterface>(HitResult.GetActor());
			//UE_LOG(LogTemp, Warning, TEXT("Damagable Actor"));
			//GetDamagedActor->TakeDamage_Implementation(AttackFeature);
		}
	}
}

void UAttackComponent::Detect_Circular(FName AttackName, FVector Center, FVector Direction, FVector VerticalVector, float StartAngle, float EndAngle, float Radius, int TraceNum, bool IsDrawLine)
{
	FAttackFeature DataForFind;
	DataForFind.FeatureID = AttackName;
	const FAttackFeature* AttackFeature = CurAttack->Feature.Find(DataForFind);
	if (AttackFeature == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed To Find AttackFeature With Name : %s"), *AttackName.ToString());
		return;
	}

	FVector ForwardVector = Direction.GetSafeNormal();
	FVector RightVector = FVector::CrossProduct(VerticalVector, ForwardVector).GetSafeNormal();
	for (int i = 0; i <= TraceNum; i++)
	{
		float Angle = FMath::Lerp(StartAngle, EndAngle, i / (float)TraceNum);
		FVector RotatedDirection = ForwardVector.RotateAngleAxis(Angle, RightVector);
		FVector EndLocation = Center + (RotatedDirection * Radius);
		//FVector EndLocation = Center + (ForwardVector * FMath::Cos(FMath::DegreesToRadians(Angle)) * Radius) + (FVector::RightVector * FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius);
		//FVector EndLocation = Center + (Direction * Radius) + (FVector::UpVector * FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius);
		IsDrawLine ? Detect_LineTrace(*AttackFeature, Center, EndLocation, true) : Detect_LineTrace(*AttackFeature, Center, EndLocation);
		/*
		for (int j = -FanNum; j <= FanNum; j++)
		{
			FVector StartLocation = CharLocation + RightVector * (FanOffset * j);
			FVector EndLocation = StartLocation + (ForwardVector * Radius) + (FVector::UpVector * FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius);
		}
		*/
	}
}

void UAttackComponent::Detect_Circular(FAttackFeature AttackFeature, FVector Center, FVector Direction, FVector VerticalVector, float StartAngle, float EndAngle, float Radius, int TraceNum, bool IsDrawLine)
{
	FVector ForwardVector = Direction.GetSafeNormal();
	FVector RightVector = FVector::CrossProduct(VerticalVector, ForwardVector).GetSafeNormal();
	for (int i = 0; i <= TraceNum; i++)
	{
		float Angle = FMath::Lerp(StartAngle, EndAngle, i / (float)TraceNum);
		FVector RotatedDirection = ForwardVector.RotateAngleAxis(Angle, RightVector);
		FVector EndLocation = Center + (RotatedDirection * Radius);
		//FVector EndLocation = Center + (ForwardVector * FMath::Cos(FMath::DegreesToRadians(Angle)) * Radius) + (FVector::RightVector * FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius);
		//FVector EndLocation = Center + (Direction * Radius) + (FVector::UpVector * FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius);
		Detect_LineTrace(AttackFeature, Center, EndLocation);
		/*
		for (int j = -FanNum; j <= FanNum; j++)
		{
			FVector StartLocation = CharLocation + RightVector * (FanOffset * j);
			FVector EndLocation = StartLocation + (ForwardVector * Radius) + (FVector::UpVector * FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius);
		}
		*/
	}
}

void UAttackComponent::Detect_Sphere(FVector StartLoc, FVector EndLoc, float Radius)
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLoc,
		EndLoc,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(Radius),
		CollisionParams
	);

	/*
	FVector TraceVec = EndLoc - StartLoc;
	FVector Center = StartLoc + TraceVec * 0.5f;
	float HalfHeight = FVector::Dist(StartLoc, EndLoc) * 0.5f;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bHit ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(
		GetWorld(),
		Center,
		HalfHeight,
		Radius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);
	*/

	if (bHit)
	{
		if (HitResult.GetActor()->GetClass()->ImplementsInterface(UHitReactionInterface::StaticClass()))
		{
			//IHitReactionInterface::Execute_TakeDamage(HitResult.GetActor(), CurAttack->Feature);
		}
	}
}

void UAttackComponent::Detect_Capsule(FVector StartLoc, FVector EndLoc, float Radius, float Padding)
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Red, false, 1.0f);

	float WeaponLength = FVector::Distance(StartLoc, EndLoc);
	float HalfHeight = (WeaponLength * 0.5f) + Padding;

	FCollisionShape DetectShape = FCollisionShape::MakeCapsule(Radius, HalfHeight);

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLoc,
		EndLoc,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		DetectShape,
		CollisionParams
	);

	if (bHit)
	{
		
	}
}

void UAttackComponent::Detect_Box(FVector StartLoc, FVector EndLoc, FVector HalfExtent)
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Red, false, 1.0f);

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLoc,
		EndLoc,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		FCollisionShape::MakeBox(HalfExtent),
		CollisionParams
	);

	if (bHit)
	{

	}
}

void UAttackComponent::InflictDamage(AActor* Target)
{
}

void UAttackComponent::SetDashDistance()
{
}

void UAttackComponent::SetWeaponData(FVector Start, FVector End, FVector Add)
{
	//CurRange.RangeBegin = Start;
	//CurRange.RangeEnd = End;
	//CurRange.AddValue = Add;
}

void UAttackComponent::TriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor!=GetOwner())
	{
		if (OtherActor->GetClass()->ImplementsInterface(UHitReactionInterface::StaticClass()))
		{
			FAttackFeature DataForFind;
			DataForFind.FeatureID = "WeaponAttack";
			//IHitReactionInterface::Execute_TakeDamage(OtherActor, *CurAttack->Feature.Find(DataForFind));
			/*
			IHitReactionInterface* GetDamagedActor = Cast<IHitReactionInterface>(OtherActor);
			GetDamagedActor->TakeDamage_Implementation(CurAttack->Feature);
			*/
		}
	}
}