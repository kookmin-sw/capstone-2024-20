#include "CharacterChangerComponent.h"

#include "CapCharacterSkeletalMeshComponent.h"
#include "GameFramework/Character.h"


UCharacterChangerComponent::UCharacterChangerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCharacterChangerComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* Character = GetOwner<ACharacter>();
	if (Character)
	{
		OwnerCharacter = Character;
	}
}


void UCharacterChangerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCharacterChangerComponent::Change(ECharacterType Type)
{
	UCapCharacterSkeletalMeshComponent* SkeletalMeshComponent = UCharacterTypeFactory::GetCharacter(Type);

	if(SkeletalMeshComponent)
	{
		OwnerCharacter->GetMesh()->SetSkeletalMeshAsset(SkeletalMeshComponent->GetSkeletalMeshAsset());
	}
}
