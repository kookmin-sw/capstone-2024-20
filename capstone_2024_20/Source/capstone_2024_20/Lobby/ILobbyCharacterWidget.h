#pragma once

class CAPSTONE_2024_20_API ILobbyCharacterWidget
{
public:
	virtual ~ILobbyCharacterWidget() = default;
	
	virtual void ChangeColor(FLinearColor color) = 0;

	virtual void SetName(FString Name) = 0;

	virtual void SetVisibilityFromBool(bool IsVisible) = 0;
};

