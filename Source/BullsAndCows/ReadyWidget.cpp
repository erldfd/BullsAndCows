#include "ReadyWidget.h"

#include "ChatPlayerController.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UReadyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ReadyButton == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("ReadyButton == nullptr"));
		return;
	}

	ReadyButton->OnClicked.AddDynamic(this, &UReadyWidget::OnReadyButtonClicked);
}

void UReadyWidget::SetActiveReadyText(int32 PlayerIndex, bool bShouldReady)
{
	if (bShouldReady)
	{
		if (PlayerIndex == 0)
		{
			HostReadyText->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			GuestReadyText->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
	else
	{
		if (PlayerIndex == 0)
		{
			HostReadyText->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			GuestReadyText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}


void UReadyWidget::OnReadyButtonClicked()
{
	AChatPlayerController* Controller = CastChecked<AChatPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	Controller->RequestSetReadyForGame_Server(Controller->GetIsReady() == false);
}
