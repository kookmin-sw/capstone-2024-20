// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCannonBallBox.h"

void AMyCannonBallBox::SpawnCarryCannonBall(AMyCharacter* user)
{
	if(user)
	{
		// 캐릭터 앞 방향에 캐논볼을 소환할 위치 및 회전 계산
		FVector SpawnLocation = user->GetActorLocation() + user->GetMesh()->GetRightVector() * 100.0f + user->GetMesh()->GetUpVector() * 50.0f;
		FRotator SpawnRotation = user->GetActorRotation();

	
		// 캐논볼 소환
		if(BP_CarryCannonBallClass)
		{
			SpawnedCannonBall = GetWorld()->SpawnActor<AActor>(BP_CarryCannonBallClass, SpawnLocation, SpawnRotation);

			if (SpawnedCannonBall != nullptr)
			{
				// 캐논볼을 캐릭터의 RootComponent에 붙입니다.
				SpawnedCannonBall->AttachToComponent(user->GetMesh(),FAttachmentTransformRules::KeepWorldTransform);
			}
			user->SetCurrentCarryObject(SpawnedCannonBall);
		}
		
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("제대로 생성됨"));
	}
	else
	{

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("없어용"));
	}
}
