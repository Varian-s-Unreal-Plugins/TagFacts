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

	UFUNCTION(Category = "TagFacts|Comparitors", BlueprintCallable, BlueprintPure)
	static bool CompareFact(FFact Fact, int32 Value, TEnumAsByte<EFactComparator> Comparator);
};
