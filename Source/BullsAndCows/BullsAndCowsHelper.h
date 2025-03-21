#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "BullsAndCowsHelper.generated.h"

UENUM()
enum class ETextState : uint8
{
	NormalChat,
	BullsAndCows,
	Duplicated,
	NotMatchedNumberLength
};
/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API UBullsAndCowsHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "BullsAndCows")
	static FString GetRandomBullsAndCowsNumber(int32 NumberLength);

	UFUNCTION(BlueprintCallable, Category = "BullsAndCows")
	static FString GetBullsAndCowsResult(const FString& GuessedNumberStr, const FString& Answer);

	UFUNCTION(BlueprintCallable, Category = "BullsAndCows")
	static bool IsBullsAndCowsNumber(int32 NumberLength, const FString& Input);

	UFUNCTION(BlueprintCallable, Category = "BullsAndCows")
	static ETextState CheckTextState(int32 NumberLength, const FString& Input);
	
};
