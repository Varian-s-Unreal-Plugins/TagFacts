// Copyright (C) Varian Daemon 2025. All Rights Reserved.

#include "Data/FL_TagFactLibrary.h"

#include "Core/FactSubSystem.h"

bool UFL_TagFactLibrary::CompareFact(FGameplayTag Fact, TEnumAsByte<EFactComparator> Comparator, int32 Value)
{
	int32 FactValue = UFactSubSystem::GetFactValue(Fact);
	
	switch (Comparator)
	{
	case IsTrue:
		return FactValue > 0;

	case IsFactEqualTo:
		return FactValue == Value;

	case IsFactGreaterThan:
		return FactValue > Value;
	
	case IsFactLessThan:
		return FactValue < Value;

	case IsFalse:
		return FactValue <= 0;
	default:
		return false;
	}
}

bool UFL_TagFactLibrary::CompareFactAgainstOtherFact(FGameplayTag Fact, TEnumAsByte<EFactComparator> Comparator,
	FGameplayTag OtherFact)
{
	int32 FactValue = UFactSubSystem::GetFactValue(Fact);
	int32 OtherFactValue = UFactSubSystem::GetFactValue(OtherFact);
	
	switch (Comparator)
	{
	case IsFactEqualTo:
		return FactValue == OtherFactValue;

	case IsFactGreaterThan:
		return FactValue > OtherFactValue;
	
	case IsFactLessThan:
		return FactValue < OtherFactValue;
	default:
		return false;
	}
}
