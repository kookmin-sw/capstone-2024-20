#include "Trigger.h"

#include "capstone_2024_20/MyShip.h"
#include "capstone_2024_20/Sailing/SailingSystem.h"

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
		IsTriggered = [this]()
		{
			return SailingSystem->IsReachedDestination();
		};
	}
	else if (Id == "T_0002")
	{
		// Todo@autumn - This is a temporary solution, replace it with data.
		IsTriggered = [this]()
		{
			return SailingSystem->GetMyShip()->GetCurrentHP() <= 0;	
		};
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Trigger Id"));
	}
}
