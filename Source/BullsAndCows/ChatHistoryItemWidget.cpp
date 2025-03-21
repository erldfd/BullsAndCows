#include "ChatHistoryItemWidget.h"

#include "Components/RichTextBlock.h"
#include "ChatData.h"

UChatHistoryItemWidget::UChatHistoryItemWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UChatHistoryItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UChatData* Data = CastChecked<UChatData>(ListItemObject);

	ChatText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *Data->GetChatText())));
}
