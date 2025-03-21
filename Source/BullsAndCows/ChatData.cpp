#include "ChatData.h"

void UChatData::SetChatText(const FString& InText)
{
	ChatText = InText;
}

const FString& UChatData::GetChatText() const
{
	return ChatText;
}
