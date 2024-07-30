// Fill out your copyright notice in the Description page of Project Settings.


#include "PBScriptWidget.h"
#include "PBDialogueSystem.h"
#include "../../../NPC/PBNPCBase.h"
#include "Input/Reply.h"
#include "../PBInteractInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

UPBScriptWidget::UPBScriptWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UPBScriptWidget::SetScriptWidget(APBNPCBase* InteractActor)
{
	if (InteractActor)
	{
		DialogueActor = InteractActor;
		DialogueComponent = DialogueActor->GetComponentByClass<UPBDialogueSystem>();
		if (DialogueComponent)
		{
			DialogueDT = DialogueComponent->GetDialogueDT();
			if (DialogueDT)
			{
				APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				FInputModeUIOnly WidgetInputMode;
				WidgetInputMode.SetWidgetToFocus(this->TakeWidget());
				PlayerController->SetInputMode(WidgetInputMode);
				SetIsFocusable(true);
				SetKeyboardFocus();
				DialogueComponent->SetIsDialogue(true);
				ShowDialogue(GetDTLoc());
				AddToViewport();
			}
		}
	}
}

void UPBScriptWidget::ShowDialogue(FName RowName)
{
	CurDialogueDTRow = DialogueDT->FindRow<FDialogueContentStruct>(RowName, "");
	if (!CurDialogueDTRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("Row Name Not Found in DT"));
		return;
	}
	SetTypeWriter(CurDialogueDTRow->Text);
}

void UPBScriptWidget::SetTypeWriter(FText CurText)
{
	//UE_LOG(LogTemp, Warning, TEXT("SetTypeWriter"));
	InitTypeWriter();
	TypingDelay.BindUObject(this, &UPBScriptWidget::TypeWriter);
	ScriptText = CurText;
	ScriptString = ScriptText.ToString();
	ScriptLength = ScriptString.Len();
	TypeWriter();
	IsTypeWriting = true;
}

void UPBScriptWidget::TypeWriter()
{
	//UE_LOG(LogTemp, Warning, TEXT("TypeWrtier"));
	ScriptChar = UKismetStringLibrary::GetSubstring(ScriptString, ScriptIndex, 1);
	FFormatNamedArguments CombineText;
	CombineText.Add(TEXT("Origin"), CurScriptText);
	CombineText.Add(TEXT("New"), FText::FromString(ScriptChar));

#define LOCTEXT_NAMESPACE "MyNamespace"
	CurScriptText = FText::Format(LOCTEXT("NextScriptText", "{Origin}{New}"), CombineText);
#undef LOCTEXT_NAMESPACE

	if (ScriptTextBlock != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ScriptTextBlock NOT NULL"));
		ScriptTextBlock->SetText(CurScriptText);
		ScriptIndex++;
		if (ScriptIndex < ScriptLength)
		{
			float Delay = 0.1f;
			GetWorld()->GetTimerManager().SetTimer(TypingDelayHandle, TypingDelay, Delay, false);
		}
		else
		{
			IsTypeWriting = false;
		}
	}
}

void UPBScriptWidget::CompleteTypeWriter()
{
	GetWorld()->GetTimerManager().ClearTimer(TypingDelayHandle);
	ScriptTextBlock->SetText(CurDialogueDTRow->Text);
	IsTypeWriting = false;
}

void UPBScriptWidget::InitTypeWriter()
{
	ScriptIndex = 0;
	ScriptLength = 0;
	ScriptText = FText::GetEmpty();
	CurScriptText = FText::GetEmpty();
	ScriptString = NULL;
	ScriptChar = NULL;
}

void UPBScriptWidget::CloseDialogue()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FInputModeGameOnly CharacterInputMode;
	PlayerController->SetInputMode(CharacterInputMode);
	RemoveFromViewport();
	IPBInteractInterface::Execute_EndInteract(DialogueActor);
}

FName UPBScriptWidget::GetDTLoc()
{
	return DialogueComponent->GetPrevDialogLoc() == NAME_None ? FName("Start") : DialogueComponent->GetPrevDialogLoc();
}

FReply UPBScriptWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	if (IsTypeWriting)
	{
		CompleteTypeWriter();
	}
	else
	{
		if (CurDialogueDTRow->JumpTo == FName("Finish"))
		{
			CloseDialogue();
		}
		else
		{
			ShowDialogue(CurDialogueDTRow->JumpTo);
		}
	}
	return FReply::Handled();
	//return FReply::Unhandled();
}
