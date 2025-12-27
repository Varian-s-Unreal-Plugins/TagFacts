// Copyright (C) Varian Daemon 2025. All Rights Reserved.

#pragma once

#if COG_INSTALLED

#include "CogCommon.h"
#if ENABLE_COG

#include "CogWindow.h"

class UFactSubSystem;
struct FGameplayTagNode;

class TAGFACTS_API FCogTagFactsWindow : public FCogWindow
{
public:

	virtual void RenderHelp() override;

	virtual void RenderContent() override;

	void ProcessGameplayTagNodeRecursive(TSharedPtr<FGameplayTagNode> Node, ImGuiTreeNodeFlags ParentFlags, UFactSubSystem* FactSubSystem);

};

#endif
#endif