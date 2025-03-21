#include "BNC_GameState.h"

#include "ChatPlayerController.h"
#include "HUDWidget.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"

void ABNC_GameState::BeginPlay()
{
	Super::BeginPlay();
}

void ABNC_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABNC_GameState, RemainingChance_Host);
	DOREPLIFETIME(ABNC_GameState, RemainingChance_Guest);
	DOREPLIFETIME(ABNC_GameState, CurrentGameState);
	DOREPLIFETIME(ABNC_GameState, bIsHostTurn);
	DOREPLIFETIME(ABNC_GameState, MaxTurnTime);
	DOREPLIFETIME(ABNC_GameState, HostScore);
	DOREPLIFETIME(ABNC_GameState, GuestScore);
}

int32 ABNC_GameState::GetRemainingChance(bool bIsHost)
{
	if (bIsHost)
	{
		return RemainingChance_Host;
	}

	return RemainingChance_Guest;
}

void ABNC_GameState::SetRemainingChance(bool bIsHost, int32 NewRemaingChance)
{
	if (HasAuthority() == false)
	{
		return;
	}

	if (bIsHost)
	{
		RemainingChance_Host = NewRemaingChance;
		OnRep_RemainingChance_Host();
	}
	else
	{
		RemainingChance_Guest = NewRemaingChance;
		OnRep_RemainingChance_Guest();
	}
}

int32 ABNC_GameState::GetInitialChanceCount() const
{
	return InitialChanceCount;
}

bool ABNC_GameState::IsHostTurn() const
{
	return bIsHostTurn;
}

void ABNC_GameState::SetIsHostTurn(bool NewIsHostTurn)
{
	if (HasAuthority() == false)
	{
		return;
	}

	bIsHostTurn = NewIsHostTurn;
	OnRep_IsHostTurn();
}

EGameState ABNC_GameState::GetCurrentGameState() const
{
	return CurrentGameState;
}

void ABNC_GameState::SetCurrentGameState(const EGameState& NewState)
{
	if (HasAuthority() == false)
	{
		return;
	}

	CurrentGameState = NewState;
	OnRep_CurrentGameState();
}

float ABNC_GameState::GetMaxTurnTime() const
{
	return MaxTurnTime;
}

void ABNC_GameState::SetMaxTurnTime(float NewMaxTurnTime)
{
	if (HasAuthority() == false)
	{
		return;
	}

	MaxTurnTime = NewMaxTurnTime;
	OnRep_MaxTurnTime();
}

int32 ABNC_GameState::GetHostScore() const
{
	return HostScore;
}

void ABNC_GameState::SetHostScore(int32 NewScore)
{
	if (HasAuthority() == false)
	{
		return;
	}

	HostScore = NewScore;
	OnRep_HostScore();
}

int32 ABNC_GameState::GetGuestScore() const
{
	return GuestScore;
}

void ABNC_GameState::SetGuestScore(int32 NewScore)
{
	if (HasAuthority() == false)
	{
		return;
	}

	GuestScore = NewScore;
	OnRep_GuestScore();
}

void ABNC_GameState::OnRep_RemainingChance_Host()
{
	AChatPlayerController* PlayerController = CastChecked<AChatPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	int32 PlayerIndex = PlayerController->PlayerState->GetPlayerId();
	UHUDWidget* Hud = PlayerController->GetHudWidgetInstance();
	check(Hud);

	Hud->SetRemainChanceCountText_Host(RemainingChance_Host);
}

void ABNC_GameState::OnRep_RemainingChance_Guest()
{
	AChatPlayerController* PlayerController = CastChecked<AChatPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	int32 PlayerIndex = PlayerController->PlayerState->GetPlayerId();
	UHUDWidget* Hud = PlayerController->GetHudWidgetInstance();
	check(Hud);

	Hud->SetRemainChanceCountText_Guest(RemainingChance_Guest);
}

void ABNC_GameState::OnRep_CurrentGameState()
{
	AChatPlayerController* PlayerController = CastChecked<AChatPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	int32 PlayerIndex = PlayerController->PlayerState->GetPlayerId();
	UHUDWidget* Hud = PlayerController->GetHudWidgetInstance();
	check(Hud);

	if (CurrentGameState == EGameState::PlayingNow)
	{
		Hud->ReadyHudForGameStart();
	}
	else if (CurrentGameState == EGameState::Draw)
	{
		Hud->ReadyHudForDrawGame();
	}
	else if (CurrentGameState == EGameState::HostWon)
	{
		Hud->ReadyHudForWinGame(true);
	}
	else
	{
		Hud->ReadyHudForWinGame(false);
	}
}

void ABNC_GameState::OnRep_IsHostTurn()
{
	AChatPlayerController* PlayerController = CastChecked<AChatPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	int32 PlayerIndex = (bIsHostTurn) ? 0 : 1;
	PlayerController->GetHudWidgetInstance()->SetCurrentTurnOwnerText(PlayerIndex);
}

void ABNC_GameState::OnRep_MaxTurnTime()
{
	AChatPlayerController* PlayerController = CastChecked<AChatPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	PlayerController->GetHudWidgetInstance()->SetMaxTurnTime(MaxTurnTime);
}

void ABNC_GameState::OnRep_HostScore()
{
	AChatPlayerController* PlayerController = CastChecked<AChatPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	PlayerController->GetHudWidgetInstance()->SetHostScoreText(HostScore);
}

void ABNC_GameState::OnRep_GuestScore()
{
	AChatPlayerController* PlayerController = CastChecked<AChatPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	PlayerController->GetHudWidgetInstance()->SetGuestScoreText(GuestScore);
}
