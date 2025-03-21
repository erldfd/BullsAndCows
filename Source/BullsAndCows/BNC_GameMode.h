#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "BNC_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API ABNC_GameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:

	void GameStart();
	void RestartGame();
	void GoToNextTurn();
	void DrawGame();
	void WinGame(bool bIsHostWinner);

	void StartTimer();
	void TerminateTimer();

public:

	const FString& GetAnswer() const;

private:

	int32 Id = 0;

	
	//UPROPERTY(ReplicatedUsing = VALUE)
	//bool bIsHostTurn = true;

	//bool bIsWaitingTime = false;

	FString Answer = "";
	int32 AnswerLength = 3;

	FTimerHandle TurnTimerHandle;
};
