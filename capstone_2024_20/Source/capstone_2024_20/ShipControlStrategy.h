#pragma once

#include "CoreMinimal.h"
#include "IControlStrategy.h"

class CAPSTONE_2024_20_API ShipControlStrategy : public IControlStrategy
{
public:
	virtual void Move(const FInputActionInstance& Instance, AActor* Actor,APlayerController* PlayerController, float DeltaTime) override;

	float CurrentRotationSpeed = 1.0f; // 현재 회전 속도, 초기값 1
	float MaxRotationSpeed = 10.0f; // 최대 회전 속도
	int LastInputDirection = 0; // 마지막 입력 방향, -1: 왼쪽, 0: 없음, 1: 오른쪽
};
