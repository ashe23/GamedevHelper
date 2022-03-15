// Copyright Ashot Barkhudaryan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Net/Core/PushModel/PushModel.h"
#include "GamedevHelperArrayLibrary.generated.h"

UCLASS()
class UGamedevHelperArrayLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (DisplayName = "Shuffle Array With Given Seed", CompactNodeTitle = "SHUFFLE_SEED", ArrayParm = "TargetArray"), Category = "GamedevHelper|Array")
	static void Array_ShuffleWithSeed(const TArray<int32>& TargetArray, const int32 Seed);
	static void GenericArray_ShuffleWithSeed(void* TargetArray, const FArrayProperty* ArrayProp, const int32 Seed);
public:
	DECLARE_FUNCTION(execArray_ShuffleWithSeed)
	{
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FArrayProperty>(nullptr);
		void* ArrayAddress = Stack.MostRecentPropertyAddress;
		FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}
		P_GET_PROPERTY(FIntProperty, Seed);
		P_FINISH;
		P_NATIVE_BEGIN;
		MARK_PROPERTY_DIRTY(Stack.Object, ArrayProperty);
		GenericArray_ShuffleWithSeed(ArrayAddress, ArrayProperty, Seed);
		P_NATIVE_END;
	}
};