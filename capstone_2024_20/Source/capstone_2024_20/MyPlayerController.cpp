#include "MyPlayerController.h"
#include "CannonControlStrategy.h"
#include "CharacterControlStrategy.h"
#include "ShipControlStrategy.h"
#include "Kismet/GameplayStatics.h"
#include "InputMappingContext.h"
#include "MyIngameHUD.h"
#include "Sailing/SailingSystem.h"
#include "Upgrade/UpgradeWidgetElement.h"

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
	
	CurrentStrategy = new CharacterControlStrategy();
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Emerald, TEXT("HasAutority"));
	}

	TArray<AActor*> FoundShips;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyShip::StaticClass(), FoundShips);
	if (FoundShips.Num() > 0)
	{
		Ship = Cast<AMyShip>(FoundShips[0]);
	}

	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem != nullptr)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	if (HasAuthority())
	{
		Player = Cast<AMyCharacter>(GetPawn());
		if (Player)
		{
			ControlledActor = Player;
			if (Player->InputComponent)
			{
				SetupPlayerInputComponent(Player->InputComponent);
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Player NULL"));
		}
	}

	CurrentControlMode = ControlMode::CHARACTER;

	if(Ship)
	{
		SetViewTarget(Ship->Camera_Character);
	}

	SailingSystem = Cast<ASailingSystem>(UGameplayStatics::GetActorOfClass(GetWorld(), ASailingSystem::StaticClass()));
	
	if (IsLocalController())
	{
		MyInGameHUD = Cast<AMyIngameHUD>(GetHUD());
		MyInGameHUD->BeginPlay();
	
		const UUpgradeWidget* PopupUpgrade = MyInGameHUD->GetPopupUpgrade();
		PopupUpgrade->SpeedUpgrade->OnClickUpgradeDelegate.AddUObject(this, &AMyPlayerController::UpgradeMyShipMoveSpeed);
		PopupUpgrade->HandlingUpgrade->OnClickUpgradeDelegate.AddUObject(this, &AMyPlayerController::UpgradeMyShipHandling);
		PopupUpgrade->CannonAttackUpgrade->OnClickUpgradeDelegate.AddUObject(this, &AMyPlayerController::UpgradeMyShipCannonAttack);
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
					SetViewTarget(Ship->Camera_Character);
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
}

void AMyPlayerController::Move(const FInputActionInstance& Instance)
{
	if (Player->GetCurrentPlayerState() == UserState::DEAD || Player->IsAttacking())
	{
		return;
	}
	
	if (CurrentStrategy != nullptr && CurrentControlMode != ControlMode::TELESCOPE && CurrentControlMode != ControlMode::BED)
	{
		CurrentStrategy->Move(Instance, ControlledActor,this, GetWorld()->GetDeltaSeconds());
	}
}

void AMyPlayerController::Interaction_Pressed()
{
	if (Player->GetCurrentPlayerState() == UserState::DEAD || Player->IsAttacking())
	{
		return;
	}
	
	if(Player->GetIsOverLap())
	{
		bIsPressingKey = true;
		PressDuration = 0.0f; // 타이머 리셋
		CurrentHitObject = Player->GetCurrentHitObject(); // 맨 처음 상호작용 시도한 오브젝트 기록
	}
}

void AMyPlayerController::Interaction_Trigger()
{
	if (Player->GetCurrentPlayerState() == UserState::DEAD || Player->IsAttacking())
	{
		return;
	}
	
	if (Player->GetIsOverLap())
	{
		if (PressDuration >= 3.0f) // 3초 넘게 누르면 DRAGGING 상태로 전환
		{
			//무언가를 끌고 있지 않을때만 끌기가 가능하게
			if(Player->CurrentPlayerState == Player->GetUserStateNone())
			{
				ServerRPC_DragObject(Player);
				Player->SetPlayerState(Player->GetUserStateDragging());
			}
		}
	}
}

void AMyPlayerController::Interaction_Released()
{
	if (Player->GetCurrentPlayerState() == UserState::DEAD || Player->IsAttacking())
	{
		return;
	}
	
	bIsPressingKey = false;
	if (Player->GetIsOverLap() && CurrentHitObject)
	{
		if (PressDuration < 3.0f && !Player->GetCurrentHitObject()->GetIsDragging() && !bIsCameraTransitioning) // 3초 안됐으면 그냥 상호작용
		{
			Player->SetTextWidgetVisible(!Player->GetTextWidgetVisible());
			ViewChange();
			InteractOnClient(CurrentHitObject);
			InteractOnServer(CurrentHitObject);
		}
		else if(PressDuration >= 3.0f)
		{
			Player->SetPlayerState(Player->GetUserStateNone());
			ServerRPC_DropObject(CurrentHitObject, Ship);
		}
		
		CurrentHitObject = nullptr;
	}
}

void AMyPlayerController::DraggingRotate(const FInputActionInstance& Instance)
{
	if (Player->GetCurrentPlayerState() == UserState::DEAD || Player->IsAttacking())
	{
		return;
	}
	
	if(Player->CurrentPlayerState==Player->GetUserStateDragging())
	{
		const FRotator NewRotation = CurrentHitObject->TargetRotation + FRotator(0.0f, Instance.GetValue().Get<float>(),0.0f);
		ServerRPC_RotateDraggingObject(CurrentHitObject, NewRotation);
	}
}

void AMyPlayerController::SetControlMode(ControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	if(!bIsCameraTransitioning)
	{
		constexpr float BlendTime = 0.5f;
		bIsCameraTransitioning = true;
		switch (CurrentControlMode)
		{
			case ControlMode::SHIP:
				SetViewTargetWithBlend(Ship->Camera_Ship, BlendTime);
				break;

			case ControlMode::CHARACTER:
				SetViewTargetWithBlend(Ship->Camera_Character, BlendTime);
				break;

			case ControlMode::CANNON:
				SetViewTargetWithBlend(Cannon->Camera_Cannon,BlendTime);
				break;

			case ControlMode::TELESCOPE:
				SetViewTargetWithBlend(Ship->Camera_Telescope,BlendTime);
				break;
			
			case ControlMode::BED:
				SetViewTargetWithBlend(Bed->Camera_Bed, BlendTime);
		}
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			bIsCameraTransitioning = false;
		}, BlendTime, false);
	}
}

void AMyPlayerController::ViewChange()
{
	switch (CurrentControlMode)
	{
	case ControlMode::CHARACTER:
		// 플레이어가 현재 선택/접근한 오브젝트의 이름을 비교
		if (Player->GetCurrentHitObjectName().Equals(TEXT("SteelWheel")))
		{
			LastMappingContext = DefaultMappingContext;
			CurrentStrategy = new ShipControlStrategy;
			ControlledActor = Ship;
			SetControlMode(ControlMode::SHIP);
		}
		else if (Player->GetCurrentHitObjectName().Equals(TEXT("Cannon")))
		{
			if(Player->CurrentPlayerState == Player->GetUserStateNone())
			{
				Subsystem->RemoveMappingContext(DefaultMappingContext);
				Subsystem->AddMappingContext(CannonMappingContext, 0);
				LastMappingContext = CannonMappingContext;
				CurrentStrategy = new CannonControlStrategy();
				ControlledActor = Player->GetCurrentHitObject();
				Cannon = Cast<AMyCannon>(Player->GetCurrentHitObject());
				Cannon->VisibleWidget(true);
				SetControlMode(ControlMode::CANNON);
			}
			else if(Player->CurrentPlayerState == Player->GetUserStateCarrying())
			{
				//지금은 무조건 대포알로 간주
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
				ServerRPC_SpawnCarryCannonBall(Player, CannonBallBox);
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
		else if(Player->GetCurrentHitObjectName().Equals(TEXT("Bed")))
		{
			if(Player->CurrentPlayerState == Player->GetUserStateNone())
			{
				Bed = Cast<AMyBed>(Player->GetCurrentHitObject());
				ServerRPC_PlayerSleep(Player,true, Bed);
				SetControlMode(ControlMode::BED);
				FTimerHandle SleepTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(SleepTimerHandle, this, &AMyPlayerController::PlayerSleep, GetWorld()->GetDeltaSeconds(), false);
			}
		}
		break;

	case ControlMode::BED:
		ServerRPC_PlayerAwake(Player,false, Bed);
		PlayerAwake();
		
	case ControlMode::SHIP:
	case ControlMode::CANNON:
		if(Cannon)
			Cannon->VisibleWidget(false);
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
	if (Player->GetCurrentPlayerState() == UserState::DEAD || Player->IsAttacking())
	{
		return;
	}
	
	if (IsLocalController() && Cannon->GetIsLoad())
	{
		ServerRPC_Shoot(Cannon);
		ServerRPC_UseCannonBall(Cannon);
	}
}

// BindAction에 등록하기 위함
// ReSharper disable once CppMemberFunctionMayBeConst
void AMyPlayerController::Attack(const FInputActionInstance& Instance)
{
	if (Player->GetCurrentPlayerState() == UserState::DEAD || Player->IsAttacking())
	{
		return;
	}

	if (!Player->CanAttack())
	{
		return;
	}
	
	ServerRPC_Attack();
}

void AMyPlayerController::UpgradeMyShipMoveSpeed()
{
	ServerRPC_UpgradeMyShipMoveSpeed();
}

void AMyPlayerController::UpgradeMyShipHandling()
{
	ServerRPC_UpgradeMyShipHandling();
}

void AMyPlayerController::UpgradeMyShipCannonAttack()
{
	ServerRPC_UpgradeMyShipCannonAttack();
}

void AMyPlayerController::InteractOnClient(AMyObject* OBJ)
{
	OBJ->Interact();
}

void AMyPlayerController::InteractOnServer(AMyObject* OBJ)
{
	ServerRPC_InteractOnServer(OBJ);
}

void AMyPlayerController::ServerRPC_Attack_Implementation()
{
	if (HasAuthority())
	{
		Player->Attack();
	}
}

void AMyPlayerController::ServerRPC_Shoot_Implementation(AMyCannon* CannonActor)
{
	if (HasAuthority())
	{
		CannonActor->FireCannon();
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
		const FRotator NewRot = FRotator(0.0f, newYaw*speed*GetWorld()->GetDeltaSeconds(), 0.0f) + Ship->GetActorRotation();
		Ship->TargetRotation = NewRot;
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

void AMyPlayerController::ServerRPC_PlayerSleep_Implementation(AMyCharacter* user, bool b, AMyBed* bed)
{
	if(HasAuthority())
	{
		user->SetPlayerState(user->GetUserStateSleeping());
		user->SetActorLocation(bed->GetSleepLocation());
		user->SetActorRotation(bed->GetSleepRotation());
		user->SetIsSleeping(b);
		user->AttachToActor(Ship, FAttachmentTransformRules::KeepWorldTransform);
		user->bUseControllerRotationYaw = true;
	}	
}

void AMyPlayerController::ServerRPC_InteractOnServer_Implementation(AMyObject* OBJ)
{
	OBJ->InteractOnServer();
}

void AMyPlayerController::PlayerSleep()
{
	if(Bed)
	{
		Player->SetActorRotation(Bed->GetSleepRotation());

		Bed->SleepSound();
		
		GetWorld()->GetTimerManager().SetTimer(HealthTimerHandle, [this]()
		{
			Player->Heal(1);
		}, 1.0f, true);
	}
}

void AMyPlayerController::PlayerAwake()
{
	Bed->AwakeSound();
	
	GetWorld()->GetTimerManager().ClearTimer(HealthTimerHandle);
}

void AMyPlayerController::ServerRPC_UpgradeMyShipMoveSpeed_Implementation()
{
	SailingSystem->UpgradeMyShipMoveSpeed();
}

void AMyPlayerController::ServerRPC_UpgradeMyShipHandling_Implementation()
{
	SailingSystem->UpgradeMyShipHandling();
}

void AMyPlayerController::ServerRPC_UpgradeMyShipCannonAttack_Implementation()
{
	SailingSystem->UpgradeMyShipCannonAttack();
}

void AMyPlayerController::ServerRPC_PlayerAwake_Implementation(AMyCharacter* user, bool b, AMyBed* bed)
{
	if(HasAuthority())
	{
		user->SetPlayerState(user->GetUserStateNone());
		user->SetActorLocationAndRotation(bed->GetAwakeLocation(),bed->GetAwakeRotation());
		user->SetIsSleeping(b);
		user->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		user->bUseControllerRotationYaw = false;
	}
}
