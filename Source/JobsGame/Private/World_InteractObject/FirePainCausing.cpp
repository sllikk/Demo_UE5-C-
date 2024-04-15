// Fill out your copyright notice in the Description page of Project Settings.

#include "World_InteractObject/FirePainCausing.h"
#include "Components/BoxComponent.h"
#include "CustomType/CustomDamage.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

DEFINE_LOG_CATEGORY(FirePainCausing)

AFirePainCausing::AFirePainCausing()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetWorldScale3D(FVector(4.5f,4.5f,3.5f));
	RootComponent = BoxComponent;

	// Load particles
	FSoftObjectPath ParticleFind(TEXT("/Game/VFX/Particles/Fire/P_Fire_Big"));
	UParticleSystem* ParticleSystem = nullptr;
	if (ParticleFind.IsValid())
	{
		ParticleSystem = Cast<UParticleSystem>(ParticleFind.TryLoad());
	}
	else
	{
		UE_LOG(FirePainCausing, Warning, TEXT("Error find: %s"), *ParticleFind.ToString())
	}
	if (ParticleSystem != nullptr)
	{
		FireParticles = ParticleSystem;
	}
	
}


void AFirePainCausing::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFirePainCausing::OnDetectionBoxOverlap);

	if (FireParticles != nullptr)
	{
		FireParticles->FloorScale3D = FVector(0.6, 0.6, 0.6);
		UGameplayStatics::SpawnEmitterAtLocation(this, FireParticles, GetActorLocation());
	}
	
}


void AFirePainCausing::OnDetectionBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FString Name = OtherActor->GetName(); 
	UE_LOG(FirePainCausing, Warning, TEXT("Detected %s"), *Name);
	
	CustomDamage = NewObject<UCustomDamage>();
	CustomDamage->DamageType = EDamageType::DMG_FIRE;
	const FDamageTypeData DamageTypeData = CustomDamage->GetDamageTypeData();

	if (CustomDamage != nullptr)
	{
		static float Damage = 1;
		Damage += DamageTypeData.DamageMultiplayer;
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, CustomDamage->StaticClass());
	}
	
}

void AFirePainCausing::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}






