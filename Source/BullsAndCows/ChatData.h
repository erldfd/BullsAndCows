#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "ChatData.generated.h"

/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API UChatData : public UObject
{
	GENERATED_BODY()


public:

	void SetChatText(const FString& InText);
	const FString& GetChatText() const;

private:

	FString ChatText;
	
};
