// Fill out your copyright notice in the Description page of Project Settings.

#include "World_InteractObject/HopperMine.h"
#include "Components/AudioComponent.h"
#include "Components/PointLightComponent.h"
#include "Player/MyCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WorldActors/Door.h"

DEFINE_LOG_CATEGORY(LogLoadResourceMine);
DEFINE_LOG_CATEGORY(LogHopper);

AHopperMine::AHopperMine()
{
	PrimaryActorTick.bCanEverTick = true;

	// Loading and Settings mine and Mine Component 
	HopperMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HopperMesh")); 
	DetectedCollision = CreateDefaultSubobject<USphereComponent>(TEXT("DetectedSphere"));
	ActivateCollision = CreateDefaultSubobject<USphereComponent>(TEXT("DetectedBox"));
	LightDetector = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightDetector"));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> HopperMesh(TEXT("/Game/World_InteractObject/HopperMine/Hoppers"));
	if (HopperMesh.Succeeded())
	{
		HopperMeshComponent->SetStaticMesh(HopperMesh.Object);
	}
	else
	{
		UE_LOG(LogLoadResourceMine, Warning, TEXT("Eror find object mesh"));
	}

	
	SetRootComponent(HopperMeshComponent);
	HopperMeshComponent->SetWorldScale3D(FVector(32.0f,32.0f,35.0f));

	// Settings component mine
	// Light component
	LightDetector->SetupAttachment(HopperMeshComponent);
	LightDetector->SetWorldLocation(FVector(0.0f, 0.0f, 0.32));
	LightDetector->AttenuationRadius = 30.0f;
	LightDetector->SetVisibility(false);
	LightDetector->IntensityUnits = ELightUnits::Lumens;	
	LightDetector->bAffectsWorld = true;	

	ActivateCollision->SetupAttachment(HopperMeshComponent);
	ActivateCollision->InitSphereRadius(5.0f);

	DetectedCollision->SetupAttachment(HopperMeshComponent);
	

}


void AHopperMine::BeginPlay()
{
	Super::BeginPlay();


	TArray<FLoadSoundResource> ResourcesToLoad = {
	{"/Game/Sound/Sound_InteractObj/Cue/Mine_active_loop_Cue", nullptr},
	{"/Game/Sound/Sound_InteractObj/Cue/Mine_deactivate_Cue", nullptr},
	{"/Game/Sound/Sound_InteractObj/Cue/Mine_deploy_Cue", nullptr},
	
	};
	
	
	for (FLoadSoundResource& Resource : ResourcesToLoad)
	{
		Resource.LoadedResource = LoadObject<UObject>(nullptr,*Resource.ResourcePath);
		if (!Resource.LoadedResource)
		{
			UE_LOG(LOG_LOADING_RESOURCE, Warning, TEXT("Eror find sound object"));
		}
	}
	for (const FLoadSoundResource& Resource : ResourcesToLoad)
	{
		USoundBase* ArrayLoadedSound = Cast<USoundBase>(Resource.LoadedResource);

		if (ArrayLoadedSound)
		{
			MineSound.Add(ArrayLoadedSound);
		}		
		
	}

	// Physics Mine
	HopperMeshComponent->SetSimulatePhysics(true);
	HopperMeshComponent->SetMassOverrideInKg(NAME_None, 90, true);

	//Collision Detected
	DetectedCollision->OnComponentBeginOverlap.AddDynamic(this, &AHopperMine::OnDetectionRadiusBeginOverlap);
	DetectedCollision->OnComponentEndOverlap.AddDynamic(this, &AHopperMine::OnDetectionRadiusEndOverlap);
	// Collision Activate
	ActivateCollision->OnComponentBeginOverlap.AddDynamic(this, &AHopperMine::BeginActivateMine);	
}



void AHopperMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}



// Detection Actors and Player
void AHopperMine::OnDetectionRadiusBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MyCharacter = Cast<AMyCharacter>(OtherActor);

	if (MineSound.IsValidIndex(0))
	{
		if (LightDetector != nullptr)
		{
			UE_LOG(LogHopper, Warning, TEXT("Detected "));
			AudioComponent = UGameplayStatics::SpawnSoundAttached(MineSound[0] , HopperMeshComponent);
			LightDetector->SetVisibility(true);
			LightDetector->SetLightColor(FLinearColor(1.0f, 0.0f,0.0f));
		}
		
	}
	
}


void AHopperMine::OnDetectionRadiusEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (MyCharacter && OtherActor == MyCharacter)
	{
		if (MineSound.IsValidIndex(0))
		{
			UE_LOG(LogHopper, Warning, TEXT("OnDetected"));
			AudioComponent->Stop();
			AudioComponent = UGameplayStatics::SpawnSoundAttached(MineSound[1] , HopperMeshComponent);
			LightDetector->SetVisibility(false);
		}
	
	}
}


// Activate Mine
void AHopperMine::BeginActivateMine(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MyCharacter = Cast<AMyCharacter>(OtherActor);

	if (MyCharacter)
	{
		if (MineSound.IsValidIndex(0))
		{
			UE_LOG(LogHopper, Warning, TEXT("MINE ACTIVATE"));
			AudioComponent = UGameplayStatics::SpawnSoundAttached(MineSound[2], HopperMeshComponent);
		}

	}

}


void AHopperMine::ActivateMine()
{


}



