// // Copyright Ashot Barkhudaryan. All Rights Reserved.
//
// #include "GamedevHelperCoreStringLibrary.h"
// // Engine Headers
// #include "Kismet/KismetStringLibrary.h"
//
// FString UGamedevHelperStringLibrary::GetFirstLetter(const FString& OriginalString)
// {
// 	if (OriginalString.IsEmpty())
// 	{
// 		return OriginalString;
// 	}
//
// 	return UKismetStringLibrary::GetSubstring(OriginalString, 0, 1);
// }
//
// FString UGamedevHelperStringLibrary::GetLastLetter(const FString& OriginalString)
// {
// 	if (OriginalString.IsEmpty())
// 	{
// 		return OriginalString;
// 	}
//
// 	return UKismetStringLibrary::GetSubstring(OriginalString, OriginalString.Len() - 1, 1);
// }
//
// FString UGamedevHelperStringLibrary::RemoveFirstLetter(const FString& OriginalString)
// {
// 	return OriginalString.RightChop(1);
// }
//
// FString UGamedevHelperStringLibrary::RemoveLastLetter(const FString& OriginalString)
// {
// 	return OriginalString.LeftChop(1);
// }
//
// FString UGamedevHelperStringLibrary::Intersection(const FString& StringA, const FString& StringB)
// {
	// if (StringA.IsEmpty() || StringB.IsEmpty())
	// {
	// 	return FString{};
	// }
	//
	// FString Chars;
	// Chars.Reserve(StringA.Len() + StringB.Len());
	//
	// for (int32 i = 0; i < StringA.Len(); ++i)
	// {
	// 	const auto SingleChar = UKismetStringLibrary::GetSubstring(StringA, i, 1);
	//
	// 	if (StringB.Contains(SingleChar, ESearchCase::CaseSensitive))
	// 	{
	// 		Chars.Append(SingleChar);
	// 	}
	// }
	//
	// Chars.Shrink();
	//
	// return Chars;
// }
//
// FString UGamedevHelperStringLibrary::Union(const FString& StringA, const FString& StringB)
// {
	// FString Chars;
	// Chars.Reserve(StringA.Len() + StringB.Len());
	//
	// for (int32 i = 0; i < StringA.Len(); ++i)
	// {
	// 	const auto SingleChar = UKismetStringLibrary::GetSubstring(StringA, i, 1);
	// 	if (!Chars.Contains(SingleChar, ESearchCase::CaseSensitive))
	// 	{
	// 		Chars.Append(SingleChar);
	// 	}
	// }
	//
	// for (int32 i = 0; i < StringB.Len(); ++i)
	// {
	// 	const auto SingleChar = UKismetStringLibrary::GetSubstring(StringB, i, 1);
	// 	if (!Chars.Contains(SingleChar, ESearchCase::CaseSensitive))
	// 	{
	// 		Chars.Append(SingleChar);
	// 	}
	// }
	//
	// Chars.Shrink();
	//
	// return Chars;
// }
//
// FString UGamedevHelperStringLibrary::SymmetricDifference(const FString& StringA, const FString& StringB)
// {
	// // Symmetric difference = union - intersection
	// const FString UnionSet = Union(StringA, StringB);
	// const FString IntersectionSet = Intersection(StringA, StringB);
	//
	// FString Chars;
	// Chars.Reserve(UnionSet.Len());
	//
	// for (int32 i = 0; i < UnionSet.Len(); ++i)
	// {
	// 	const auto SingleChar = UKismetStringLibrary::GetSubstring(UnionSet, i, 1);
	// 	if (!IntersectionSet.Contains(SingleChar, ESearchCase::CaseSensitive))
	// 	{
	// 		Chars.Append(SingleChar);
	// 	}
	// }
	//
	// Chars.Shrink();
	//
	// return Chars;
// }
//
// FString UGamedevHelperStringLibrary::Difference(const FString& StringA, const FString& StringB)
// {
	// FString Chars;
	// Chars.Reserve(StringA.Len() + StringB.Len());
	//
	// for (int32 i = 0; i < StringA.Len(); ++i)
	// {
	// 	const auto SingleChar = UKismetStringLibrary::GetSubstring(StringA, i, 1);
	// 	if (!StringB.Contains(SingleChar, ESearchCase::CaseSensitive))
	// 	{
	// 		Chars.Append(SingleChar);
	// 	}
	// }
	//
	// Chars.Shrink();
	//
	// return Chars;
// }
//
// bool UGamedevHelperStringLibrary::IsSubSet(const FString& StringA, const FString& StringB)
// {
	// for (int32 i = 0; i < StringB.Len(); ++i)
	// {
	// 	const auto SingleChar = UKismetStringLibrary::GetSubstring(StringA, i, 1);
	// 	if (!StringB.Contains(SingleChar, ESearchCase::CaseSensitive))
	// 	{
	// 		return false;
	// 	}
	// }
	//
	// return true;
// }
//
// bool UGamedevHelperStringLibrary::Contains(const FString& OriginalString, const FString& Dictionary, const ESearchCase::Type SearchCase)
// {
	// const FString StringA = SearchCase == ESearchCase::CaseSensitive ? OriginalString : OriginalString.ToLower();
	// const FString StringB = SearchCase == ESearchCase::CaseSensitive ? Dictionary : Dictionary.ToLower();
	//
	// return Intersection(StringA, StringB).Len() > 0;
// }
//
// bool UGamedevHelperStringLibrary::ContainsOnly(const FString& OriginalString, const FString& Dictionary, const ESearchCase::Type SearchCase)
// {
	// const FString StringA = SearchCase == ESearchCase::CaseSensitive ? OriginalString : OriginalString.ToLower();
	// const FString StringB = SearchCase == ESearchCase::CaseSensitive ? Dictionary : Dictionary.ToLower();
	//
	// return IsSubSet(StringA, StringB);
// }
//
// bool UGamedevHelperStringLibrary::ContainsLetters(const FString& OriginalString)
// {
	// const FString Alpha = TEXT("abcdefghijklmnopqrstuvwxyz");
	// return Contains(OriginalString, Alpha, ESearchCase::IgnoreCase);
// }
//
// bool UGamedevHelperStringLibrary::ContainsDigits(const FString& OriginalString)
// {
	// const FString Digits = TEXT("0123456789");
	// return Contains(OriginalString, Digits, ESearchCase::IgnoreCase);
// }
//
// bool UGamedevHelperStringLibrary::ContainsOnlyLetters(const FString& OriginalString)
// {
// 	const FString Alpha = TEXT("abcdefghijklmnopqrstuvwxyz");
// 	return ContainsOnly(OriginalString, Alpha, ESearchCase::IgnoreCase);
// }
//
// bool UGamedevHelperStringLibrary::ContainsOnlyDigits(const FString& OriginalString)
// {
	// const FString Digits = TEXT("0123456789");
	// return ContainsOnly(OriginalString, Digits, ESearchCase::IgnoreCase);
// }
