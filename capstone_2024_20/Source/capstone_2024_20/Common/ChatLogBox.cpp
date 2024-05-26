// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatLogBox.h"

#include "Components/TextBlock.h"

void UChatLogBox::SetTitleAndDetail(FString& NewTitle, FString& NewDetail)
{
	Title->SetText(FText::FromString(NewTitle));
	Detail->SetText(FText::FromString(NewDetail));
}
