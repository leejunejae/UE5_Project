// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../PBWidgetInterface.h"
#include "PDialogueStruct.h"
#include "Components/RichTextBlock.h"
#include "PBScriptWidget.generated.h"

/**
 * 
 */
class IPBInteractInterface;
class APBNPCBase;
class UPBDialogueSystem;

UCLASS()
class UE5PROJECT_API UPBScriptWidget : public UPBWidgetInterface
{
	GENERATED_BODY()

public:
	UPBScriptWidget(const FObjectInitializer& ObjectInitializer);
	void SetScriptWidget(APBNPCBase* InteractActor);
	void ShowDialogue(FName RowName);
	void SetTypeWriter(FText CurText);
	void TypeWriter();
	void CompleteTypeWriter();
	void InitTypeWriter();
	void CloseDialogue();
	FName GetDTLoc();
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

private:
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* ScriptBackground;

	UPROPERTY(meta = (BindWidget))
		class URichTextBlock* ScriptTextBlock;

	APBNPCBase* DialogueActor;
	UPBDialogueSystem* DialogueComponent;
	UDataTable* DialogueDT;
	FDialogueContentStruct* CurDialogueDTRow;

	// 순차적인 글자 표시를 위한 변수
	int ScriptIndex; // 출력중인 문자열 위치
	int ScriptLength; // 문자열 길이
	FText ScriptText; // 출력할 전체 문장 원본
	FText CurScriptText; // 현재 출력한 문장
	FString ScriptString; // 문장 파싱을 위해 FString형으로 변환
	FString ScriptChar; // 현재 출력할 문자 저장
	bool IsTypeWriting;

	FTimerDelegate TypingDelay;
	FTimerHandle TypingDelayHandle;
};