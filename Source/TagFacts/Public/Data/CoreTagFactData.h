// Copyright (C) Varian Daemon 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CoreTagFactData.generated.h"

UENUM(BlueprintType)
enum EFactComparator
{
	//Is the value greater than 0
	IsTrue,
	IsFactGreaterThan,
	IsFactLessThan,
	IsFactEqualTo,
	//Is the value less or equal to 0?
	IsFalse
};

//V: This is unused, remove this?
UENUM()
enum EFactType
{
	Value,
	Fact
};

USTRUCT(BlueprintType)
struct FFact
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fact")
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fact")
	int32 Value = 0;

	bool operator==(const FFact& Argument) const
	{
		return Argument.Tag == Tag;
	}
};
FORCEINLINE uint32 GetTypeHash(const FFact& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(Thing.Tag));
}

USTRUCT(BlueprintType)
struct TAGFACTS_API FFactCheck
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fact")
	FGameplayTag Fact;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fact")
	TEnumAsByte<EFactComparator> CheckToPerform = IsTrue;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fact")
	TEnumAsByte<EFactType> Type = Value;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fact", meta = (EditCondition="Type == EFactType::Fact", EditConditionHides))
	FGameplayTag FactToCompare;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fact", meta = (EditCondition="Type == EFactType::Value", EditConditionHides))
	int32 ValueToCompare = 0;

	bool Evaluate();
};
