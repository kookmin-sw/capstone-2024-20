#include "CharacterChangerComponent.h"

#include "CapCharacterSkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UCharacterChangerComponent::UCharacterChangerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CharacterType = ECharacterType::Character1;
}


void UCharacterChangerComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* Character = GetOwner<ACharacter>();
	if (Character)
	{
		OwnerCharacter = Character;
		ChangeCharacter(CharacterType);
	}
}


void UCharacterChangerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCharacterChangerComponent::OnRep_CharacterType()
{	
	ChangeCharacter(CharacterType);
}

void UCharacterChangerComponent::ChangeCharacter(ECharacterType Type)
{
	const UCapCharacterSkeletalMeshComponent* SkeletalMeshComponent = UCharacterTypeFactory::GetCharacter(Type);

	if (SkeletalMeshComponent && OwnerCharacter)
	{
		OwnerCharacter->GetMesh()->SetAnimInstanceClass(SkeletalMeshComponent->AnimClass);
		OwnerCharacter->GetMesh()->SetSkeletalMeshAsset(SkeletalMeshComponent->GetSkeletalMeshAsset());
	}
}

void UCharacterChangerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCharacterChangerComponent, CharacterType);
}

void UCharacterChangerComponent::Multicast_Change_Implementation(ECharacterType Type)
{
	ChangeCharacter(Type);
}

void UCharacterChangerComponent::Change_Implementation(ECharacterType Type)
{
	if(GetOwner()->HasAuthority())
	{
		CharacterType = Type;
		OnRep_CharacterType();
	}
}
