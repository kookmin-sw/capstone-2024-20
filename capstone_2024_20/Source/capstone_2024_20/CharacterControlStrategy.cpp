// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterControlStrategy.h"

#include "MyCharacter.h"

void CharacterControlStrategy::Move(const FInputActionInstance& Instance, AActor* Actor,APlayerController* PlayerController, float DeltaTime)
{
	AMyCharacter* Character = Cast<AMyCharacter>(Actor);
	if (Character)
	{
		FVector2D MoveVec = Instance.GetValue().Get<FVector2D>();

		const FRotator Rotation = Character->GetControlRotation();
		const FRotator YawRotation(0,Rotation.Yaw,0);
		const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// 이동 입력을 기반으로 실제 이동 벡터 계산
		if (!MoveVec.IsNearlyZero())
		{
			float TargetYaw = 0.0f;
			if (MoveVec.X > 0) {
				if (MoveVec.Y > 0) TargetYaw = 45.0f;       // 오른쪽 위
				else if (MoveVec.Y < 0) TargetYaw = 135.0f; // 오른쪽 아래
				else TargetYaw = 90.0f;                      // 오른쪽
			} else if (MoveVec.X < 0) {
				if (MoveVec.Y > 0) TargetYaw = -45.0f;      // 왼쪽 위
				else if (MoveVec.Y < 0) TargetYaw = -135.0f;// 왼쪽 아래
				else TargetYaw = -90.0f;                    // 왼쪽
			} else {
				if (MoveVec.Y > 0) TargetYaw = 0.0f;       // 위
				else if (MoveVec.Y < 0) TargetYaw = 180.0f; // 아래
			}

			// 메시 컴포넌트에 회전 적용
			USkeletalMeshComponent* MeshComponent = Character->GetMesh();
			if (MeshComponent)
			{
				FRotator NewRotation = FRotator(0.0f, TargetYaw, 0.0f);
				Character->ServerRPC_MeshRotation(NewRotation);
				//MeshComponent->SetWorldRotation(NewRotation);
			}
		}
        
		Character->AddMovementInput(ForwardDir, MoveVec.Y);
		Character->AddMovementInput(RightDir, MoveVec.X);
	}
}
