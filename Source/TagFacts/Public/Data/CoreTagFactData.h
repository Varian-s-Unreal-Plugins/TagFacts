// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CoreTagFactData.generated.h"

UENUM(BlueprintType)
enum EFactComparator
{
	IsTrue, //Is the value greater than 0
	IsFactGreaterThan,
	IsFactLessThan,
	IsFactEqualTo,
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

//V: This is unused, remove this?
USTRUCT(BlueprintType)
struct FFactCheck
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fact")
	FFact Fact;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fact")
	TEnumAsByte<EFactComparator> CheckToPerform = IsTrue;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fact")
	TEnumAsByte<EFactType> Type = Value;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fact", meta = (EditCondition="Type == EFactType::Fact", EditConditionHides))
	FFact FactToCompare;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fact", meta = (EditCondition="Type == EFactType::Value", EditConditionHides))
	int32 ValueToCompare = 0;
};
