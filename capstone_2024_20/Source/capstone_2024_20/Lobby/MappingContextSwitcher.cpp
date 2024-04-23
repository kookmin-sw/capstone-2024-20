// Fill out your copyright notice in the Description page of Project Settings.


#include "MappingContextSwitcher.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

void UMappingContextSwitcher::Init(APlayerController* PlayerController)
{
}

void UMappingContextSwitcher::ReplaceMappingContext(TArray<UInputMappingContext*> NewMapingContexts)
{
	if(Subsystem == nullptr)
		return;
	
	for(const auto MappingContext: NewMapingContexts)
	{
		Subsystem->AddMappingContext(MappingContext, 1);
	}
}
