// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatLogBox.h"

#include "Components/TextBlock.h"

void UChatLogBox::SetTitleAndDetail(const FString& NewTitle, const FString& NewDetail)
{
	Title->SetText(FText::FromString(NewTitle + TEXT(":")));
	Detail->SetText(FText::FromString(NewDetail));
}
