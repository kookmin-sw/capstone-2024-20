#include "ShipControlStrategy.h"
#include "MyPlayerController.h"
#include "MyShip.h"

void ShipControlStrategy::Move(const FInputActionInstance& Instance, AActor* Actor,APlayerController* PlayerController, float DeltaTime)
{
	if (const AMyShip* Ship = Cast<AMyShip>(Actor))
	{
		if(const FVector2D MoveVec = Instance.GetValue().Get<FVector2D>(); MoveVec.Y > 0)
		{
			const FVector ForwardDirection = Ship->GetActorForwardVector();
			const FVector NewLoc = ForwardDirection * MoveVec.Y * Ship->GetMoveSpeed() * DeltaTime;
			
			AMyPlayerController* Controller = Cast<AMyPlayerController>(PlayerController);
			Controller->ServerRPC_MoveShip_Loc(NewLoc);
			
			if(MoveVec.X != 0)
			{
				int InputDirection;
				// 입력 방향을 결정합니다 (-1, 1)
				if(MoveVec.X > 0)
					InputDirection = 1;
				else
					InputDirection = -1;

				
				// 입력 방향이 마지막과 같은 경우, 회전 속도를 증가시킵니다.
				if(InputDirection == LastInputDirection)
				{
					CurrentRotationSpeed = FMath::Min(CurrentRotationSpeed + Ship->GetRotationAcceleration() * DeltaTime, MaxRotationSpeed);
				}
				else
				{
					// 입력 방향이 바뀌었거나 첫 입력인 경우, 회전 속도를 초기화합니다.
					CurrentRotationSpeed = 1.0f;
				}
				
				Controller->ServerRPC_MoveShip_Rot(InputDirection, CurrentRotationSpeed);
				
				// 마지막 입력 방향을 업데이트합니다.
				LastInputDirection = InputDirection;
			}
			else
			{
				// 입력이 없을 경우, 회전 속도와 마지막 입력 방향을 초기화합니다.
				CurrentRotationSpeed = 1.0f;
				LastInputDirection = 0;
			}
		}
	}
}
