// Fill out your copyright notice in the Description page of Project Settings.


#include "MappingContextSwitcher.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

UMappingContextSwitcher::UMappingContextSwitcher()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMappingContextSwitcher::ClearPrevMappingContext()
{
	if (CheckSubsystemNull() == true)
		return;

	for (const auto& MappingContext : InputMappingContexts)
	{
		Subsystem->RemoveMappingContext(MappingContext);
	}

	InputMappingContexts.Empty();
}

bool UMappingContextSwitcher::CheckSubsystemNull()
{
	if (Subsystem == nullptr)
	{
		APlayerController* PlayerController = GetOwner<APlayerController>();
		Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	}

	return Subsystem == nullptr;
}

void UMappingContextSwitcher::ReplaceMappingContext(UInputMappingContext* NewMappingContext)
{
	TArray<UInputMappingContext*> NewMappingContexts;
	NewMappingContexts.Add(NewMappingContext);

	ReplaceMappingContext(NewMappingContexts);
}

void UMappingContextSwitcher::ReplaceMappingContext(TArray<UInputMappingContext*> NewMappingContexts)
{
	if (CheckSubsystemNull() == true)
		return;

	ClearPrevMappingContext();

	for (const auto& MappingContext : NewMappingContexts)
	{
		Subsystem->AddMappingContext(MappingContext, 1);
	}

	InputMappingContexts.Append(NewMappingContexts);
}
