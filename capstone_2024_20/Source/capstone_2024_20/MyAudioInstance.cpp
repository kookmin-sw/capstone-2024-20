#include "MyAudioInstance.h"
#include "Kismet/GameplayStatics.h"

UMyAudioInstance::UMyAudioInstance()
{
	ConstructorHelpers::FObjectFinder<USoundCue> SoundCueAsset(TEXT("/Script/Engine.SoundCue'/Game/Sounds/BGM/TitleSQ.TitleSQ'"));
	ConstructorHelpers::FObjectFinder<USoundCue> SoundCueAsset2(TEXT("/Script/Engine.SoundCue'/Game/Sounds/BGM/LobbySQ.LobbySQ'"));
	ConstructorHelpers::FObjectFinder<USoundCue> SoundCueAsset3(TEXT("/Script/Engine.SoundCue'/Game/Sounds/BGM/IngameSQ.IngameSQ'"));
	if (SoundCueAsset.Succeeded())
	{
		MusicTracks.Add(SoundCueAsset.Object);
		MusicTracks.Add(SoundCueAsset2.Object);
		MusicTracks.Add(SoundCueAsset3.Object);
	}
}


void UMyAudioInstance::Init()
{
	Super::Init();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("asdffdsa"));
}
void UMyAudioInstance::OnStart()
{
	Super::OnStart();
	if (MusicTracks.IsValidIndex(0) && MusicTracks[0])
    	{
    		if (MusicComponent && MusicComponent->IsPlaying())
    		{
    			MusicComponent->Stop();
    		}
    
    		MusicComponent = UGameplayStatics::SpawnSound2D(this, MusicTracks[0], 1.0f, 1.0f, 0.0f, nullptr, true);
    	}
	
}
void UMyAudioInstance::PlayMusic(int32 Index)
{
	if (MusicTracks.IsValidIndex(Index) && MusicTracks[Index])
	{
		if (MusicComponent && MusicComponent->IsPlaying())
		{
			MusicComponent->Stop();
		}

		MusicComponent = UGameplayStatics::SpawnSound2D(this, MusicTracks[Index], 1.0f, 1.0f, 0.0f, nullptr, true);
	}
}