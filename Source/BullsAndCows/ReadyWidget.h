#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "ReadyWidget.generated.h"

/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API UReadyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

public:

	void SetActiveReadyText(int32 PlayerIndex, bool bShouldReady);
private:

	UFUNCTION()
	void OnReadyButtonClicked();

protected:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> ReadyButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> HostReadyText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> GuestReadyText;
	
};
