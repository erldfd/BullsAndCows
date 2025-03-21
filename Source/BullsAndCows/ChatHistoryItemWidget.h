#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Blueprint/IUserObjectListEntry.h"

#include "ChatHistoryItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API UChatHistoryItemWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:

	UChatHistoryItemWidget(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject);

protected:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class URichTextBlock> ChatText;
};
