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


    UE_LOG(LogTemp, Warning, TEXT("Time = %f"), Time);
    UE_LOG(LogTemp, Warning, TEXT("Location : X = %f, Y = %f, Z = %f"), ComponentSpaceTransforms[BoneIndex].GetLocation().X, ComponentSpaceTransforms[BoneIndex].GetLocation().Y, ComponentSpaceTransforms[BoneIndex].GetLocation().Z);
    UE_LOG(LogTemp, Warning, TEXT("Transform : Roll = %f, Pitch = %f, Yaw = %f"), ComponentSpaceTransforms[BoneIndex].GetRotation().Rotator().Roll, ComponentSpaceTransforms[BoneIndex].GetRotation().Rotator().Pitch, ComponentSpaceTransforms[BoneIndex].GetRotation().Rotator().Yaw);
    UE_LOG(LogTemp, Warning, TEXT("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ"));


    return ComponentSpaceTransforms[BoneIndex];

    /*
    if (!AnimSequence)
    {
        UE_LOG(LogTemp, Warning, TEXT("AnimSequence null"));
        return FTransform::Identity;
    }

    FTransform TargetBoneTransform;
    int32 TargetBoneIndex = AnimSequence->GetSkeleton()->GetReferenceSkeleton().FindBoneIndex(BoneName);
    if (TargetBoneIndex == INDEX_NONE)
    {
        return FTransform::Identity;
    }

    FSkeletonPoseBoneIndex TargetBoneIndex(TargetBoneIndex);

    AnimSequence->GetBoneTransform(TargetBoneTransform, TargetBoneIndex, Time, true);

    UE_LOG(LogTemp, Warning, TEXT("Time = %f"), Time);
    UE_LOG(LogTemp, Warning, TEXT("Location : X = %f, Y = %f, Z = %f"), OutTransform.GetLocation().X, OutTransform.GetLocation().Y, OutTransform.GetLocation().Z);
    UE_LOG(LogTemp, Warning, TEXT("Transform : Roll = %f, Pitch = %f, Yaw = %f"), OutTransform.GetRotation().Rotator().Roll, OutTransform.GetRotation().Rotator().Pitch, OutTransform.GetRotation().Rotator().Yaw);
    UE_LOG(LogTemp, Warning, TEXT("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ"));
    
    return TargetBoneTransform;
    */

    /*
    if (!AnimSequence)
    {
        UE_LOG(LogTemp, Warning, TEXT("AnimSequence null"));
        return FTransform::Identity;
    }

    const IAnimationDataModel* DataModel = AnimSequence->GetDataModel();
    if (!DataModel)
    {
        UE_LOG(LogTemp, Warning, TEXT("AnimSequence has no DataModel"));
        return FTransform::Identity;
    }

    const int32 TrackIndex = DataModel->GetBoneTrackIndexByName(BoneName);
    if (TrackIndex == INDEX_NONE)
    {
        //UE_LOG(LogTemp, Warning, TEXT("Bone %s not found in AnimSequence"), *BoneName.ToString());
        return FTransform::Identity;
    }

    const FRawAnimSequenceTrack& Track = AnimSequence->GetDataModel()->GetBoneTrackByIndex(TrackIndex).InternalTrackData;
    const int32 NumFrames = AnimSequence->GetNumberOfFrames();
    const float SequenceLength = AnimSequence->GetPlayLength();
    const float FrameDelta = SequenceLength / (float)(NumFrames - 1);

    const int32 FrameIndex = FMath::Clamp(FMath::FloorToInt(Time / FrameDelta), 0, NumFrames - 1);

    FVector3f Pos = Track.PosKeys.IsValidIndex(FrameIndex) ? Track.PosKeys[FrameIndex] : FVector3f::ZeroVector;
    FQuat4f Rot = Track.RotKeys.IsValidIndex(FrameIndex) ? Track.RotKeys[FrameIndex] : FQuat4f::Identity;
    FVector3f Scale = Track.ScaleKeys.IsValidIndex(FrameIndex) ? Track.ScaleKeys[FrameIndex] : FVector3f(1.0f);

    return FTransform(
        FQuat(Rot),      
        FVector(Pos),    
        FVector(Scale)   
    );
    */
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