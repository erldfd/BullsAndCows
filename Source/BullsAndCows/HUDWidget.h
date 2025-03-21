#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	UFUNCTION(BlueprintCallable)
	void AddHistoryText(const FString& Text);

	UFUNCTION(BlueprintCallable)
	void AddChatText(const FString& Text);

	void SetRemainChanceCountText_Host(int32 Count);
	void SetRemainChanceCountText_Guest(int32 Count);
	void SetCurrentTurnOwnerText(int32 PlayerIndex);
	void SetWinText(int32 PlayerIndex);
	void SetTimerRadialSlider(float MaxTurnTime, float CurrentTurnTime);
	void SetElapsedTime(float NewElapsedTime);
	void SetMaxTurnTime(float NewMaxTurnTime);

	void SetHostScoreText(int32 NewScore);
	void SetGuestScoreText(int32 NewScore);

	void SetActiveRemainChanceCountText_Host(bool bShouldActivate);
	void SetActiveRemainChanceCountText_Guest(bool bShouldActivate);
	void SetActiveCurrentTurnOwnerText(bool bShouldActivate);
	void SetActiveWinText(bool bShouldActivate);
	void SetActiveTimerRadialSlider(bool bShouldActivate);
	void SetActiveReadyButton(bool bShouldActivate);
	void SetActivePlayerNames(bool bShouldActivate);

	void SetActiveScoreBoardOverlay(bool bShouldActivate);
	void SetActiveHostReadyText(bool bShouldActivate);
	void SetActiveGuestReadyText(bool bShouldActivate);

	void ReadyHudForGameStart();
	void ReadyHudForDrawGame();
	void ReadyHudForWinGame(bool bWonHost);

	void StartTimer();
	void TerminateTimer();

	void ClearHistoryView();

private:

	UFUNCTION()
	void OnInputText(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnReadyButtonClicked();


protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHistoryView> HistoryView;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHistoryView> ChatView;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEditableTextBox> TextInputBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> RemainChanceCountText_Host;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> RemainChanceCountText_Guest;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> CurrentTurnOwnerText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> WinText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URadialSlider> TimerRadialSlider;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ReadyButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> HostNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> GuestNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UOverlay> ScoreBoardOverlay;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> HostScoreText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> GuestScoreText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> HostReadyText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> GuestReadyText;

private:

	float ElapsedTime = 10.0f;
	float MaxTurnTime = 10.0f;
};
