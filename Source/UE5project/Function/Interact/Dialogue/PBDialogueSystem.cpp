// Fill out your copyright notice in the Description page of Project Settings.


#include "PBDialogueSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPBDialogueSystem::UPBDialogueSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	static ConstructorHelpers::FClassFinder<UPBScriptWidget> ScriptWidget (TEXT("/Game/Character/C_Source/PBScriptWidget_BP.PBScriptWidget_BP_C"));
	if (!ensure(ScriptWidget.Class != nullptr)) return;

	DialogueWidgetClass = ScriptWidget.Class;

	static ConstructorHelpers::FObjectFinder<UDataTable> NPCListDT_Asset(TEXT("DataTable'/Game/DialogueSample/DT_DialogueParticipant.DT_DialogueParticipant'"));
	if (NPCListDT_Asset.Succeeded())
	{
		NPCListDT = NPCListDT_Asset.Object;
	}

	Participant.DataTable = NPCListDT;
}


// Called when the game starts
void UPBDialogueSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPBDialogueSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPBDialogueSystem::SetDialogueDT(UDataTable* InitDialogueDT)
{
	DialogueDT = InitDialogueDT;
}

void UPBDialogueSystem::SetParticipant(FName CurParticipant)
{
	Participant.RowName = CurParticipant;
}

void UPBDialogueSystem::SetIsDialogue(bool CurDialogue)
{
	IsDialog = CurDialogue;
}

UDataTable* UPBDialogueSystem::GetDialogueDT()
{
	return DialogueDT;
}

FName UPBDialogueSystem::GetPrevDialogLoc()
{
	return Mark;
}

void UPBDialogueSystem::StartDialogue(AActor* InteractPlayer)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartDialogue1"));
		if (DialogueWidgetClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("StartDialogue2"));
			// Widget 인스턴스를 생성합니다.

			DialogueWidget = CreateWidget<UPBScriptWidget>(PlayerController, DialogueWidgetClass);
			//DialogueWidget->SetScriptWidget(InteractPlayer);
			IsDialog = true;
			/*
			if (DialogueWidget)
			{
				UE_LOG(LogTemp, Warning, TEXT("StartDialogue3"));
				DialogueWidget->AddToViewport();

				// 대화가 종료될 때 호출되는 함수를 설정합니다.
				// 예: 대화 창 UI를 숨기거나 제거합니다.
				//DialogueWidget->RemoveFromViewport();
			}
			*/
		}
	}
}