// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBed.h"

FVector AMyBed::GetSleepLocation()
{
	return SleepArrow->GetComponentLocation();
}

FRotator AMyBed::GetSleepRotation()
{
	return SleepArrow->GetComponentRotation();
}

FVector AMyBed::GetAwakeLocation()
{
	return AwakeArrow->GetComponentLocation();
}

FRotator AMyBed::GetAwakeRotation()
{
	return AwakeArrow->GetComponentRotation();
}


