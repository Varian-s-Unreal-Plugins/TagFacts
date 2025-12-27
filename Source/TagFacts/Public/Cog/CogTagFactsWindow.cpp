// Copyright (C) Varian Daemon 2025. All Rights Reserved.

#include "CogTagFactsWindow.h"

#if COG_INSTALLED && ENABLE_COG

#include "GameplayTagsManager.h"
#include "Core/FactSubSystem.h"

void FCogTagFactsWindow::RenderHelp()
{
	ImGui::Text(
	"Add, remove, increment or decrement facts inside the fact database."
	);
}

void FCogTagFactsWindow::RenderContent()
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

	//Start recursively processing root tags and their children
	if(RootTags.Num() > 0)
	{
		for(const FGameplayTag& RootTag : RootTags)
		{
			TSharedPtr<FGameplayTagNode> RootNode = TagManager.FindTagNode(RootTag);
			if(RootNode.IsValid())
			{
				ProcessGameplayTagNodeRecursive(RootNode, 0, FactSubSystem);
			}
		}
	}
}

void FCogTagFactsWindow::ProcessGameplayTagNodeRecursive(TSharedPtr<FGameplayTagNode> Node,
	ImGuiTreeNodeFlags ParentFlags, UFactSubSystem* FactSubSystem)
{
	if(!Node.IsValid())
	{
		return;
	}

	//Retrieve the tag
	FGameplayTag CurrentTag = Node->GetCompleteTag();
	if(!CurrentTag.IsValid())
	{
		return;
	}
	FString CurrentTagString = CurrentTag.ToString();

	//Set flags for the current node
	ImGuiTreeNodeFlags NodeFlags = ParentFlags;
	if(Node->GetChildTagNodes().Num() == 0)
	{
		//No children, mark as leaf
		NodeFlags |= ImGuiTreeNodeFlags_Leaf;
	}

	//Create the tree
	ImGui::AlignTextToFramePadding();
	ImGui::PushID(TCHAR_TO_ANSI(*CurrentTagString));
	CurrentTagString.Split(".", nullptr, &CurrentTagString, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	const bool TreeOpen = ImGui::TreeNodeEx(TCHAR_TO_ANSI(*CurrentTagString), NodeFlags);
	ImGui::SameLine();
	if(ImGui::Button("-"))
	{
		FactSubSystem->DecrementFact(CurrentTag);
	}
	ImGui::SameLine();
	FString TagValue = FString::FromInt(FactSubSystem->GetFactValue(CurrentTag));
	ImGui::Text(TCHAR_TO_ANSI(*TagValue));
	ImGui::SameLine();
	if(ImGui::Button("+"))
	{
		FactSubSystem->IncrementFact(CurrentTag);
	}
	ImGui::PopID();
	if(TreeOpen)
	{
		//Process children recursively
		TArray<TSharedPtr<FGameplayTagNode>>& ChildrenNodes = Node->GetChildTagNodes();

		//Sort children alphabetically
		ChildrenNodes.Sort([](const TSharedPtr<FGameplayTagNode>& A, const TSharedPtr<FGameplayTagNode>& B)
		{
			return A->GetSimpleTagName().ToString() < B->GetSimpleTagName().ToString();
		});

		//Recursive call for each child
		for (const TSharedPtr<FGameplayTagNode>& ChildNode : ChildrenNodes)
		{
			ProcessGameplayTagNodeRecursive(ChildNode, 0, FactSubSystem);
		}
		
		ImGui::TreePop();
	}
}

#endif
