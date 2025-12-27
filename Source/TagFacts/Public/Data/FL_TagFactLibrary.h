// Copyright (C) Varian Daemon 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CoreTagFactData.h"
#include "..\Core\FactSubSystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FL_TagFactLibrary.generated.h"



UCLASS()
class TAGFACTS_API UFL_TagFactLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**Compare a facts value against @Value using the selected comparator.
	 * If the @Comparator is set to @IsTrue or @IsFalse, the value is ignored. */
	UFUNCTION(Category = "TagFacts|Comparitors", BlueprintCallable, BlueprintPure)
	static bool CompareFact(FGameplayTag Fact, TEnumAsByte<EFactComparator> Comparator, int32 Value);
	
	/**Compare a facts value against @Value using the selected comparator.
	 * The @Comparators options @IsTrue and @IsFalse are not supported in this function. Use @CompareFact instead*/
	UFUNCTION(Category = "TagFacts|Comparitors", BlueprintCallable, BlueprintPure)
	static bool CompareFactAgainstOtherFact(FGameplayTag Fact, TEnumAsByte<EFactComparator> Comparator, FGameplayTag OtherFact);
};
