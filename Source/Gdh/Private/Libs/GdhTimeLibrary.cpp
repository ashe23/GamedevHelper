// Copyright Ashot Barkhudaryan All Rights Reserved.

#include "Libs/GdhTimeLibrary.h"
// Engine Headers
#include "Kismet/KismetStringLibrary.h"

FString UGdhTimeLibrary::GetHumanReadableTime(const double Seconds)
{
	constexpr double SecondsInYear = 365.0f * 24 * 60 * 60;

	double TotalSeconds = FMath::Clamp(Seconds, 0.0f, SecondsInYear); // clamping from 0 secs to 1 year seconds

	if (TotalSeconds < 1)
	{
		return FString::Printf(TEXT("less then a sec"));
	}

	if (TotalSeconds > SecondsInYear)
	{
		return FString::Printf(TEXT("more then a year"));
	}

	TMap<FString, int32> Intervals;
	Intervals.Reserve(7);

	Intervals.Add(TEXT("year"), SecondsInYear);
	Intervals.Add(TEXT("month"), 31 * 24 * 60 * 60);
	Intervals.Add(TEXT("week"), 7 * 24 * 60 * 60);
	Intervals.Add(TEXT("day"), 24 * 60 * 60);
	Intervals.Add(TEXT("hour"), 60 * 60);
	Intervals.Add(TEXT("minute"), 60);
	Intervals.Add(TEXT("second"), 1);

	TArray<FString> Parts;
	Parts.Reserve(Intervals.Num());

	for (const auto& Interval : Intervals)
	{
		const int32 Amount = FMath::FloorToInt(TotalSeconds / Interval.Value);
		TotalSeconds -= Interval.Value * Amount;

		if (Amount != 0)
		{
			const FString TimeForm = Amount > 1 ? Interval.Key + TEXT("s") : Interval.Key;
			Parts.Add(FString::Printf(TEXT("%d %s"), Amount, *TimeForm));
		}
	}

	return UKismetStringLibrary::JoinStringArray(Parts, TEXT(" "));
}
