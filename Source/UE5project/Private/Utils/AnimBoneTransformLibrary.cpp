// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/AnimBoneTransformLibrary.h"
#include "Animation/AnimSequence.h"
#include "Utils/AnimBoneTransformDataAsset.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"
#include "BonePose.h"

FTransform UAnimBoneTransformLibrary::GetBoneTransformAtTime(UAnimSequence* AnimSequence, const FName& BoneName, float Time)
{
    if (!AnimSequence || !AnimSequence->GetSkeleton())
    {
        return FTransform::Identity;
    }

    const USkeleton* Skeleton = AnimSequence->GetSkeleton();
    const FReferenceSkeleton& RefSkeleton = Skeleton->GetReferenceSkeleton();

    const int32 BoneIndex = RefSkeleton.FindBoneIndex(BoneName);
    if (BoneIndex == INDEX_NONE)
    {
        return FTransform::Identity;
    }

    // 1. Required Bone Indices: 모든 본을 포함
    TArray<FBoneIndexType> RequiredBoneIndices;
    const int32 NumBones = RefSkeleton.GetNum();
    for (int32 i = 0; i < NumBones; ++i)
    {
        RequiredBoneIndices.Add(i);
    }

    // 2. Curve Evaluation Option
    FCurveEvaluationOption CurveOption(false);

    // 3. FBoneContainer 생성
    FBoneContainer RequiredBones(RequiredBoneIndices, CurveOption, *AnimSequence->GetSkeleton());

    // 4. Pose 생성 준비
    FMemMark Mark(FMemStack::Get());
    FCompactPose Pose;
    FBlendedCurve Curve;
    UE::Anim::FStackAttributeContainer Attributes;
    FAnimationPoseData AnimationPoseData(Pose, Curve, Attributes);

    Pose.SetBoneContainer(&RequiredBones);

    // 5. 애니메이션 포즈 추출 (지정 시간)
    FAnimExtractContext ExtractContext(Time, false);
    AnimSequence->GetAnimationPose(AnimationPoseData, ExtractContext);

    // 6. 로컬 트랜스폼 배열 생성 (Pose에서 직접 가져오기)
    TArray<FTransform> LocalTransforms;
    LocalTransforms.SetNumUninitialized(Pose.GetNumBones());

    for (FCompactPoseBoneIndex Index(0); Index < Pose.GetNumBones(); ++Index)
    {
        LocalTransforms[Index.GetInt()] = Pose[Index];
    }

    // 7. 컴포넌트 스페이스 트랜스폼 계산
    TArray<FTransform> ComponentSpaceTransforms;
    TArrayView<const FTransform> LocalTransformsView(LocalTransforms);

    FAnimationRuntime::FillUpComponentSpaceTransforms(RefSkeleton, LocalTransformsView, ComponentSpaceTransforms);

    FTransform RootTransform = ComponentSpaceTransforms[0];
    FTransform BoneTransform = ComponentSpaceTransforms[BoneIndex];

    FTransform RelativeTransform = BoneTransform.GetRelativeTransform(RootTransform);

    return RelativeTransform;
}

void UAnimBoneTransformLibrary::ExtractAnimBoneTransformToAsset(UAnimSequence* AnimSequence, const FName& BoneName, float FrameRate, const FString& SavePath, const FString& AssetName)
{
    if (!AnimSequence) return;

    const float Duration = AnimSequence->GetPlayLength();
    TArray<FBoneFrameSample> Samples;

    for (float Time = 0.f; Time <= Duration; Time += FrameRate)
    {
        FTransform BoneTransform = GetBoneTransformAtTime(AnimSequence, BoneName, Time);
        Samples.Add(FBoneFrameSample(Time, BoneTransform));
    }

    FString PackageName = SavePath + "/" + AssetName;
    UPackage* Package = CreatePackage(*PackageName);
    UAnimBoneTransformDataAsset* NewAsset = NewObject<UAnimBoneTransformDataAsset>(Package, *AssetName, RF_Public | RF_Standalone);

    NewAsset->BoneTransformArray = Samples;

    FAssetRegistryModule::AssetCreated(NewAsset);
    NewAsset->MarkPackageDirty();

    FSavePackageArgs SaveArgs;
    UPackage::SavePackage(Package, NewAsset, *FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension()), SaveArgs);
}