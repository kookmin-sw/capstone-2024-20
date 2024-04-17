#pragma once

class CAPSTONE_2024_20_API ILobbyReadyWidget
{
public:
	virtual ~ILobbyReadyWidget() = default;
	
	virtual void ChangeColor(FLinearColor color) = 0;

	virtual void SetVisibilityFromBool(bool IsVisible) = 0;
};

