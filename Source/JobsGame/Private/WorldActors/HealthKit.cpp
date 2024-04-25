// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldActors/HealthKit.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "PlayerComponent/Health.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"


DEFINE_LOG_CATEGORY(LogHealthKit);

AHealthKit::AHealthKit()
{

	PrimaryActorTick.bCanEverTick = false;

	m_Amounth = 25.0f;

	// Load Mesh in Health Kit
	FSoftObjectPath MeshAssets(TEXT("/Game/WorldActors/RestoreKits/HealthKit/health_kit"));	
	UStaticMesh* StaticMesh = nullptr;

	if (MeshAssets.IsValid())
	{
		StaticMesh = Cast<UStaticMesh>(MeshAssets.TryLoad());
	}
	if (StaticMesh != nullptr)
	{
		MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthKit"));
		MeshComponent->SetStaticMesh(StaticMesh);
		MeshComponent->SetWorldScale3D(FVector(0.015f, 0.015f, 0.015f));
		SetRootComponent(MeshComponent);
		
		CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
		CollisionSphere->InitSphereRadius(3000.0f);
		CollisionSphere->SetCollisionProfileName(TEXT("OverllupAll"));
		CollisionSphere->SetupAttachment(MeshComponent);
	}
	else
	{
		UE_LOG(LogHealthKit, Warning, TEXT("Eror find object: %s"), *MeshAssets.ToString());
	}

	// Load PickUp Sound
	FSoftObjectPath SoundFinder(TEXT("/Game/Sound/ActorSound/Cue/Pickup_Health_Cue"));
	USoundBase* SoundBase = nullptr;

	if (SoundFinder.IsValid())
	{
		SoundBase = Cast<USoundBase>(SoundFinder.TryLoad());
	}
	if (SoundBase != nullptr)
	{
		SoundPickup = SoundBase;
	}
	else
	{
		UE_LOG(LogHealthKit, Warning, TEXT("Eror find sound!!!!"));

	}

}


void AHealthKit::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComponent->SetSimulatePhysics(true);
	 
}

void AHealthKit::NotifyActorBeginOverlap(AActor* OtherActor)
{
	HealthComponent = Cast<UHealthComponent>(OtherActor->GetComponentByClass(UHealthComponent::StaticClass()));

	if (HealthComponent != nullptr)
	{
		if (HealthComponent->GetHealth() < HealthComponent->GetMaxHealth())
		{
		HealthComponent->RestoreHealth(m_Amounth);
			
			if (SoundPickup != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, SoundPickup, GetActorLocation());
				Destroy();
			}
		}
	}
}








