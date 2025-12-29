// Copyright (C) Varian Daemon 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/CoreTagFactData.h"
#include "FactSubSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFactAdded, FFact, NewFact);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFactRemoved, FFact, RemovedFact);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFactIncremented, FFact, Fact, int32, OldValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFactDecremented, FFact, Fact, int32, OldValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFactOverriden, int32, OldValue, int32, NewValue);

USTRUCT(BlueprintType)
struct FFactMessage
{
	GENERATED_BODY()

	UPROPERTY(Category = "Tag Fact", EditAnywhere, BlueprintReadWrite)
	FGameplayTag Fact;

	UPROPERTY(Category = "Tag Fact", EditAnywhere, BlueprintReadWrite)
	int32 OldValue = 0;

	UPROPERTY(Category = "Tag Fact", EditAnywhere, BlueprintReadWrite)
	int32 NewValue = 0;
};

UCLASS()
class TAGFACTS_API UFactSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:

	UFactSubSystem();
	
	UPROPERTY(SaveGame)
	TSet<FFact> Facts;

public:

	UPROPERTY(Category = "Fact System", BlueprintAssignable)
	FFactAdded FactAdded;

	UPROPERTY(Category = "Fact System", BlueprintAssignable)
	FFactRemoved FactRemoved;

	UPROPERTY(Category = "Fact System", BlueprintAssignable)
	FFactIncremented FactIncremented;

	UPROPERTY(Category = "Fact System", BlueprintAssignable)
	FFactDecremented FactDecremented;

	UPROPERTY(Category = "Fact System", BlueprintAssignable)
	FFactOverriden FactOverriden;

	static UFactSubSystem* Get();

	/**The fact array is private because you are supposed to interact
	 * with it through the helper functions.*/
	UFUNCTION(Category = "Fact System", BlueprintCallable, BlueprintPure)
	static TSet<FFact> GetFacts();

	UFUNCTION(Category = "Fact System", BlueprintCallable, BlueprintPure)
	static FGameplayTagContainer GetFactsAsTagContainer();

	/**Completely override the current facts with a new list.
	 * Primarily used for loading from a save.*/
	UFUNCTION(Category = "Fact System", BlueprintCallable)
	static void SetFacts(TSet<FFact> NewFacts);

	/**Add a fact to the system. This will return false if the fact
	 * did not already exist. Will only return true if this function
	 * call added it to the array.*/
	UFUNCTION(Category = "Fact System", BlueprintCallable)
	static bool AddFact(FFact Fact);

	/**Remove a fact from the system. Will only return true if the fact
	 * was successfully removed from the system.*/
	UFUNCTION(Category = "Fact System", BlueprintCallable)
	static bool RemoveFact(FGameplayTag Fact);

	/**Increment a @Fact by a set @Amount.*/
	UFUNCTION(Category = "Fact System", BlueprintCallable)
	static void IncrementFact(const FGameplayTag Fact, const int32 Amount = 1, bool AddIfMissing = true);

	/**Decrement a @Fact by a set @Amount.*/
	UFUNCTION(Category = "Fact System", BlueprintCallable)
	static void DecrementFact(const FGameplayTag Fact, const int32 Amount = 1);

	/**Not advised to be used. This will forcibly set the fact value.*/
	UFUNCTION(Category = "Fact System", BlueprintCallable)
	static bool SetFactValue(FFact Fact, bool AddIfMissing = true);

	/**Check the fact array and find out if the tag can be found.*/
	UFUNCTION(Category = "Fact System", BlueprintCallable, BlueprintPure)
	static bool DoesFactExist(FGameplayTag Fact);

	/**Get the value of a fact. Will return 0 if the fact is not found.
	 * If you need to find out if a fact has a value, use DoesFactExist instead.*/
	UFUNCTION(Category = "Fact System", BlueprintCallable, BlueprintPure)
	static int32 GetFactValue(FGameplayTag Fact);

	
#if WITH_EDITOR
	
	UFUNCTION(Category = "Fact System|Console Commands", Exec)
	static void SetFact(const TArray<FString>& Args);

#endif
};
