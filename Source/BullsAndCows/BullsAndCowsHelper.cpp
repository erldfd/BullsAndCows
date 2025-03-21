#include "BullsAndCowsHelper.h"

#include "Algo/RandomShuffle.h"

FString UBullsAndCowsHelper::GetRandomBullsAndCowsNumber(int32 NumberLength)
{
	int MaxNumberLength = 10;
	checkf(NumberLength <= MaxNumberLength, TEXT("NumberLength > MaxNumberLength"));

	TArray<int32> NotSelectedNumbers{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	Algo::RandomShuffle(NotSelectedNumbers);

	FString BullsAndCowsNumber = "";

	for (int i = 0; i < NumberLength; ++i)
	{
		BullsAndCowsNumber += FString::FromInt(NotSelectedNumbers[i]);
	}

	return BullsAndCowsNumber;
}

FString UBullsAndCowsHelper::GetBullsAndCowsResult(const FString& GuessedNumberStr, const FString& Answer)
{
	int MaxNumberLength = 10;
	int AnswerLength = Answer.Len();
	checkf(MaxNumberLength >= AnswerLength, TEXT("MaxNumberLength < AnswerLength"));
	FString GuessedStr = GuessedNumberStr.Mid(1);

	if (GuessedStr.Len() != Answer.Len())
	{
		return "";
	}

	int StrikeCount = 0;
	int BallCount = 0;
	int OutCount = AnswerLength;

	for (int i = 0; i < AnswerLength; ++i)
	{
		int Index = INDEX_NONE;
		if (Answer.FindChar(GuessedStr[i], Index))
		{
			OutCount--;

			if (i == Index)
			{
				StrikeCount++;
			}
			else
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == AnswerLength)
	{
		return "<Green>Correct</>";
	}

	return FString::Printf(TEXT("<Green>%dS</> <Yellow>%dB</> <Red>%dO</>"), StrikeCount, BallCount, OutCount);
}

bool UBullsAndCowsHelper::IsBullsAndCowsNumber(int32 NumberLength, const FString& Input)
{
	FString Pattern = "^/";

	for (int i = 0; i < NumberLength; ++i)
	{
		Pattern += "\\d";
	}

	Pattern += '$';

	FRegexPattern RegexPattern(Pattern);
	FRegexMatcher Matcher(RegexPattern, Input);

	return Matcher.FindNext();
}

ETextState UBullsAndCowsHelper::CheckTextState(int32 NumberLength, const FString& Input)
{
	FString Pattern = "^/\\d+$";

	FRegexPattern RegexPattern(Pattern);
	FRegexMatcher Matcher(RegexPattern, Input);

	if (Matcher.FindNext() == false)
	{
		return ETextState::NormalChat;
	}

	if (Input.Len() != NumberLength + 1)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, FString::Printf(TEXT("숫자 길이가 맞지 않음.")));
		return ETextState::NotMatchedNumberLength;
	}

	FString Matched = Matcher.GetCaptureGroup(0);

	TSet<TCHAR> CharSet;

	for (int i = 1; i < Matched.Len(); ++i)
	{
		if (CharSet.Contains(Matched[i]))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, FString::Printf(TEXT("중복 숫자 : %c"), Matched[i]));
			return ETextState::Duplicated;
		}

		CharSet.Add(Matched[i]);
	}

	return ETextState::BullsAndCows;
}
