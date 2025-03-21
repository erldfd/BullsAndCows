#include "ChatPlayerController.h"

#include "HUDWidget.h"
#include "ReadyWidget.h"
#include "BNC_GameMode.h"
#include "BullsAndCowsHelper.h"
#include "BNC_GameState.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "Online/CoreOnline.h"

void AChatPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController() == false)
	{
		return;
	}

	check(HudWidgetClass);

	HudWidgetInstance = CreateWidget<UHUDWidget>(this, HudWidgetClass);
	check(HudWidgetInstance);

	//HudWidgetInstance->AddToViewport();

	check(ReadyWidgetClass);

	ReadyWidgetInstance = CreateWidget<UReadyWidget>(this, ReadyWidgetClass);
	check(ReadyWidgetInstance);

	ReadyWidgetInstance->AddToViewport();

	SetInputMode(FInputModeGameAndUI());
	SetShowMouseCursor(true);
}

void AChatPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AChatPlayerController, bIsReady);
}

UHUDWidget* AChatPlayerController::GetHudWidgetInstance() const
{
	return HudWidgetInstance;
}

void AChatPlayerController::StartHudTimer_Client_Implementation()
{
	HudWidgetInstance->StartTimer();
}

void AChatPlayerController::TerminateHudTimer_Client_Implementation()
{
	HudWidgetInstance->TerminateTimer();
}

void AChatPlayerController::RequestSetReadyForGame_Server_Implementation(bool bShouldReady)
{
	if (HasAuthority() == false)
	{
		return;
	}

	ABNC_GameMode* GameMode = CastChecked<ABNC_GameMode>(UGameplayStatics::GetGameMode(this));

	uint8 bAreAllPlayersReady = 1;
	bIsReady = bShouldReady;

	int32 PlayerIndex = PlayerState->GetPlayerId();
	for (AChatPlayerController* PlayerController : TActorRange<AChatPlayerController>(GetWorld()))
	{
		check(PlayerController);
		
		PlayerController->SetActiveReady_Client(PlayerIndex, bShouldReady);
		bAreAllPlayersReady *= (int32)PlayerController->GetIsReady();
	}

	if (bAreAllPlayersReady)
	{
		GameMode->GameStart();
	}
}

void AChatPlayerController::SetActiveReady_Client_Implementation(int32 PlayerIndex, bool bShouldReady)
{
	check(ReadyWidgetInstance);

	ReadyWidgetInstance->SetActiveReadyText(PlayerIndex, bShouldReady);
}

bool AChatPlayerController::GetIsReady() const
{
	return bIsReady;
}

void AChatPlayerController::SetIsReady(bool NewIsReady)
{
	if (HasAuthority() == false)
	{
		return;
	}

	bIsReady = NewIsReady;
}

void AChatPlayerController::RemoveReadyWidget_Client_Implementation()
{
	check(IsLocalController());

	ReadyWidgetInstance->RemoveFromParent();
}

void AChatPlayerController::AddHudWidgetToViewport_Client_Implementation()
{
	check(IsLocalController());

	HudWidgetInstance->AddToViewport();
}

void AChatPlayerController::SendMessageToServer_Server_Implementation(const FString& Message)
{
	if (HasAuthority() == false)
	{
		return;
	}
	
	ABNC_GameState* GameState = CastChecked<ABNC_GameState>(UGameplayStatics::GetGameState(this));
	ABNC_GameMode* GameMode = CastChecked<ABNC_GameMode>(UGameplayStatics::GetGameMode(this));
	int32 NumberLength = 3;
	int32 PlayerIndex = PlayerState->GetPlayerId();

	bool bIsHostChat = (PlayerIndex == 0);

	if (GameState->GetCurrentGameState() == EGameState::PlayingNow)
	{
		ETextState TextState = UBullsAndCowsHelper::CheckTextState(NumberLength, Message);

		switch (TextState)
		{
		case ETextState::NormalChat:
			break;
		case ETextState::BullsAndCows:

			// 호스트 턴이면서 호스트가 쓴 채팅일 경우 아니면 둘 다 아닐 경우(다른 플레이어일 경우)
			if (GameState->IsHostTurn() == bIsHostChat)
			{
				FString ResultMessage = UBullsAndCowsHelper::GetBullsAndCowsResult(Message, GameMode->GetAnswer());
				FString NewMessage = FString::Printf(TEXT("Player%d의 선택 : %s [ %s ]"), PlayerIndex, *Message.Mid(1), *ResultMessage);
				SendBullsAndCowsTextToAllClient(NewMessage);

				if (ResultMessage == "<Green>Correct</>")
				{
					GameMode->WinGame(bIsHostChat);
					return;
				}

				GameMode->GoToNextTurn();
				return;
			}

			break;

		case ETextState::Duplicated:

			GameMode->WinGame(bIsHostChat == false);
			break;

		case ETextState::NotMatchedNumberLength:

			GameMode->WinGame(bIsHostChat == false);
			break;

		default:
			break;
		}
		
	}
	
	FString NewMessage = FString::Printf(TEXT("Player%d : %s"), PlayerIndex, *Message);
	SendChatTextToAllClient(NewMessage);
}

void AChatPlayerController::SendBullsAndCowsTextToClient_Client_Implementation(const FString& Text)
{
	check(HudWidgetInstance);

	HudWidgetInstance->AddHistoryText(Text);
}

void AChatPlayerController::SendChatTextToClient_Client_Implementation(const FString& ChatText)
{
	check(HudWidgetInstance);

	HudWidgetInstance->AddChatText(ChatText);
}

void AChatPlayerController::SendBullsAndCowsTextToAllClient(const FString& Text)
{
	if (HasAuthority() == false)
	{
		return;
	}

	for (AChatPlayerController* PlayerController : TActorRange<AChatPlayerController>(GetWorld()))
	{
		check(PlayerController);
		PlayerController->SendBullsAndCowsTextToClient_Client(Text);
	}
}

void AChatPlayerController::SendChatTextToAllClient(const FString& ChatText)
{
	if (HasAuthority() == false)
	{
		return;
	}

	for (AChatPlayerController* PlayerController : TActorRange<AChatPlayerController>(GetWorld()))
	{
		check(PlayerController);
		PlayerController->SendChatTextToClient_Client(ChatText);
	}
}

void AChatPlayerController::RequestSetReadyForGameInGameRoom_Server_Implementation(bool bShouldReady)
{
	if (HasAuthority() == false)
	{
		return;
	}

	ABNC_GameMode* GameMode = CastChecked<ABNC_GameMode>(UGameplayStatics::GetGameMode(this));

	uint8 bAreAllPlayersReady = 1;
	bIsReady = bShouldReady;

	int32 PlayerIndex = PlayerState->GetPlayerId();
	for (AChatPlayerController* PlayerController : TActorRange<AChatPlayerController>(GetWorld()))
	{
		check(PlayerController);

		PlayerController->SetActiveReadyInGameRoom_Client(PlayerIndex, bShouldReady);
		bAreAllPlayersReady *= (int32)PlayerController->GetIsReady();
	}

	if (bAreAllPlayersReady)
	{
		GameMode->RestartGame();
	}
}

void AChatPlayerController::SetActiveReadyInGameRoom_Client_Implementation(int32 PlayerIndex, bool bShouldReady)
{
	check(HudWidgetInstance);

	if (PlayerIndex == 0)
	{
		HudWidgetInstance->SetActiveHostReadyText(bShouldReady);
	}
	else
	{
		HudWidgetInstance->SetActiveGuestReadyText(bShouldReady);
	}
}
