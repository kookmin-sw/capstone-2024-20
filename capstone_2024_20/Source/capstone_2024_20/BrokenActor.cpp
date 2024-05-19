// Fill out your copyright notice in the Description page of Project Settings.


#include "BrokenActor.h"

// Sets default values
ABrokenActor::ABrokenActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABrokenActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABrokenActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

