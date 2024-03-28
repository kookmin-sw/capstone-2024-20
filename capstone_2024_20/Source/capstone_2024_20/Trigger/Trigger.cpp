#include "Trigger.h"

UTrigger::UTrigger(): IsTriggered([]() { return false; }), SailingSystem(nullptr)
{
	// do nothing
}

void UTrigger::Initialize(const FString& Id, ASailingSystem* InSailingSystem)
{
	if (InSailingSystem == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("InSailingSystem is nullptr"));
		return;
	}

	if (Id.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Id is empty"));
		return;
	}
	
	SailingSystem = InSailingSystem;
	
	if (Id == "T_0001")
	{
		// Todo@autumn - This is a temporary solution, replace it with data.
		IsTriggered = [](){return true;};
	}
	else if (Id == "T_0002")
	{
		// Todo@autumn - This is a temporary solution, replace it with data.
		IsTriggered = [](){return false;};
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Trigger Id"));
	}
}
