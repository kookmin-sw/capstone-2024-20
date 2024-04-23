// Fill out your copyright notice in the Description page of Project Settings.


#include "MappingContextSwitcher.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

void UMappingContextSwitcher::ClearPrevMappingContext()
{
	if (CheckSubsystemNull() == true)
		return;

	for (const auto MappingContext : InputMappingContexts)
	{
		Subsystem->RemoveMappingContext(MappingContext);
	}

	InputMappingContexts.Empty();
}

bool UMappingContextSwitcher::CheckSubsystemNull() const
{
	return Subsystem;
}

void UMappingContextSwitcher::Init(const APlayerController* PlayerController)
{
	Subsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
}

void UMappingContextSwitcher::ReplaceMappingContext(TArray<UInputMappingContext*> NewMappingContexts)
{
	if (CheckSubsystemNull() == true)
		return;
	
	ClearPrevMappingContext();
	
	for (const auto MappingContext : NewMappingContexts)
	{
		Subsystem->AddMappingContext(MappingContext, 1);
	}

	InputMappingContexts.Append(NewMappingContexts);
}
