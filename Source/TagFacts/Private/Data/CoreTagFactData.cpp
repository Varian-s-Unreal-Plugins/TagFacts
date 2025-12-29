#include "Data/CoreTagFactData.h"

#include "Core/FactSubSystem.h"
#include "Data/FL_TagFactLibrary.h"

bool FFactCheck::Evaluate()
{
	UFactSubSystem* FactSubSystem = UFactSubSystem::Get();
	if(!FactSubSystem)
	{
		return false;
	}
		
	if(Type == EFactType::Fact)
	{
		return UFL_TagFactLibrary::CompareFactAgainstOtherFact(Fact, CheckToPerform, FactToCompare);
	}
	else
	{
		return UFL_TagFactLibrary::CompareFact(Fact, CheckToPerform, ValueToCompare);
	}
}
