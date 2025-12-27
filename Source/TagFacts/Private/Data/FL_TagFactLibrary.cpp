// Copyright (C) Varian Daemon 2025. All Rights Reserved.


#include "Data/FL_TagFactLibrary.h"

#include "Kismet/GameplayStatics.h"

bool UFL_TagFactLibrary::CompareFact(FFact Fact, int32 Value, TEnumAsByte<EFactComparator> Comparator)
{
	switch (Comparator)
	{
	case IsTrue:
		return Fact.Value > 0;

	case IsFactEqualTo:
		return Fact.Value == Value;

	case IsFactGreaterThan:
		return Fact.Value > Value;
	
	case IsFactLessThan:
		return Fact.Value < Value;

	default:
		return false;
	}
}
