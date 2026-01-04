// Copyright (C) Varian Daemon 2025. All Rights Reserved.


#include "Core/FactSubSystem.h"

#if AsyncMessageSystem_Enabled
#include "AsyncMessageSystemBase.h"
#include "AsyncMessageWorldSubsystem.h"
#endif
#include "GameplayTagsManager.h"
#include "OmniRuntimeMacros.h"
#include "TagFacts.h"
#include "Kismet/GameplayStatics.h"

#if WITH_EDITOR
#include "Misc/DefaultValueHelper.h"
#endif

UFactSubSystem::UFactSubSystem()
{
#if WITH_EDITOR
	if(GEngine)
	{
		IConsoleManager::Get().RegisterConsoleCommand(
			TEXT("SetFact"),
			TEXT("Set the value of a fact. If 0, remove the fact"),
			FConsoleCommandWithArgsDelegate::CreateStatic(&SetFact),
			ECVF_Default);
	}
#endif
}

UFactSubSystem* UFactSubSystem::Get()
{
	if(!GEngine->GameViewport) { return nullptr; }
	const UWorld* World = GEngine->GameViewport->GetWorld();
	if(!World) { return nullptr; }
	if(!UGameplayStatics::GetGameInstance(World)){ return nullptr; }
	if(UFactSubSystem* SubSystem = UGameplayStatics::GetGameInstance(World)->GetSubsystem<UFactSubSystem>())
	{
		return SubSystem;
	}

	return nullptr;
}

TSet<FFact> UFactSubSystem::GetFacts()
{
	TSet<FFact> EmptySet;

	UFactSubSystem* SubSystem = UFactSubSystem::Get();
	if(!SubSystem)
	{
		return EmptySet;
	}
	
	return SubSystem->Facts;
}

FGameplayTagContainer UFactSubSystem::GetFactsAsTagContainer()
{
	UFactSubSystem* SubSystem = UFactSubSystem::Get();
	FGameplayTagContainer TagContainer;
	for(auto& CurrentFact : SubSystem->GetFacts())
	{
		TagContainer.AddTag(CurrentFact.Tag);
	}
	return TagContainer;
}

void UFactSubSystem::SetFacts(TSet<FFact> NewFacts)
{
	UFactSubSystem* SubSystem = UFactSubSystem::Get();
	if(!SubSystem)
	{
		return;
	}

	UE_LOG(LogTagFacts, Log, TEXT("Facts successfully overriden"))
	
	SubSystem->Facts = NewFacts;
}

bool UFactSubSystem::AddFact(FFact Fact)
{
	UFactSubSystem* SubSystem = UFactSubSystem::Get();
	if(!SubSystem)
	{
		return false;
	}
	
	if(!SubSystem->Facts.Contains(Fact))
	{
		UE_LOG(LogTagFacts, Log, TEXT("Fact added: %s with value of %d"), *Fact.Tag.ToString(), Fact.Value)
		SubSystem->Facts.Add(Fact);
		SubSystem->FactAdded.Broadcast(Fact);
		
		#if AsyncMessageSystem_Enabled
		if(TSharedPtr<FAsyncMessageSystemBase> Sys = UAsyncMessageWorldSubsystem::GetSharedMessageSystem(SubSystem->GetWorld()))
		{
			FInstancedStruct Message = FInstancedStruct::Make(FFactMessage());
			Message.GetMutablePtr<FFactMessage>()->Fact = Fact.Tag;
			Message.GetMutablePtr<FFactMessage>()->OldValue = 0;
			Message.GetMutablePtr<FFactMessage>()->NewValue = Fact.Value;
			Sys->QueueMessageForBroadcast(
				FAsyncMessageId(Fact.Tag), 
				Message);
		}
		#endif
		
		return true;
	}

	return false;
}

bool UFactSubSystem::RemoveFact(FGameplayTag Fact)
{
	UFactSubSystem* SubSystem = UFactSubSystem::Get();
	if(!SubSystem)
	{
		return false;
	}

	if(const FFact* FoundFact = SubSystem->Facts.Find(FFact({Fact})))
	{
		UE_LOG(LogTagFacts, Log, TEXT("Fact removed: %s"), *Fact.ToString())
		SubSystem->Facts.Remove(FFact({Fact}));
		SubSystem->FactRemoved.Broadcast(FFact({FoundFact->Tag, FoundFact->Value}));
		
		#if AsyncMessageSystem_Enabled
		if(TSharedPtr<FAsyncMessageSystemBase> Sys = UAsyncMessageWorldSubsystem::GetSharedMessageSystem(SubSystem->GetWorld()))
		{
			FInstancedStruct Message = FInstancedStruct::Make(FFactMessage());
			Message.GetMutablePtr<FFactMessage>()->Fact = Fact;
			Message.GetMutablePtr<FFactMessage>()->OldValue = FoundFact->Value;
			Message.GetMutablePtr<FFactMessage>()->NewValue = 0;
			Sys->QueueMessageForBroadcast(
				FAsyncMessageId(Fact), 
				Message);
		}
		#endif
		
		return true;
	}

	return false;
}

void UFactSubSystem::IncrementFact(const FGameplayTag Fact, const int32 Amount, bool AddIfMissing)
{
	if(!Fact.IsValid() || Amount < 1)
	{
		return;
	}
	
	UFactSubSystem* SubSystem = UFactSubSystem::Get();
	if(!SubSystem)
	{
		return;
	}
	
	if(Amount == 0)
	{
		return;
	}
	
	if(FFact* FoundFact = SubSystem->Facts.Find(FFact({Fact})))
	{
		const int32 OldValue = FoundFact->Value;
		FoundFact->Value = FoundFact->Value + Amount;
		UE_LOG(LogTagFacts, Log, TEXT("Fact incremented: %s - old value: %d - new value: %d"), *Fact.ToString(), OldValue, FoundFact->Value)
		SubSystem->FactIncremented.Broadcast(FFact({FoundFact->Tag, FoundFact->Value}), OldValue);
		
		#if AsyncMessageSystem_Enabled
		if(TSharedPtr<FAsyncMessageSystemBase> Sys = UAsyncMessageWorldSubsystem::GetSharedMessageSystem(SubSystem->GetWorld()))
		{
			FInstancedStruct Message = FInstancedStruct::Make(FFactMessage());
			Message.GetMutablePtr<FFactMessage>()->Fact = Fact;
			Message.GetMutablePtr<FFactMessage>()->OldValue = OldValue;
			Message.GetMutablePtr<FFactMessage>()->NewValue = FoundFact->Value;
			Sys->QueueMessageForBroadcast(
				FAsyncMessageId(Fact),
				Message);
		}
		#endif
	}
	else if(AddIfMissing)
	{
		AddFact(FFact({Fact, Amount}));
	}
}

void UFactSubSystem::DecrementFact(const FGameplayTag Fact, const int32 Amount)
{
	UFactSubSystem* SubSystem = UFactSubSystem::Get();
	if(!SubSystem)
	{
		return;
	}
	
	if(Amount == 0)
	{
		return;
	}
	
	if(FFact* FoundFact = SubSystem->Facts.Find(FFact({Fact})))
	{
		const int32 OldValue = FoundFact->Value;
		FoundFact->Value = FoundFact->Value - Amount;
		UE_LOG(LogTagFacts, Log, TEXT("Fact decremented: %s - old value: %d - new value: %d"), *Fact.ToString(), OldValue, FoundFact->Value)
		SubSystem->FactDecremented.Broadcast(FFact({FoundFact->Tag, FoundFact->Value}), OldValue);
		if(FoundFact->Value <= 0)
		{
			RemoveFact(Fact);
		}
		else
		{
			#if AsyncMessageSystem_Enabled
			if(TSharedPtr<FAsyncMessageSystemBase> Sys = UAsyncMessageWorldSubsystem::GetSharedMessageSystem(SubSystem->GetWorld()))
			{
				FInstancedStruct Message = FInstancedStruct::Make(FFactMessage());
				Message.GetMutablePtr<FFactMessage>()->Fact = Fact;
				Message.GetMutablePtr<FFactMessage>()->OldValue = OldValue;
				Message.GetMutablePtr<FFactMessage>()->NewValue = FoundFact->Value;
				Sys->QueueMessageForBroadcast(
					FAsyncMessageId(Fact), 
					Message);
			}
			#endif
		}
	}
}

bool UFactSubSystem::SetFactValue(FFact Fact, bool AddIfMissing)
{
	UFactSubSystem* SubSystem = UFactSubSystem::Get();
	if(!SubSystem)
	{
		return false;
	}

	FFact* FoundFact = SubSystem->Facts.Find(FFact({Fact}));
	if(!FoundFact && AddIfMissing)
	{
		AddFact(Fact);
		FoundFact = SubSystem->Facts.Find(FFact({Fact}));
	}
	
	if(FoundFact)
	{
		const int32 OldValue = FoundFact->Value;
		const int32 NewValue = Fact.Value;
		FoundFact->Value = Fact.Value;
		UE_LOG(LogTagFacts, Log, TEXT("Fact set: %s with value of: %d"), *Fact.Tag.ToString(), NewValue)
		SubSystem->FactOverriden.Broadcast(OldValue, NewValue);
	}

	return false;
}

bool UFactSubSystem::DoesFactExist(FGameplayTag Fact)
{
	UFactSubSystem* SubSystem = UFactSubSystem::Get();
	if(!SubSystem)
	{
		return false;
	}
	
	return SubSystem->Facts.Contains(FFact({Fact}));
}

int32 UFactSubSystem::GetFactValue(FGameplayTag Fact)
{
	UFactSubSystem* SubSystem = UFactSubSystem::Get();
	if(!SubSystem)
	{
		return 0;
	}
	
	if(!SubSystem->Facts.IsEmpty())
	{
		FFact* FoundFact = SubSystem->Facts.Find(FFact({Fact}));
		if(FoundFact)
		{
			return FoundFact->Value;
		}
	}

	return 0;
}

#if WITH_EDITOR

void UFactSubSystem::SetFact(const TArray<FString>& Args)
{
	UFactSubSystem* SubSystem = UFactSubSystem::Get();
	if(!SubSystem)
	{
		return;
	}
	
	if(Args.Num() != 2)
	{
		UKismetSystemLibrary::PrintString(SubSystem, "SetFact only accepts 2 arguments");
		return;
	}

	//Parse the tag
	FGameplayTag TagToSet = FGameplayTag::RequestGameplayTag(FName(*Args[0]));
	if(!TagToSet.IsValid())
	{
		UKismetSystemLibrary::PrintString(SubSystem, "Could not find Tag");
		return;
	}

	//Parse the integer value
	int32 Value;
	if (!FDefaultValueHelper::ParseInt(Args[1], Value))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Invalid integer value"));
		return;
	}

	if(Value == 0)
	{
		SubSystem->RemoveFact(TagToSet);
		return;
	}
	
	SubSystem->SetFactValue(FFact(TagToSet, Value), true);
	
}

#endif

#if OmniDebugger

Omni_RegisterDebugger(FSIMTagFacts, "Gameplay.TagFacts.Facts Control", "TagFacts.FactsControl", "Facts Control")

void FSIMTagFacts::FSIMTagFacts::Draw(float DeltaTime)
{
	const UGameplayTagsManager& TagManager = UGameplayTagsManager::Get();
	UFactSubSystem* FactSubSystem = UFactSubSystem::Get();
	if(!FactSubSystem)
	{
		return;
	}

	//Fetch all tags
	FGameplayTagContainer AllTags;
	TagManager.RequestAllGameplayTags(AllTags, false);

	//Identify root tags (no parent and no children)
	TArray<FGameplayTag> RootTags;
	for(const FGameplayTag& Tag : AllTags)
	{
		FString TagString = Tag.ToString();
		if(!TagString.Contains(".")) // No parent (no dot in the tag name)
		{
			RootTags.Add(Tag);
		}
	}

	//Sort the array alphabetically
	RootTags.Sort([](const FGameplayTag& A, const FGameplayTag& B)
	{
		return A.ToString() < B.ToString();
	});
	
	const FTableRowStyle* TableRowStyle = &FAppStyle::Get().GetWidgetStyle<FTableRowStyle>("TableView.AlternatingRow");
	SlateIM::MinWidth(500.f);
	SlateIM::MinHeight(300.f);
	SlateIM::MaxHeight(300.f);
	SlateIM::HAlign(HAlign_Right);
	SlateIM::BeginTable(nullptr, TableRowStyle);
	SlateIM::AddTableColumn(TEXT("Name"));

	//Start recursively processing root tags and their children
	if(RootTags.Num() > 0)
	{
		for(const FGameplayTag& RootTag : RootTags)
		{
			TSharedPtr<FGameplayTagNode> RootNode = TagManager.FindTagNode(RootTag);
			if(RootNode.IsValid())
			{
				ProcessGameplayTagNodeRecursive(RootNode, FactSubSystem, false);
			}
		}
	}
	
	SlateIM::EndTable();
}

void FSIMTagFacts::FSIMTagFacts::ProcessGameplayTagNodeRecursive(TSharedPtr<FGameplayTagNode> Node,
	UFactSubSystem* FactSubSystem, bool HasParent)
{
	if(!Node.IsValid())
	{
		return;
	}
	
	if(HasParent ? SlateIM::BeginTableRowChildren() : SlateIM::NextTableCell())
	{
		//Retrieve the tag
		FGameplayTag CurrentTag = Node->GetCompleteTag();
		if(!CurrentTag.IsValid())
		{
			if(HasParent)
			{
				SlateIM::EndTableRowChildren();
			}
			return;
		}
		FString CurrentTagString = CurrentTag.ToString();

		//Create the tree
		CurrentTagString.Split(".", nullptr, &CurrentTagString, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		
		SlateIM::BeginHorizontalStack();
		SlateIM::Fill();
		SlateIM::Text(*CurrentTagString);
		SlateIM::HAlign(HAlign_Right);
		if(SlateIM::Button(TEXT("-")))
		{
			FactSubSystem->DecrementFact(CurrentTag);
		}
		FString TagValue = FString::FromInt(FactSubSystem->GetFactValue(CurrentTag));
		SlateIM::Text(*TagValue);
		if(SlateIM::Button(TEXT("+")))
		{
			FactSubSystem->IncrementFact(CurrentTag);
		}
		SlateIM::EndHorizontalStack();
		
		TArray<TSharedPtr<FGameplayTagNode>>& ChildrenNodes = Node->GetChildTagNodes();
		if(ChildrenNodes.IsEmpty())
		{
			if(HasParent)
			{
				SlateIM::EndTableRowChildren();
			}
			return;
		}
	
		// if (SlateIM::BeginTableRowChildren())
		// {
		// 	
		// }
		// SlateIM::EndTableRowChildren();
		
		//Process children recursively
		//Sort children alphabetically
		ChildrenNodes.Sort([](const TSharedPtr<FGameplayTagNode>& A, const TSharedPtr<FGameplayTagNode>& B)
		{
			return A->GetSimpleTagName().ToString() < B->GetSimpleTagName().ToString();
		});
		
		//Recursive call for each child
		for (const TSharedPtr<FGameplayTagNode>& ChildNode : ChildrenNodes)
		{
			ProcessGameplayTagNodeRecursive(ChildNode, FactSubSystem, true);
		}
	}
	if(HasParent)
	{
		SlateIM::EndTableRowChildren();
	}
}
#endif
