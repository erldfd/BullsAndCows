#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "ChatPlayerController.generated.h"

class UHUDWidget;
class UReadyWidget;
/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API AChatPlayerController : public APlayerController
{
	GENERATED_BODY()


protected:

	virtual void BeginPlay() override;

protected:

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:

	UFUNCTION(BlueprintCallable)
	UHUDWidget* GetHudWidgetInstance() const;

	UFUNCTION(Client, Reliable)
	void StartHudTimer_Client();

	UFUNCTION(Client, Reliable)
	void TerminateHudTimer_Client();

	UFUNCTION(Server, Unreliable)
	void RequestSetReadyForGame_Server(bool bShouldReady);

	UFUNCTION(Client, Reliable)
	void SetActiveReady_Client(int32 PlayerIndex, bool bShouldReady);

	bool GetIsReady() const;
	void SetIsReady(bool NewIsReady);

	UFUNCTION(Client, Reliable)
	void RemoveReadyWidget_Client();

	UFUNCTION(Client, Reliable)
	void AddHudWidgetToViewport_Client();

	UFUNCTION(Server, Unreliable)
	void SendMessageToServer_Server(const FString& Message);

	UFUNCTION(Client, Reliable)
	void SendBullsAndCowsTextToClient_Client(const FString& Text);

	UFUNCTION(Client, Reliable)
	void SendChatTextToClient_Client(const FString& ChatText);

	void SendBullsAndCowsTextToAllClient(const FString& Text);

	void SendChatTextToAllClient(const FString& ChatText);

	UFUNCTION(Server, Unreliable)
	void RequestSetReadyForGameInGameRoom_Server(bool bShouldReady);

	UFUNCTION(Client, Reliable)
	void SetActiveReadyInGameRoom_Client(int32 PlayerIndex, bool bShouldReady);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UHUDWidget> HudWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UHUDWidget> HudWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UReadyWidget> ReadyWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UReadyWidget> ReadyWidgetInstance;

protected:

	UPROPERTY(Replicated)
	bool bIsReady = false;
};
