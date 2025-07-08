// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/AnimBoneTransformDataAsset.h"

FTransform UAnimBoneTransformDataAsset::GetTransformAtTime(float QueryTime) const
{
    if (BoneTransformArray.Num() == 0)
        return FTransform::Identity;

    const TArray<FBoneFrameSample>& Samples = BoneTransformArray;
    int32 NumSamples = Samples.Num();

    float StartTime = Samples[0].Time;
    float EndTime = Samples.Last().Time;

    if (QueryTime <= StartTime)
        return Samples[0].BoneTransform;
    if (QueryTime >= EndTime)
        return Samples.Last().BoneTransform;

    float DeltaTime = Samples[1].Time - Samples[0].Time;
    float ExactIndex = (QueryTime - StartTime) / DeltaTime;

    int32 IndexA = FMath::FloorToInt(ExactIndex);
    int32 IndexB = IndexA + 1;
    float Alpha = ExactIndex - IndexA;

    IndexA = FMath::Clamp(IndexA, 0, NumSamples - 1);
    IndexB = FMath::Clamp(IndexB, 0, NumSamples - 1);

    const FTransform LowerTransfrom = Samples[IndexA].BoneTransform;
    const FTransform UpperTransfrom = Samples[IndexB].BoneTransform;

    FTransform BoneTransformAtTime;
    BoneTransformAtTime.Blend(LowerTransfrom, UpperTransfrom, Alpha);
    return BoneTransformAtTime;
}