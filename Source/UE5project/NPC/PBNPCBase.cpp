// Fill out your copyright notice in the Description page of Project Settings.


#include "PBNPCBase.h"
#include <Engine/Classes/Components/CapsuleComponent.h>
#include "Kismet/GameplayStatics.h"
#include "../Function/PBPlayerInterface.h"
#include "../Function/Interact/Dialogue/PBScriptWidget.h"
#include "Blueprint/UserWidget.h"

// Sets default values
APBNPCBase::APBNPCBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = GetCapsuleComponent();

	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));

	DialogueSystem = CreateDefaultSubobject<UPBDialogueSystem>(TEXT("DialogSystem"));
	DialogueSystem->bAutoActivate = true;

	InteractTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractTrigger"));
	InteractTrigger->SetupAttachment(GetMesh());
	InteractTrigger->SetRelativeLocation(FVector(0.0f, 30.0f, 110.0f));
	InteractTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));

	static ConstructorHelpers::FClassFinder<UPBScriptWidget> ScriptWidget(TEXT("/Game/Character/C_Source/PBScriptWidget_BP.PBScriptWidget_BP_C"));
	if (!ensure(ScriptWidget.Class != nullptr)) return;

	DialogueWidgetClass = ScriptWidget.Class;

	Tags.Add("NPC");
}

void APBNPCBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InteractTrigger->OnComponentBeginOverlap.AddDynamic(this, &APBNPCBase::TriggerBegin);
	InteractTrigger->OnComponentEndOverlap.AddDynamic(this, &APBNPCBase::TriggerEnd);
}

void APBNPCBase::Interact_Implementation(ACharacter* InteractActor)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		//DialogueSystem->StartDialogue(GetOwner());
		UE_LOG(LogTemp, Warning, TEXT("StartDialogue1"));
		if (DialogueWidgetClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("StartDialogue2"));
			// Widget 인스턴스를 생성합니다.

			DialogueWidget = CreateWidget<UPBScriptWidget>(PlayerController, DialogueWidgetClass);
			DialogueWidget->SetScriptWidget(this);
			DialogueSystem->SetIsDialogue(true);
			//PlayerController->SetInputMode(FInputModeGameAndUI());
		
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

		// Widget Blueprint 클래스를 로드합니다.
		/*
		TSubclassOf<UUserWidget> DialogueWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Character/C_Source/PBScriptWidget_BP.PBScriptWidget_BP_C"));

		if (DialogueWidgetClass)
		{
			// Widget 인스턴스를 생성합니다.
			UPBScriptWidget* DialogueWidget = CreateWidget<UPBScriptWidget>(PlayerController, DialogueWidgetClass);

			if (DialogueWidget)
			{
				// Widget을 플레이어에게 표시합니다.
				DialogueWidget->AddToViewport();

				// 대화가 종료될 때 호출되는 함수를 설정합니다.
				// 예: 대화 창 UI를 숨기거나 제거합니다.
				//DialogueWidget->RemoveFromViewport();
			}
		}
		*/
	}
}

void APBNPCBase::EndInteract_Implementation()
{
	DialogueSystem->SetIsDialogue(false);
}

// Called when the game starts or when spawned
void APBNPCBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APBNPCBase::TriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		if (OtherActor->GetClass()->ImplementsInterface(UPBPlayerInterface::StaticClass()))
		{
			IPBPlayerInterface::Execute_RegisterInteractableActor(OtherActor, this);
			UE_LOG(LogTemp, Warning, TEXT("Can Talk"));
		}
	}
}

void APBNPCBase::TriggerEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		if (OtherActor->GetClass()->ImplementsInterface(UPBPlayerInterface::StaticClass()))
		{
			IPBPlayerInterface::Execute_DeRegisterInteractableActor(OtherActor, this);
		}
	}
}

// Called every frame
void APBNPCBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APBNPCBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

