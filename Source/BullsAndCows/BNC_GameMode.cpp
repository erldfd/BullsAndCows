#include "BNC_GameMode.h"

#include "ChatPlayerController.h"
#include "ReadyWidget.h"
#include "HUDWidget.h"
#include "BullsAndCowsHelper.h"
#include "BNC_GameState.h"

#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

void ABNC_GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void ABNC_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	NewPlayer->PlayerState->SetPlayerId(Id++);
}

void ABNC_GameMode::GameStart()
{
	ABNC_GameState* BNCGameState = CastChecked<ABNC_GameState>(GameState);

	if (BNCGameState->GetCurrentGameState() == EGameState::PlayingNow)
	{
		return;
	}

	BNCGameState->SetMaxTurnTime(BNCGameState->GetMaxTurnTime());

	BNCGameState->SetRemainingChance(true, BNCGameState->GetInitialChanceCount());
	BNCGameState->SetRemainingChance(false, BNCGameState->GetInitialChanceCount());

	BNCGameState->SetIsHostTurn((bool)FMath::RandRange(0, 1));
	BNCGameState->SetCurrentGameState(EGameState::PlayingNow);

	for (AChatPlayerController* PlayerController : TActorRange<AChatPlayerController>(GetWorld()))
	{
		PlayerController->RemoveReadyWidget_Client();
		PlayerController->AddHudWidgetToViewport_Client();
		PlayerController->SetIsReady(false);
	}

	Answer = UBullsAndCowsHelper::GetRandomBullsAndCowsNumber(AnswerLength);

	StartTimer();
}

void ABNC_GameMode::RestartGame()
{
	ABNC_GameState* BNCGameState = CastChecked<ABNC_GameState>(GameState);

	if (BNCGameState->GetCurrentGameState() == EGameState::PlayingNow)
	{
		return;
	}

	BNCGameState->SetMaxTurnTime(BNCGameState->GetMaxTurnTime());

	BNCGameState->SetRemainingChance(true, BNCGameState->GetInitialChanceCount());
	BNCGameState->SetRemainingChance(false, BNCGameState->GetInitialChanceCount());

	BNCGameState->SetIsHostTurn((bool)FMath::RandRange(0, 1));
	BNCGameState->SetCurrentGameState(EGameState::PlayingNow);

	for (AChatPlayerController* PlayerController : TActorRange<AChatPlayerController>(GetWorld()))
	{
		PlayerController->SetIsReady(false);
	}

	Answer = UBullsAndCowsHelper::GetRandomBullsAndCowsNumber(AnswerLength);

	StartTimer();
}

void ABNC_GameMode::GoToNextTurn()
{
	ABNC_GameState* BNCGameState = CastChecked<ABNC_GameState>(GameState);
	
	bool bIsHostTurn = BNCGameState->IsHostTurn();
	BNCGameState->SetRemainingChance(bIsHostTurn, BNCGameState->GetRemainingChance(bIsHostTurn) - 1);

	int32 HostChanceCount = BNCGameState->GetRemainingChance(true);
	int32 GuestChanceCount = BNCGameState->GetRemainingChance(false);

	if (HostChanceCount == 0 && GuestChanceCount == 0)
	{
		DrawGame();
		return;
	}

	BNCGameState->SetIsHostTurn(bIsHostTurn == false);
	StartTimer();
}

void ABNC_GameMode::DrawGame()
{
	ABNC_GameState* BNCGameState = CastChecked<ABNC_GameState>(GameState);
	BNCGameState->SetCurrentGameState(EGameState::Draw);
	TerminateTimer();

	for (AChatPlayerController* PlayerController : TActorRange<AChatPlayerController>(GetWorld()))
	{
		PlayerController->SendBullsAndCowsTextToAllClient(FString::Printf(TEXT("정답은 <Yellow>%s</>"), *Answer));
		return;
	}
}

void ABNC_GameMode::WinGame(bool bIsHostWinner)
{
	ABNC_GameState* BNCGameState = CastChecked<ABNC_GameState>(GameState);
	BNCGameState->SetCurrentGameState((bIsHostWinner) ? EGameState::HostWon : EGameState::GuestWon);
	
	if (bIsHostWinner)
	{
		BNCGameState->SetHostScore(BNCGameState->GetHostScore() + 1);
	}
	else
	{
		BNCGameState->SetGuestScore(BNCGameState->GetGuestScore() + 1);
	}

	TerminateTimer();

	for (AChatPlayerController* PlayerController : TActorRange<AChatPlayerController>(GetWorld()))
	{
		PlayerController->SendBullsAndCowsTextToAllClient(FString::Printf(TEXT("정답은 <Yellow>%s</>"), *Answer));
		return;
	}
}

void ABNC_GameMode::StartTimer()
{
	ABNC_GameState* BNCGameState = CastChecked<ABNC_GameState>(GameState);

	for (AChatPlayerController* PlayerController : TActorRange<AChatPlayerController>(GetWorld()))
	{
		PlayerController->StartHudTimer_Client();
	}

	GetWorldTimerManager().SetTimer(TurnTimerHandle, [&]
		{
			GoToNextTurn();

		}, BNCGameState->GetMaxTurnTime(), false);

}

void ABNC_GameMode::TerminateTimer()
{
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);

	for (AChatPlayerController* PlayerController : TActorRange<AChatPlayerController>(GetWorld()))
	{
		PlayerController->TerminateHudTimer_Client();
	}
}

const FString& ABNC_GameMode::GetAnswer() const
{
	return Answer;
}
