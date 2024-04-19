// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "CannonControlStrategy.h"
#include "CharacterControlStrategy.h"
#include "MyShip.h"
#include "ShipControlStrategy.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

class AStaticMeshActor;

AMyPlayerController::AMyPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	static ConstructorHelpers::FObjectFinder<UInputAction> AC_Move(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/Actions/Move.Move'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> AC_Interaction(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/Actions/Interaction.Interaction'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> AC_Shoot(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/Actions/Shoot.Shoot'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> AC_DraggingRotate(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/Actions/DraggingRotate.DraggingRotate'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> AC_Attack(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/Actions/AC_Attack.AC_Attack'"));
	
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_Default_Mapping(
		TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Inputs/Mappings/IMC_test.IMC_test'"));
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_Shoot_Mapping(
		TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Inputs/Mappings/IMC_Cannon.IMC_Cannon'"));

	MoveAction = AC_Move.Object;
	InteractionAction = AC_Interaction.Object;
	ShootAction = AC_Shoot.Object;
	DraggingRotateAction = AC_DraggingRotate.Object;
	AttackAction = AC_Attack.Object;
	DefaultMappingContext = IMC_Default_Mapping.Object;
	CannonMappingContext = IMC_Shoot_Mapping.Object;

	// test

	CurrentStrategy = new CharacterControlStrategy();


}




void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//플레이어 할당

	if (HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 60.0f, FColor::Emerald, TEXT("HasAutority"));
	}

	//배 할당
	TArray<AActor*> FoundShips;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyShip::StaticClass(), FoundShips);
	if (FoundShips.Num() > 0)
	{
		Ship = Cast<AMyShip>(FoundShips[0]);
		
	}

	// 매핑 컨텐스트 할당
	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem != nullptr)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	Player = Cast<AMyCharacter>(GetPawn());
	if (Player)
	{
		ControlledActor = Player;
		if (Player->InputComponent)
			SetupPlayerInputComponent(Player->InputComponent);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Player NULL"));
	}
	
	EnableCheats();
}




void AMyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!HasAuthority())
	{
		if (!Player || flag)
		{
			Player = Cast<AMyCharacter>(GetPawn());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Player NULL"));
	
			if (Player)
			{
				ControlledActor = Player;
				if (Player->InputComponent)
				{
					SetupPlayerInputComponent(Player->InputComponent);
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Player Component NOT NULL"));
					flag = false;
				}
			}
		}
	}

	// 키를 누르고 있으면 시간을 측정
	if (bIsPressingKey)
	{
		if(CurrentHitObject == Player->GetCurrentHitObject()) // 상호작용 시작 시 오브젝트와 일치하면 시간 증가
		{
			PressDuration += GetWorld()->DeltaTimeSeconds;
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Interaction - PressDuration: %f"), PressDuration));
		}
		else // 상호작용 시작 시 오브젝트와 일치하지 않으면 (이동했으면) 초기화 
		{
			PressDuration = 0.0f;
		}
		
	}
	
}


void AMyPlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (Input != nullptr)
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Move);
		Input->BindAction(InteractionAction, ETriggerEvent::Started, this, &AMyPlayerController::Interaction_Pressed);
		Input->BindAction(InteractionAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Interaction_Trigger);
		Input->BindAction(InteractionAction, ETriggerEvent::Completed, this, &AMyPlayerController::Interaction_Released);
		Input->BindAction(DraggingRotateAction, ETriggerEvent::Triggered, this, &AMyPlayerController::DraggingRotate);
		Input->BindAction(ShootAction, ETriggerEvent::Started, this, &AMyPlayerController::Shoot);
		Input->BindAction(AttackAction, ETriggerEvent::Completed, this, &AMyPlayerController::Attack);
	}
}

void AMyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Player = Cast<AMyCharacter>(InPawn);

	//Player->NamePlateWidget->SetName(Player->GetPlayerState()->GetPlayerName());
	
}

void AMyPlayerController::Move(const FInputActionInstance& Instance)
{
	if (CurrentStrategy != nullptr && CurrentControlMode != ControlMode::TELESCOPE)
	{
		CurrentStrategy->Move(Instance, ControlledActor,this, GetWorld()->GetDeltaSeconds());
	}
}




void AMyPlayerController::Interaction_Pressed()
{
	if(Player->GetIsOverLap())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Interaction start"));
		bIsPressingKey = true;
		PressDuration = 0.0f; // 타이머 리셋
		CurrentHitObject = Player->GetCurrentHitObject(); // 맨 처음 상호작용 시도한 오브젝트 기록
	}
}

void AMyPlayerController::Interaction_Trigger()
{
	if (Player->GetIsOverLap())
	{
		if (PressDuration >= 3.0f) // 3초 넘게 누르면 DRAGGING 상태로 전환
		{
			//무언가를 끌고 있지 않을때만 끌기가 가능하게
			if(Player->CurrentPlayerState == Player->GetUserStateNone())// && !Player->GetCurrentHitObject()->GetIsDragging())
			{
				ServerRPC_DragObject(Player);
				//Player->DragObject(); // 여기서 오브젝트의 IsDragging이 true가 됨.
				Player->SetPlayerState(Player->GetUserStateDragging());
				GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Red, TEXT("Dragging 상태로 변경!"));
			}
		}
	}

	// if(!Player->GetCurrentHitObject()->GetIsDragging())
	// 	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("object carrying"));
		
}


void AMyPlayerController::Interaction_Released()
{
	bIsPressingKey = false;
	if (Player->GetIsOverLap() && CurrentHitObject)
	{
		if (PressDuration < 3.0f && !Player->GetCurrentHitObject()->GetIsDragging()) // 3초 안됐으면 그냥 상호작용
		{
			UE_LOG(LogTemp, Log, TEXT("interaction"));
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Interaction"));
			Player->SetTextWidgetVisible(!Player->GetTextWidgetVisible());
			ViewChange();
			CurrentHitObject->Interact();
		}
	
		else if(PressDuration >= 3.0f)
		{
			Player->SetPlayerState(Player->GetUserStateNone());
			
			//이동하는 오브젝트 놔주는 함수
			//다시 Ship의 자식으로
			//Player->DropObject(Ship);
			ServerRPC_DropObject(CurrentHitObject, Ship);
			
		}
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Interaction end"));
		CurrentHitObject = nullptr;
	}
}

void AMyPlayerController::DraggingRotate(const FInputActionInstance& Instance)
{
	if(Player->CurrentPlayerState==Player->GetUserStateDragging())
	{
		FRotator NewRotation = CurrentHitObject->TargetRotation + FRotator(0.0f, Instance.GetValue().Get<float>(),0.0f);
		ServerRPC_RotateDraggingObject(CurrentHitObject, NewRotation);
		
		//Player->GetCurrentHitObject()->AddActorLocalRotation(NewRotation);
	}
}



void AMyPlayerController::SetControlMode(ControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode)
	{
	case ControlMode::SHIP:
		Player->bUseControllerRotationYaw = false;
		TargetArmLength = 6000.0f;
		TargetRotation = Ship->GetActorRotation()+FRotator(-70.0f, 0.0f, 0.0f);
		Player->SetIsChanging(TargetArmLength,FVector(0.0f,0.0f,200.0f), TargetRotation, true);
		break;

	case ControlMode::CHARACTER:
		TargetArmLength = 1000.0f;
		TargetRotation = FRotator(-25.0f, 0.0f, 0.0f);
		Player->SetIsChanging(TargetArmLength, FVector(0.0f),TargetRotation, true);
		break;

	case ControlMode::CANNON:
		TargetArmLength = 1500.0f;
		TargetRotation = Cannon->GetActorRotation() + FRotator(-30.0f, -90.0f, 0.0f);
		//TargetRotation = FRotator(-30.0f, 0.0f, 0.0f);
		Player->SetIsChanging(TargetArmLength,FVector(0.0f), TargetRotation, true);
		break;

	case ControlMode::TELESCOPE:
		TargetArmLength = 10000.0f;
		TargetRotation = FRotator(-90.0f, 0.0f, 0.0f);
		Player->SetIsChanging(TargetArmLength,FVector(0.0f), TargetRotation, true);
		
	}
}

//모드 변환
void AMyPlayerController::ViewChange()
{
	switch (CurrentControlMode)
	{
	case ControlMode::CHARACTER:
		// 플레이어가 현재 선택/접근한 오브젝트의 이름을 비교
		if (Player->GetCurrentHitObjectName().Equals(TEXT("SteelWheel")))
		{
			// 현재 접근한 오브젝트가 "SteelWheel"이면, 컨트롤 모드를 SHIP으로 변경
			
			LastMappingContext = DefaultMappingContext;
			CurrentStrategy = new ShipControlStrategy;
			ControlledActor = Ship;
			SetControlMode(ControlMode::SHIP);
		}
		else if (Player->GetCurrentHitObjectName().Equals(TEXT("Cannon")))
		{

			// if 캐릭터 스테이트가 carrying이라면
			if(Player->CurrentPlayerState == Player->GetUserStateNone())
			{
				// else None 이라면 아니면 (아무것도 들고 있지 않은 상태면 Cannon 조작으로
				// 현재 접근한 오브젝트가 "Cannon"이면, 컨트롤 모드를 CANNON으로 변경
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("ChangeMapping"));
				Subsystem->RemoveMappingContext(DefaultMappingContext);
				Subsystem->AddMappingContext(CannonMappingContext, 0);
				LastMappingContext = CannonMappingContext;
				CurrentStrategy = new CannonControlStrategy();
				ControlledActor = Player->GetCurrentHitObject();
				Cannon = Cast<AMyCannon>(Player->GetCurrentHitObject());
				SetControlMode(ControlMode::CANNON);
			}
			else if(Player->CurrentPlayerState == Player->GetUserStateCarrying())
			{
				//나중에 옮기는 오브젝트가 뭔지도 검사
				//지금은 무조건 대포알로 간주

				//대포 장전
				Cannon = Cast<AMyCannon>(Player->GetCurrentHitObject());

				ServerRPC_LoadCannonBall(Cannon);
				Player->SetPlayerState(Player->GetUserStateNone());
				Player->SetTextWidgetVisible(!Player->GetTextWidgetVisible());
				ServerRPC_DestroyCarryCannonBall(Player);      
			}

			
		}
		else if (Player->GetCurrentHitObjectName().Equals(TEXT("CannonBallBox")))
		{
			if(Player->CurrentPlayerState == Player->GetUserStateNone())
			{
				CannonBallBox = Cast<AMyCannonBallBox>(Player->GetCurrentHitObject());
				// 현재 접근한 오브젝트가 "CannonBallBox"면 캐논볼 생성
				ServerRPC_SpawnCarryCannonBall(Player, CannonBallBox);
				//Player->SpawnCannonBall();
				Player->SetPlayerState(Player->GetUserStateCarrying());
			}
		}

		else if(Player->GetCurrentHitObjectName().Equals(TEXT("Telescope")))
		{
			if(Player->CurrentPlayerState == Player->GetUserStateNone())
			{
				SetControlMode(ControlMode::TELESCOPE);
				
			}
		}
		
		break;

	case ControlMode::SHIP:
	case ControlMode::CANNON:
	case ControlMode::TELESCOPE:
		// 현재 컨트롤 모드가 SHIP 또는 CANNON일 경우, 무조건 CHARACTER 모드로 전환
		SetControlMode(ControlMode::CHARACTER);
		Subsystem->RemoveMappingContext(LastMappingContext);
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		LastMappingContext = DefaultMappingContext;
		CurrentStrategy = new CharacterControlStrategy();
		ControlledActor = Player;
		break;
	}
}




void AMyPlayerController::Shoot(const FInputActionInstance& Instance)
{
	//대포알이 장전됐을때만 발사 가능
	if (IsLocalController() && Cannon->GetIsLoad())
	{
		ServerRPC_Shoot(Cannon);
		ServerRPC_UseCannonBall(Cannon);
		//Cannon->SetIsLoad(false);
	}
}

void AMyPlayerController::Attack(const FInputActionInstance& Instance)
{
	Player->Attack();
}

void AMyPlayerController::ServerRPC_Shoot_Implementation(AMyCannon* CannonActor)
{
	if (HasAuthority())
	{
		CannonActor->FireCannon();
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Shooting!"));
	}
}

void AMyPlayerController::ServerRPC_RotateCannon_Implementation(AMyCannon* CannonActor, FRotator newRot)
{
	if(HasAuthority())
	{
		CannonActor->MultiCastRPC_RotateCannon(newRot);
	}
}

void AMyPlayerController::ServerRPC_MoveShip_Loc_Implementation(FVector newLoc)
{
	if(HasAuthority())
	{
		Ship->MulticastRPC_SetShipLocation(newLoc);
	}
}

void AMyPlayerController::ServerRPC_MoveShip_Rot_Implementation(float newYaw, float speed)
{
	if(HasAuthority())
	{
		FRotator newRot = FRotator(0.0f, newYaw*speed*GetWorld()->GetDeltaSeconds(), 0.0f) + Ship->GetActorRotation();
		Ship->TargetRotation = newRot;
		//Ship->MulticastRPC_SetShipRotation(newYaw, speed);
	}
}

void AMyPlayerController::ServerRPC_LoadCannonBall_Implementation(AMyCannon* CannonActor)
{
	if(HasAuthority())
	{
		CannonActor->SetIsLoad(true);
	}
}

void AMyPlayerController::ServerRPC_UseCannonBall_Implementation(AMyCannon* CannonActor)
{
	if(HasAuthority())
	{
		CannonActor->SetIsLoad(false);
	}
}

void AMyPlayerController::ServerRPC_DragObject_Implementation(AMyCharacter* user)
{
	if(HasAuthority())
	{
		user->GetCurrentHitObject()->SetIsDragging(true);
		user->GetCurrentHitObject()->AttachToActor(user, FAttachmentTransformRules::KeepWorldTransform);

		user->GetCurrentHitObject()->MulticastRPC_TurnOffCollision();
	}
}

void AMyPlayerController::ServerRPC_DropObject_Implementation(AMyObject* obj, AActor* ship)
{
	if(HasAuthority())
	{
		
		obj->SetIsDragging(false);
		obj->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		obj->AttachToActor(ship, FAttachmentTransformRules::KeepWorldTransform);

		obj->MulticastRPC_TurnOnCollision();

	}
}

void AMyPlayerController::ServerRPC_RotateDraggingObject_Implementation(AMyObject* obj, FRotator newRotation)
{
	if(HasAuthority())
	{
		//user->GetCurrentHitObject()->TargetRotation = newRotation;
		obj->TargetRotation = newRotation;
		
	}
}

void AMyPlayerController::ServerRPC_SpawnCarryCannonBall_Implementation(AMyCharacter* user, AMyCannonBallBox* box)
{
	if(HasAuthority())
	{
		box->SpawnCarryCannonBall(user);
	}
}

void AMyPlayerController::ServerRPC_DestroyCarryCannonBall_Implementation(AMyCharacter* user)
{
	if(HasAuthority())
	{
		user->DestroyCannonBall();
	}
}








