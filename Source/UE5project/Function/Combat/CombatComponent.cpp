// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "GameFramework/Character.h"
#include "PBDamagableInterface.h"
#include "CombatInterface.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	AnimInstance = GetOwner()->GetComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();
}

// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent::SetAttackDT(UDataTable* InitDialogueDT)
{
	AttackListDT = InitDialogueDT;
}

void UCombatComponent::AnalysisAttackData(FName RowName, FName StartSection)
{
	if (!AttackListDT)
		return;

	CurAttackDTRow = AttackListDT->FindRow<FAttackDataStruct>(RowName, "");
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

void UCombatComponent::ExecuteAttack(FName SectionName)
{
	//AnimInstance->Montage_Play();

	FAttackData DataForFind;
	DataForFind.SectionName = SectionName;
	CurAttack = CurAttackDTRow->AttackDataList.Find(DataForFind);
	const FAttackData* FoundData = CurAttack;

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

		OnMontageEndedDelegate.BindUObject(this, &UCombatComponent::OnMontageEnded, FoundData);
		AnimInstance->Montage_SetBlendingOutDelegate(OnMontageEndedDelegate);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UCombatComponent::OnMontageNotifyBegin);
		AnimInstance->OnPlayMontageNotifyEnd.AddDynamic(this, &UCombatComponent::OnMontageNotifyEnd);
		//UE_LOG(LogTemp, Warning, TEXT("Cur Attack : %s"), *CurAttack->SectionName.ToString());
	
		if (FoundData->NeedMotionWarp)
		{
			OnMotionWarp.ExecuteIfBound();
		}
	}
	else
	{
		FName CheckSection = SectionName;
		FString CheckString = CheckSection.ToString();
		UE_LOG(LogTemp, Warning, TEXT("Failed to Find Section: %s"), *CheckString);
	}
}

void UCombatComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted, const FAttackData* CurAnimData)
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

void UCombatComponent::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	/*
	UE_LOG(LogTemp, Warning, TEXT("Sequence Asset: %s"), *BranchingPointPayload.SequenceAsset->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Sequence Asset: %s"), *CurAttackDTRow->ID.ToString());
	if (*BranchingPointPayload.SequenceAsset->GetName().TrimStartAndEnd().ToLower() == *CurAttackDTRow->ID.ToString().TrimStartAndEnd().ToLower())
	{
		UE_LOG(LogTemp, Warning, TEXT("String Equal"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("String NOT Equal"));
		*/

	if (BranchingPointPayload.SequenceAsset)
	{
		if (NotifyName == FName("WeaponAttack"))
		{
			if (GetOwner()->GetClass()->ImplementsInterface(UCombatInterface::StaticClass()))
			{
				UE_LOG(LogTemp, Warning, TEXT("WeaponAttack"));
				UStaticMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(GetOwner());
				Weapon->SetGenerateOverlapEvents(true);
				Weapon->OnComponentBeginOverlap.AddDynamic(this, &UCombatComponent::TriggerBegin);
				/*
				GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, [this]() {
					UStaticMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(GetOwner());
					FVector StartLoc = Weapon->GetSocketLocation("Start");
					FVector EndLoc = Weapon->GetSocketLocation("End");
					Detect_Sphere(StartLoc, EndLoc, 20.0f);
					}, 0.01f, true);
					*/
			}
			else
				UE_LOG(LogTemp, Warning, TEXT("Attack Range is Not Set"));
		}
		/*
		else if (NotifyName == FName("Slash"))
		{
			FAttackInfo DataForFind;
			DataForFind.FeatureID = NotifyName;
			const FAttackInfo SlashInfo = *CurAttack->Feature.Find(DataForFind);
			Detect_Circular(SlashInfo, GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * 150.0f) - FVector(0.0f,0.0f,90.0f), GetOwner()->GetActorForwardVector(), -GetOwner()->GetActorUpVector(), 0.0f, 30.0f, 1050.0f, 10, 1, 30.0f);
		}
		else if (NotifyName == FName("Smash"))
		{
			ACharacter* CurChar = Cast<ACharacter>(GetOwner());
			CurChar->GetCharacterMovement()->GravityScale = 5.0f;
			CurChar->JumpMaxHoldTime = 0.2f;
			UE_LOG(LogTemp, Warning, TEXT("Notify Smash"));
		}
		*/
	}
}

void UCombatComponent::OnMontageNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	//UE_LOG(LogTemp, Warning, TEXT("Notify End"));
	if (NotifyName == FName("WeaponAttack"))
	{
		UStaticMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(GetOwner());
		Weapon->SetGenerateOverlapEvents(false);
		Weapon->OnComponentBeginOverlap.RemoveDynamic(this, &UCombatComponent::Detect_Collision);
		//GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	}
	/*
	else if (NotifyName == FName("Slash"))
	{

	}
	else if (NotifyName == FName("Smash"))
	{
		ACharacter* CurChar = Cast<ACharacter>(GetOwner());
		CurChar->GetCharacterMovement()->GravityScale = 1.0f;
		CurChar->JumpMaxHoldTime = 0.0f;
		UE_LOG(LogTemp, Warning, TEXT("Notify Smash End"));
	}
	*/
}


void UCombatComponent::Detect_Collision(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap Detect"));
	if (OtherActor->GetClass()->ImplementsInterface(UPBDamagableInterface::StaticClass()))
	{
		IPBDamagableInterface* GetDamagedActor = Cast<IPBDamagableInterface>(OtherActor);
		//UE_LOG(LogTemp, Warning, TEXT("Damagable Actor"));
		//GetDamagedActor->TakeDamage_Implementation(CurAttack->Feature);
	}
}

void UCombatComponent::Detect_LineTrace(FAttackInfo AttackFeature, FVector StartLoc, FVector EndLoc, bool IsDrawLine)
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
		if (HitResult.GetActor()->GetClass()->ImplementsInterface(UPBDamagableInterface::StaticClass()))
		{
			IPBDamagableInterface* GetDamagedActor = Cast<IPBDamagableInterface>(HitResult.GetActor());
			//UE_LOG(LogTemp, Warning, TEXT("Damagable Actor"));
			GetDamagedActor->TakeDamage_Implementation(AttackFeature);
		}
	}
}

void UCombatComponent::Detect_Circular(FName AttackName, FVector Center, FVector Direction, FVector VerticalVector, float StartAngle, float EndAngle, float Radius, int TraceNum, bool IsDrawLine)
{
	FAttackInfo DataForFind;
	DataForFind.FeatureID = AttackName;
	const FAttackInfo* AttackFeature = CurAttack->Feature.Find(DataForFind);
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

void UCombatComponent::Detect_Circular(FAttackInfo AttackFeature, FVector Center, FVector Direction, FVector VerticalVector, float StartAngle, float EndAngle, float Radius, int TraceNum, bool IsDrawLine)
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

void UCombatComponent::Detect_Sphere(FVector StartLoc, FVector EndLoc, float Radius)
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
		if (HitResult.GetActor()->GetClass()->ImplementsInterface(UPBDamagableInterface::StaticClass()))
		{
			//IPBDamagableInterface::Execute_TakeDamage(HitResult.GetActor(), CurAttack->Feature);
		}
	}
}

void UCombatComponent::Detect_Capsule(FVector StartLoc, FVector EndLoc, FVector Extent)
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
		FCollisionShape::MakeCapsule(Extent),
		CollisionParams
	);

	if (bHit)
	{
		
	}
}

void UCombatComponent::Detect_Box(FVector StartLoc, FVector EndLoc, FVector HalfExtent)
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

void UCombatComponent::InflictDamage(AActor* Target)
{
}

void UCombatComponent::SetDashDistance()
{
}

void UCombatComponent::SetWeaponData(FVector Start, FVector End, FVector Add)
{
	CurRange.RangeBegin = Start;
	CurRange.RangeEnd = End;
	CurRange.AddValue = Add;
}

void UCombatComponent::TriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor!=GetOwner())
	{
		if (OtherActor->GetClass()->ImplementsInterface(UPBDamagableInterface::StaticClass()))
		{
			FAttackInfo DataForFind;
			DataForFind.FeatureID = "WeaponAttack";
			IPBDamagableInterface::Execute_TakeDamage(OtherActor, *CurAttack->Feature.Find(DataForFind));
			/*
			IPBDamagableInterface* GetDamagedActor = Cast<IPBDamagableInterface>(OtherActor);
			GetDamagedActor->TakeDamage_Implementation(CurAttack->Feature);
			*/
		}
	}
}