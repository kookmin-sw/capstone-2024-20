// Fill out your copyright notice in the Description page of Project Settings.


#include "NamePlateWidgetComponent.h"

#include "NamePlateWidget.h"
#include "capstone_2024_20/MyCharacter.h"


UNamePlateWidgetComponent::UNamePlateWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<UUserWidget>
		BP_WidgetFinder(TEXT("/Game/WidgetBlueprints/Common/BP_NamePlateWidget.BP_NamePlateWidget_C"));

	if (BP_WidgetFinder.Succeeded())
	{
		SetWidgetClass(BP_WidgetFinder.Class);
		SetRelativeLocation(FVector(0, 0, 60.0f));
		SetWidgetSpace(EWidgetSpace::Screen);
	}
}

template <class T>
T UNamePlateWidgetComponent::ChangeWidget(FString& Path)
{
	UUserWidget* Widget = ChangeWidget(Path);
	return Cast<T>(Widget);
}


UUserWidget* UNamePlateWidgetComponent::ChangeWidget(FString& Path)
{
	UClass* BlueprintClass = LoadClass<UUserWidget>(nullptr, *Path);

	if (BlueprintClass)
	{
		UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(),
		                                                   BlueprintClass);

		return ChangeWidget(NewWidget);
	}

	return nullptr;
}

UUserWidget* UNamePlateWidgetComponent::ChangeWidget(UUserWidget* NewWidget)
{
	SetWidget(NewWidget);

	return NewWidget;
}

void UNamePlateWidgetComponent::SetName(const FString& Name)
{
	if (GetOwner()->HasAuthority())
	{
		MultiRPC_SetName(Name);
	}
	else
	{
		if (GetOwnerRole() == ROLE_SimulatedProxy)
		{
			// GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red,
			//                                  TEXT("Actor NamePlate SetName 조작 불가 권한 확인 바람"));
			// GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red,
			//                                  TEXT("Actor NamePlate SetName 조작 불가 권한 확인 바람"));
		}
		else
		{
			ServerRPC_SetName(Name);
		}
	}
}

void UNamePlateWidgetComponent::MultiRPC_SetName_Implementation(const FString& Name)
{
	if(IsValid(GetWidget()) == false)
			return;
	
	Cast<UNamePlateWidget>(GetWidget())->SetName(Name);
}

void UNamePlateWidgetComponent::ServerRPC_SetName_Implementation(const FString& Name)
{
	MultiRPC_SetName(Name);
}
