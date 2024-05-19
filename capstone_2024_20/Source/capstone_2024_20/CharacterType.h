#pragma once

UENUM()
enum ECharacterType
{
	Character1,
	Character2,
	Character3,
};

namespace FCharacterTypePath
{
	const FString Character1 = TEXT("/Game/Blueprints/Character/BP_Character1.BP_Character1_C");
	const FString Character2 = TEXT("/Game/Blueprints/Character/BP_Character2.BP_Character2_C");
	const FString Character3 = TEXT("/Game/Blueprints/Character/BP_Character3.BP_Character3_C");
}