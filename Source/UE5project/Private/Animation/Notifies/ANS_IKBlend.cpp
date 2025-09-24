// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notifies/ANS_IKBlend.h"
#include "Animation/Interfaces/IAnimInstance.h"

void UANS_IKBlend::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Anim, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    if (MeshComp && MeshComp->GetAnimInstance() && MeshComp->GetAnimInstance()->GetClass()->ImplementsInterface(UIAnimInstance::StaticClass()))
    {
        TotalLen = FMath::Max(TotalDuration, KINDA_SMALL_NUMBER);
        Elapsed = 0.f;

        for (const auto& R : LimbData)
        {
            if (!R.bInitAlphaValue)
                continue;

            EIKContext FromContext = R.From;
            EIKContext ToContext = R.To;
            float TargetAlpha = R.bAlphaToZero ? 1.0f : 0.0f;

            if (FromContext == ToContext)
            {
                IIAnimInstance::Execute_SetIKWeight(MeshComp->GetAnimInstance(), ToContext, R.TargetLimb, TargetAlpha);
            }
            else
            {
                IIAnimInstance::Execute_SetIKWeight(MeshComp->GetAnimInstance(), ToContext, R.TargetLimb, TargetAlpha);
                IIAnimInstance::Execute_SetIKWeight(MeshComp->GetAnimInstance(), FromContext, R.TargetLimb, 1.f - TargetAlpha);
            }

        }
    }
}

void UANS_IKBlend::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Anim, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
    Elapsed += FrameDeltaTime;

    if (MeshComp && MeshComp->GetAnimInstance() && MeshComp->GetAnimInstance()->GetClass()->ImplementsInterface(UIAnimInstance::StaticClass()))
    {
        for (const auto& R : LimbData)
        {
            if (const FAnimNotifyEvent* Notify = EventReference.GetNotify())
            {
                EIKContext FromContext = R.From;
                EIKContext ToContext = R.To;

                const float DeltaStartTime = R.StartTime - Notify->GetTriggerTime();
                const float ElapsedFromStart = Elapsed - DeltaStartTime;
                const float BlendDuration = FMath::Max(R.EndTime - R.StartTime, KINDA_SMALL_NUMBER);
                const float ElpasedRatio = FMath::Clamp(ElapsedFromStart / BlendDuration, 0.0f, 1.0f);
                const float OutAlpha = !R.bAlphaToZero ? ElpasedRatio : 1 - ElpasedRatio;

                const float CurrentTargetAlpha = IIAnimInstance::Execute_GetIKWeight(MeshComp->GetAnimInstance(), ToContext, R.TargetLimb);
                if (!R.bAlphaToZero ? OutAlpha < CurrentTargetAlpha : OutAlpha > CurrentTargetAlpha)
                    return;

                if (FromContext == ToContext)
                {
                    IIAnimInstance::Execute_SetIKWeight(MeshComp->GetAnimInstance(), ToContext, R.TargetLimb, OutAlpha);
                }
                else
                {
                    IIAnimInstance::Execute_SetIKWeight(MeshComp->GetAnimInstance(), ToContext, R.TargetLimb, OutAlpha);
                    IIAnimInstance::Execute_SetIKWeight(MeshComp->GetAnimInstance(), FromContext, R.TargetLimb, 1.0f - OutAlpha);
                }

                //UE_LOG(LogTemp, Warning, TEXT("[Limb : %s] [OutAlpha = %f]"), *UEnum::GetValueAsString(R.TargetLimb), OutAlpha);
            }
        }
       // UE_LOG(LogTemp, Warning, TEXT("/////////////////////////////////////////////////////////"));
    }
}

void UANS_IKBlend::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Anim, const FAnimNotifyEventReference& EventReference)
{
    if (MeshComp && MeshComp->GetAnimInstance() && MeshComp->GetAnimInstance()->GetClass()->ImplementsInterface(UIAnimInstance::StaticClass()))
    {
        for (const auto& R : LimbData)
        {
            EIKContext FromContext = R.From;
            EIKContext ToContext = R.To;
            float TargetAlpha = R.bAlphaToZero ? 0.0f : 1.0f;

            if (FromContext == ToContext)
            {
                IIAnimInstance::Execute_SetIKWeight(MeshComp->GetAnimInstance(), ToContext, R.TargetLimb, TargetAlpha);
            }
            else
            {
                IIAnimInstance::Execute_SetIKWeight(MeshComp->GetAnimInstance(), ToContext, R.TargetLimb, TargetAlpha);
                IIAnimInstance::Execute_SetIKWeight(MeshComp->GetAnimInstance(), FromContext, R.TargetLimb, 1.0f - TargetAlpha);
            }
            //UE_LOG(LogTemp, Warning, TEXT("[Limb : %s] [OutAlpha = %f]"), *UEnum::GetValueAsString(R.TargetLimb), TargetAlpha);
        }
    }
}