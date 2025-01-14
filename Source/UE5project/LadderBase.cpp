// Fill out your copyright notice in the Description page of Project Settings.


#include "LadderBase.h"

ALadderBase::ALadderBase()
{
	Tags.Add("Ladder");
	LadderScale = FVector(1.0f, 1.0f, 1.0f);
	AdditionalHeight = 0.0f;

	ExitTopPosition = CreateDefaultSubobject<USceneComponent>(TEXT("ExitTopPosition"));
	ExitTopPosition->SetupAttachment(ObjectRoot);
	ExitTopPosition->ComponentTags.Add(FName("Top"));
	ExitTopPosition->ComponentTags.Add(FName("Exit"));

	EnterTopPosition = CreateDefaultSubobject<USceneComponent>(TEXT("EnterTopPosition"));
	EnterTopPosition->SetupAttachment(ObjectRoot);
	EnterTopPosition->ComponentTags.Add(FName("Top"));
	EnterTopPosition->ComponentTags.Add(FName("Enter"));
}

void ALadderBase::OnConstruction(const FTransform& Transform)
{
	for (UStaticMeshComponent* ClimbMesh : ClimbMeshes)
	{
		if (ClimbMesh)
		{
			ClimbMesh->DestroyComponent();
		}
	}
	ClimbMeshes.Empty();

	for (UBoxComponent* GripCollision : GripCollisions)
	{
		if (GripCollision)
		{
			GripCollision->DestroyComponent();
		}
	}
	GripCollisions.Empty();
	GripList1D.Empty();

	float CumulativeHeight = 0.0f;

	for (int32 i = 0; i < LadderLevel; i++)
	{
		// 새로운 StaticMeshComponent 생성
		UStaticMeshComponent* NewClimbMesh = NewObject<UStaticMeshComponent>(this);
		NewClimbMesh->SetStaticMesh(ClimbStaticMesh);
		NewClimbMesh->SetupAttachment(RootComponent);
		NewClimbMesh->SetCanEverAffectNavigation(false);
		// 사다리 위치 설정
		NewClimbMesh->SetRelativeScale3D(LadderScale);
		NewClimbMesh->SetRelativeLocation(FVector(0.0f, 0.0f, AdditionalHeight + CumulativeHeight));
		NewClimbMesh->RegisterComponent();
		CumulativeHeight += NewClimbMesh->Bounds.BoxExtent.Z * 2.0f;

		// 생성된 컴포넌트를 배열에 추가
		ClimbMeshes.Add(NewClimbMesh);
	}

	ClimbTopTrigger->SetRelativeLocation(FVector(0.0f, -70.0f, AdditionalHeight + CumulativeHeight + ClimbTopTrigger->Bounds.BoxExtent.Z));
	ClimbTopLocation->SetRelativeLocation(FVector(0.0f, -70.0f, AdditionalHeight + CumulativeHeight + 92.0f));
}

void ALadderBase::BeginPlay()
{
	Super::BeginPlay();

	for (UStaticMeshComponent* ClimbMesh : ClimbMeshes)
	{
		if (ClimbMesh)
		{
			ClimbMesh->DestroyComponent();
		}
	}
	ClimbMeshes.Empty();

	for (UBoxComponent* GripCollision : GripCollisions)
	{
		if (GripCollision)
		{
			GripCollision->DestroyComponent();
		}
	}
	GripCollisions.Empty();
	GripList1D.Empty();

	float CumulativeHeight = 0.0f;

	for (int32 i = 0; i < LadderLevel; i++)
	{
		// 새로운 StaticMeshComponent 생성
		UStaticMeshComponent* NewClimbMesh = NewObject<UStaticMeshComponent>(this);
		NewClimbMesh->SetStaticMesh(ClimbStaticMesh);
		NewClimbMesh->SetupAttachment(RootComponent);
		NewClimbMesh->SetCanEverAffectNavigation(false);
		// 사다리 위치 설정
		NewClimbMesh->SetRelativeScale3D(LadderScale);
		NewClimbMesh->SetRelativeLocation(FVector(0.0f, 0.0f, AdditionalHeight + CumulativeHeight));
		NewClimbMesh->RegisterComponent();
		CumulativeHeight += NewClimbMesh->Bounds.BoxExtent.Z * 2.0f;

		// 생성된 컴포넌트를 배열에 추가
		ClimbMeshes.Add(NewClimbMesh);

		TArray<FName> SocketNames = NewClimbMesh->GetAllSocketNames();
		int32 SocketNum = SocketNames.Num();
		// 손잡이 콜리전 추가 (예시)
		for (FName SocketName : SocketNames)
		{
			if (!SocketName.ToString().Contains(TEXT("Grip")))
				continue;
			FVector SocketLocation = NewClimbMesh->GetSocketLocation(SocketName);
			GripList1D.Add({ SocketLocation , i + 1, {} });
			UBoxComponent* NewGripCollision = NewObject<UBoxComponent>(this);
			NewGripCollision->SetCanEverAffectNavigation(false);
			NewGripCollision->SetBoxExtent(FVector(5.0f, 5.0f, 5.0f));
			NewGripCollision->SetupAttachment(RootComponent);
			NewGripCollision->SetWorldLocation(SocketLocation);
			NewGripCollision->SetCollisionProfileName(TEXT("ClimbObject"));
			NewGripCollision->RegisterComponent();
			//SocketName.ToString().Contains(TEXT("Left")) ? NewGripCollision->ComponentTags.Add(FName("LeftGrip")) : NewGripCollision->ComponentTags.Add(FName("RightGrip"));
			GripCollisions.Add(NewGripCollision);
			//UE_LOG(LogTemp, Warning, TEXT("Before Sort GripList[%d] = %f"), z, SocketLocation.Z);
		}
	}

	ClimbTopTrigger->SetRelativeLocation(FVector(0.0f, -70.0f, AdditionalHeight + CumulativeHeight + ClimbTopTrigger->Bounds.BoxExtent.Z));
	ClimbTopLocation->SetRelativeLocation(FVector(0.0f, -70.0f, AdditionalHeight + CumulativeHeight + 92.0f));

	GripList1D.Sort([](const FGripNode1D& A, const FGripNode1D& B)
		{
			return A.Position.Z < B.Position.Z;
		});

	for (int32 GripIndex = 0; GripIndex < GripList1D.Num(); GripIndex++)
	{
		GripList1D[GripIndex].GripIndex = GripIndex;
		//UE_LOG(LogTemp, Warning, TEXT("After Sort GripList[%d] = %f"), GripIndex, GripList1D[GripIndex].Position.Z);
	}
}

USceneComponent* ALadderBase::GetExitTopLocation_Implementation()
{
	return ExitTopPosition;
}

USceneComponent* ALadderBase::GetInitTopPosition_Implementation()
{
	return EnterTopPosition;
}