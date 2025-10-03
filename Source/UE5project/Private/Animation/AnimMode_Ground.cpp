#include "Animation/AnimMode_Ground.h"
#include "Characters/Player/CharacterBase.h"
#include "Characters/Player/CharacterBaseAnimInstance.h"
#include "Characters/Components/BaseCharacterMovementComponent.h"
#include "Characters/Components/CharacterStatusComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UAnimMode_Ground::OnModeEnter()
{
	bFirstTick = true;
	if (auto* Ch = Character.Get())
		PrevYaw = Ch->GetActorRotation().Yaw;
}

float UAnimMode_Ground::ComputeLeanYawPerSecond(float DeltaSeconds)
{
	auto* Ch = Character.Get();
	if (!Ch || DeltaSeconds <= KINDA_SMALL_NUMBER) return 0.f;
	const float CurYaw = Ch->GetActorRotation().Yaw;
	const float YawDelta = CurYaw - PrevYaw;
	PrevYaw = CurYaw;
	const float YawPerSec = UKismetMathLibrary::SafeDivide(YawDelta, DeltaSeconds);
	return YawPerSec * LeanScale; // = Direction:contentReference[oaicite:4]{index=4}
}

TTuple<FVector, float> UAnimMode_Ground::FootTrace(
	USkeletalMeshComponent* Mesh, UCapsuleComponent* Capsule, FName Socket)
{
	// 원 FootTrace 로직을 모드로 이동:contentReference[oaicite:5]{index=5}
	const FVector FootLoc = Mesh->GetSocketLocation(Socket);
	const FVector Start(FootLoc.X, FootLoc.Y, Mesh->GetOwner()->GetActorLocation().Z);
	const FVector End = Start - FVector(0, 0, Capsule->GetScaledCapsuleHalfHeight() + TraceDistance);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.bTraceComplex = true;

	// 속도 기준 채널 선택은 Anim에서 하던 방식 유지 가능(여기서는 Visibility로 통일하거나 필요시 주입)
	const ECollisionChannel Channel = ECC_Visibility;

	const bool bHit = Mesh->GetWorld()->LineTraceSingleByChannel(Hit, Start, End, Channel, Params);
	if (bHit)
	{
		const float Offset = ((Hit.ImpactPoint - Hit.TraceEnd).Length()) - (TraceDistance - 3.f);
		return MakeTuple(Hit.ImpactNormal, Offset);
	}
	return MakeTuple(FVector::ZeroVector, 0.f);
}

void UAnimMode_Ground::FootRotation(float DeltaSeconds, const FVector& TargetNormal, FRotator& InOutRot)
{
	const float AtanX = -1.f * UKismetMathLibrary::DegAtan2(TargetNormal.X, TargetNormal.Z);
	const float AtanY = UKismetMathLibrary::DegAtan2(TargetNormal.Y, TargetNormal.Z);
	const FRotator Target(AtanX, 0.f, AtanY);
	InOutRot = UKismetMathLibrary::RInterpTo(InOutRot, Target, DeltaSeconds, RotInterpSpeed);
}

void UAnimMode_Ground::UpdateFootIK(float DeltaSeconds, float Speed)
{
	auto* Ch = Character.Get();
	auto* Anim = AnimInst.Get();
	if (!Ch || !Anim) return;

	auto* Mesh = Ch->GetMesh();
	auto* Capsule = Ch->GetCapsuleComponent();

	const auto L = FootTrace(Mesh, Capsule, FName("Foot_L"));
	const auto R = FootTrace(Mesh, Capsule, FName("Foot_R"));

	FootRotation(DeltaSeconds, L.Key, Anim->LeftFootRotator);
	FootRotation(DeltaSeconds, R.Key, Anim->RightFootRotator);

	Anim->PelvisOffset = FMath::FInterpTo(
		Anim->PelvisOffset,
		UKismetMathLibrary::Min(L.Value, R.Value), DeltaSeconds, IKInterpSpeed);

	Anim->LeftFootGroundOffset.Z = FMath::FInterpTo(Anim->LeftFootGroundOffset.Z, L.Value - Anim->PelvisOffset, DeltaSeconds, IKInterpSpeed);
	Anim->RightFootGroundOffset.Z = FMath::FInterpTo(Anim->RightFootGroundOffset.Z, R.Value - Anim->PelvisOffset, DeltaSeconds, IKInterpSpeed);

	// 필요 시 X 오프셋 이동(원 코드 Speed>100 조건)은 여기서 처리 가능:contentReference[oaicite:8]{index=8}
}

void UAnimMode_Ground::Tick(float DeltaSeconds)
{
	if (!Character.IsValid() || !AnimInst.IsValid()) return;
	auto* Ch = Character.Get();
	auto* Anim = AnimInst.Get();

	// 상태/스탠스
	//Anim->CurGroundStance = Ch->GetCurGroundStance();                 // :contentReference[oaicite:9]{index=9}
	Anim->CharacterGroundState = Ch->GetCharacterStatusComponent()->GetGroundStance_Native();

	FVector WorldAcceleration = Character->GetCharacterMovement()->GetCurrentAcceleration() * FVector(1.0f, 1.0f, 0.0f);
	FVector LocalAcceleration = Character->GetActorRotation().UnrotateVector(WorldAcceleration);
	float AccelerationLength = LocalAcceleration.SizeSquared();
	Anim->IsAccelerating = !FMath::IsNearlyZero(AccelerationLength);

	// 이동량
	Anim->Speed = Ch->GetVelocity().Length();                     // :contentReference[oaicite:10]{index=10}
	if (bFirstTick) { OnModeEnter(); bFirstTick = false; }
	Anim->Direction = ComputeLeanYawPerSecond(DeltaSeconds);                    // ← GetAnimDirection 대체

	// 점프/낙하/착지 판정
	Anim->IsInAir = Ch->GetMovementComponent()->IsFalling();          // :contentReference[oaicite:11]{index=11}
	Anim->IsJumping = Anim->IsFalling = Anim->IsLanding = false;
	if (Anim->IsInAir)
	{
		Ch->GetVelocity().Z > 0 ? Anim->IsJumping = true : Anim->IsFalling = true; // :contentReference[oaicite:12]{index=12}
		if (auto* Move = Cast<UBaseCharacterMovementComponent>(Ch->GetMovementComponent()))
			Anim->IsLanding = (Move->GetGroundDistance(false) < 50.f);             // :contentReference[oaicite:13]{index=13}
	}

	// 런/조그 블렌드, 가속/퀵턴, 등
	Anim->MovementAlpha = FMath::GetRangePct(400.f, 600.f, Anim->Speed);          // :contentReference[oaicite:14]{index=14}

	// 발 IK
	UpdateFootIK(DeltaSeconds, Anim->Speed);
}