#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "BNC_GameState.generated.h"

UENUM()
enum class EGameState : uint8
{
	NotStarted,
	PlayingNow,
	HostWon,
	GuestWon,
	Draw
};

/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API ABNC_GameState : public AGameStateBase
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:

	int32 GetRemainingChance(bool bIsHost);
	void SetRemainingChance(bool bIsHost, int32 NewRemaingChance);

	int32 GetInitialChanceCount() const;

	bool IsHostTurn() const;
	void SetIsHostTurn(bool NewIsHostTurn);

	EGameState GetCurrentGameState() const;
	void SetCurrentGameState(const EGameState& NewState);

	float GetMaxTurnTime() const;
	void SetMaxTurnTime(float NewMaxTurnTime);

	int32 GetHostScore() const;
	void SetHostScore(int32 NewScore);

	int32 GetGuestScore() const;
	void SetGuestScore(int32 NewScore);

private:

	UFUNCTION()
	void OnRep_RemainingChance_Host();

	UFUNCTION()
	void OnRep_RemainingChance_Guest();

	UFUNCTION()
	void OnRep_CurrentGameState();

	UFUNCTION()
	void OnRep_IsHostTurn();

	UFUNCTION()
	void OnRep_MaxTurnTime();

	UFUNCTION()
	void OnRep_HostScore();

	UFUNCTION()
	void OnRep_GuestScore();

protected:

	UPROPERTY(ReplicatedUsing = OnRep_RemainingChance_Host);
	int32 RemainingChance_Host = 3;

	UPROPERTY(ReplicatedUsing = OnRep_RemainingChance_Guest)
	int32 RemainingChance_Guest = 3;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentGameState)
	EGameState CurrentGameState = EGameState::NotStarted;

	UPROPERTY(ReplicatedUsing = OnRep_IsHostTurn)
	bool bIsHostTurn = true;

	UPROPERTY(ReplicatedUsing = OnRep_MaxTurnTime, EditAnywhere, Category = "BNC_GameState")
	float MaxTurnTime = 10.0f;

	UPROPERTY(ReplicatedUsing = OnRep_HostScore)
	int32 HostScore = 0;

	UPROPERTY(ReplicatedUsing = OnRep_GuestScore)
	int32 GuestScore = 0;

	UPROPERTY(EditAnywhere, Category = "BNC_GameState")
	int32 InitialChanceCount = 3;
};
