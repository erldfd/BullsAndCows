#include "HUDWidget.h"

#include "HistoryView.h"
#include "ChatHistoryItemWidget.h"
#include "ChatData.h"
#include "ChatPlayerController.h"
#include "BNC_GameState.h"

#include "Kismet/GameplayStatics.h"
#include "Components/EditableTextBox.h"
#include "Components/RichTextBlock.h"
#include "Runtime/AdvancedWidgets/Public/Components/RadialSlider.h"
#include "Components/Button.h"
#include "Components/Overlay.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TextInputBox->OnTextCommitted.AddDynamic(this, &UHUDWidget::OnInputText);
	ReadyButton->OnClicked.AddDynamic(this, &UHUDWidget::OnReadyButtonClicked);
	SetHostScoreText(0);
	SetGuestScoreText(0);

	ABNC_GameState* GameState = CastChecked<ABNC_GameState>(UGameplayStatics::GetGameState(this));
	MaxTurnTime = GameState->GetMaxTurnTime();
}

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ElapsedTime == MaxTurnTime)
	{
		return;
	}
	else if (ElapsedTime > MaxTurnTime)
	{
		ElapsedTime = MaxTurnTime;
		return;
	}

	ElapsedTime += InDeltaTime;

	SetTimerRadialSlider(MaxTurnTime, ElapsedTime);
}

void UHUDWidget::AddHistoryText(const FString& Text)
{
	UChatData* NewChatData = NewObject<UChatData>(this);

	NewChatData->SetChatText(Text);
	HistoryView->AddItem(NewChatData);
	HistoryView->SetScrollOffset(9999);
}

void UHUDWidget::AddChatText(const FString& Text)
{
	UChatData* NewChatData = NewObject<UChatData>(this);
	NewChatData->SetChatText(Text);
	ChatView->AddItem(NewChatData);
	ChatView->SetScrollOffset(9999);
}

void UHUDWidget::SetRemainChanceCountText_Host(int32 Count)
{
	FString NewText = FString::Printf(TEXT("남은 기회 : <Yellow>%d</>"), Count);

	RemainChanceCountText_Host->SetText(FText::FromString(NewText));
}

void UHUDWidget::SetRemainChanceCountText_Guest(int32 Count)
{
	FString NewText = FString::Printf(TEXT("남은 기회 : <Yellow>%d</>"), Count);

	RemainChanceCountText_Guest->SetText(FText::FromString(NewText));
}

void UHUDWidget::SetCurrentTurnOwnerText(int32 PlayerIndex)
{
	FString NewText = "";

	if (PlayerIndex == 0)
	{
		NewText = FString::Printf(TEXT("<Red>Player0</>의 턴"));
	}
	else
	{
		NewText = FString::Printf(TEXT("<Green>Player1</>의 턴"));
	}

	CurrentTurnOwnerText->SetText(FText::FromString(NewText));
}

void UHUDWidget::SetWinText(int32 PlayerIndex)
{
	FString NewText = "";

	if (PlayerIndex == 0)
	{
		NewText = FString::Printf(TEXT("<Red>Player0</> 승리 !"));
	}
	else if(PlayerIndex == 1)
	{
		NewText = FString::Printf(TEXT("<Green>Player1</> 승리 !"));
	}
	else
	{
		NewText = FString::Printf(TEXT("무승부 !"));
	}

	WinText->SetText(FText::FromString(NewText));
}

void UHUDWidget::SetTimerRadialSlider(float InMaxTurnTime, float CurrentTurnTime)
{
	float TimeRate = CurrentTurnTime / InMaxTurnTime;
	TimerRadialSlider->SetSliderHandleStartAngle(TimeRate * 360.0f);
}

void UHUDWidget::SetElapsedTime(float NewElapsedTime)
{
	ElapsedTime = NewElapsedTime;
}

void UHUDWidget::SetMaxTurnTime(float NewMaxTurnTime)
{
	MaxTurnTime = NewMaxTurnTime;
}

void UHUDWidget::SetHostScoreText(int32 NewScore)
{
	HostScoreText->SetText(FText::FromString(FString::Printf(TEXT("<Red>%d</>"), NewScore)));
}

void UHUDWidget::SetGuestScoreText(int32 NewScore)
{
	GuestScoreText->SetText(FText::FromString(FString::Printf(TEXT("<Green>%d</>"), NewScore)));
}

void UHUDWidget::SetActiveRemainChanceCountText_Host(bool bShouldActivate)
{
	if (bShouldActivate)
	{
		RemainChanceCountText_Host->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		RemainChanceCountText_Host->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDWidget::SetActiveRemainChanceCountText_Guest(bool bShouldActivate)
{
	if (bShouldActivate)
	{
		RemainChanceCountText_Guest->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		RemainChanceCountText_Guest->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDWidget::SetActiveCurrentTurnOwnerText(bool bShouldActivate)
{
	if (bShouldActivate)
	{
		CurrentTurnOwnerText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		CurrentTurnOwnerText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDWidget::SetActiveWinText(bool bShouldActivate)
{
	if (bShouldActivate)
	{
		WinText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		WinText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDWidget::SetActiveTimerRadialSlider(bool bShouldActivate)
{
	if (bShouldActivate)
	{
		TimerRadialSlider->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		TimerRadialSlider->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDWidget::SetActiveReadyButton(bool bShouldActivate)
{
	if (bShouldActivate)
	{
		ReadyButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ReadyButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDWidget::SetActivePlayerNames(bool bShouldActivate)
{
	if (bShouldActivate)
	{
		HostNameText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		GuestNameText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		HostNameText->SetVisibility(ESlateVisibility::Hidden);
		GuestNameText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDWidget::SetActiveScoreBoardOverlay(bool bShouldActivate)
{
	if (bShouldActivate)
	{
		ScoreBoardOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		ScoreBoardOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDWidget::SetActiveHostReadyText(bool bShouldActivate)
{
	if (bShouldActivate)
	{
		HostReadyText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		HostReadyText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDWidget::SetActiveGuestReadyText(bool bShouldActivate)
{
	if (bShouldActivate)
	{
		GuestReadyText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		GuestReadyText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHUDWidget::ReadyHudForGameStart()
{
	ABNC_GameState* GameState = CastChecked<ABNC_GameState>(UGameplayStatics::GetGameState(this));

	SetRemainChanceCountText_Host(GameState->GetRemainingChance(true));
	SetRemainChanceCountText_Guest(GameState->GetRemainingChance(false));
	SetCurrentTurnOwnerText((GameState->IsHostTurn()) ? 0 : 1);
	
	SetActiveRemainChanceCountText_Host(true);
	SetActiveRemainChanceCountText_Guest(true);
	SetActiveCurrentTurnOwnerText(true);
	SetActiveTimerRadialSlider(true);
	SetActivePlayerNames(true);
	SetActiveHostReadyText(false);
	SetActiveGuestReadyText(false);
	SetActiveScoreBoardOverlay(true);
	SetActiveWinText(false);
	SetActiveReadyButton(false);

	ClearHistoryView();
}

void UHUDWidget::ReadyHudForDrawGame()
{
	SetWinText(2);
	SetActiveWinText(true);
	SetActiveTimerRadialSlider(false);
	SetActiveReadyButton(true);
}

void UHUDWidget::ReadyHudForWinGame(bool bWonHost)
{
	SetWinText((bWonHost) ? 0 : 1);
	SetActiveWinText(true);
	SetActiveTimerRadialSlider(false);
	SetActiveReadyButton(true);
}

void UHUDWidget::StartTimer()
{
	ElapsedTime = 0.0f;
}

void UHUDWidget::TerminateTimer()
{
	ElapsedTime = MaxTurnTime;
}

void UHUDWidget::ClearHistoryView()
{
	HistoryView->ClearListItems();
}

void UHUDWidget::OnInputText(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod != ETextCommit::Type::OnEnter)
	{
		return;
	}

	AChatPlayerController* PlayerController = CastChecked<AChatPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->SendMessageToServer_Server(Text.ToString());
	TextInputBox->SetText(FText());

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, [&]
		{
			TextInputBox->SetFocus();

		}, 0.1f, false);
}

void UHUDWidget::OnReadyButtonClicked()
{
	AChatPlayerController* Controller = CastChecked<AChatPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	Controller->RequestSetReadyForGameInGameRoom_Server(Controller->GetIsReady() == false);
}
