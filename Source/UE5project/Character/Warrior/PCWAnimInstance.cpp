// Fill out your copyright notice in the Description page of Project Settings.


#include "PCWAnimInstance.h"
#include "PCWarrior.h"
#include "GameFramework/PawnMovementComponent.h"

UPCWAnimInstance::UPCWAnimInstance()
{
	CurrentSpeed = 0.0f;
	IsInAir = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Character/C_Animation/CA_Warrior/CAW_BP/CAW_AttackMontage.CAW_AttackMontage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UPCWAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<APCWarrior>(Pawn);
		if (Character)
		{
			Speed = Character->GetVelocity().Size();
			Direction = (Warrior_AnimInstance->CalculateDirection(Character->GetVelocity(), Character->GetActorRotation()));
			IsInAir = Character->GetMovementComponent()->IsFalling();
			Pitch = Character->GetBaseAimRotation().Pitch;
			Yaw = Character->GetBaseAimRotation().Yaw;

			/*
			if (Montage_IsPlaying(AttackMontage))
			{
				if (PrevLoc == NextLoc)
				{
					//������ �߾���ǥ�� ������ ����
					PrevLoc = Character->GetActorLocation() - Character->GetMesh()->GetBoneTransform(0).GetLocation();
				}
				else
				{
					//������ ��ǥ�� ���
					PrevLoc = NextLoc;
					//������ �߾���ǥ�� ������ ����
					NextLoc = Character->GetActorLocation() - Character->GetMesh()->GetBoneTransform(0).GetLocation();

					Character->SetActorLocation(Character->GetActorLocation() + PrevLoc - NextLoc);
				}
			}
			*/
		}
	}
}

void UPCWAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}