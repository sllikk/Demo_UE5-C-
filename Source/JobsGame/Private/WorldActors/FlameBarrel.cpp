// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldActors/FlameBarrel.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Player/MyCharacter.h"
#include "Shared/Resourse.h"

// Sets default values
AFlameBarrel::AFlameBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 20;

	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	Trigger->SetupAttachment(BarrelMesh);
	Trigger->InitSphereRadius(255.0f);
	Trigger->SetCollisionProfileName("Interaction");
	
	const FSoftObjectPath FindMesh(TEXT("/Game/World_InteractObject/FlameBarrel/barrel"));	
	static TObjectPtr<UStaticMesh> StaticMesh = nullptr;
	if (FindMesh.IsValid())
    {
	    StaticMesh = Cast<UStaticMesh>(FindMesh.TryLoad());
    }
    if (StaticMesh != nullptr)
    {
	    BarrelMesh->SetStaticMesh(StaticMesh);
		RootComponent = BarrelMesh;
    	BarrelMesh->SetWorldScale3D(FVector(0.42f, 0.42f, 0.4f));
		BarrelMesh->SetWorldLocation(FVector(0.0f, 0.0f, 70));
    }
    else
    {
	    UE_LOG(LogLoad, Warning, TEXT("Error Load: %s"), *FindMesh.ToString())
    }

}

// Called when the game starts or when spawned
void AFlameBarrel::BeginPlay()
{
	Super::BeginPlay();

	BarrelMesh->SetSimulatePhysics(true);
	BarrelMesh->SetMassOverrideInKg(NAME_None, 35);
	BarrelMesh->SetAngularDamping(1.0f);
	BarrelMesh->SetLinearDamping(0.5);

//	BarrelMesh->OnComponentHit.AddDynamic(this, &AFlameBarrel::OnHit);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AFlameBarrel::Detected);
	OnTakeAnyDamage.AddDynamic(this, &AFlameBarrel::ChainsDamage);
	
	 TArray<FResourceLoad> ResourceLoads = {
		FResourceLoad(TEXT("/Game/M5VFXVOL2/Particles/Fire/Fire_02"), nullptr),
		FResourceLoad(TEXT("/Game/VFX/Particles/Explosion/Hopper_Explosion"), nullptr),		
	};
	
	for (FResourceLoad& Resource : ResourceLoads)
	{
		Resource.LoadedResource = LoadObject<UObject>(nullptr,*Resource.ResourcePath);
		if (!Resource.LoadedResource)
		{
			UE_LOG(LogLoad, Warning, TEXT("Error Load: %s"), *Resource.ResourcePath)
		}
	}
	for (const FResourceLoad& Resource : ResourceLoads)
	{
		  UParticleSystem* LoadParticleSystem = Cast<UParticleSystem>(Resource.LoadedResource);
		 if (LoadParticleSystem != nullptr)
		 {
		 	ParticleSystem.Add(LoadParticleSystem);
			
		 }
	}


}

// Called every frame
void AFlameBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Debug();


}

void AFlameBarrel::Detected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const AMyCharacter* Character = Cast<const AMyCharacter>(OtherActor);

	if (Character != nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("Detected: "))
		BarrelBurns();	
	}
	
}
void AFlameBarrel::ReduceHealth()
{
	--Health; 

	
	if (Health <= 0)
	{
		Explode();
	}
}

void AFlameBarrel::Explode()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerExplode);
	// Удалить все отладочные строки, связанные с этим объектом
	FlushPersistentDebugLines(GetWorld());
	FlushDebugStrings(GetWorld());

	TArray<FHitResult> OutHits;

	const FVector& MyLocation = GetActorLocation();
	const FVector& Start = MyLocation;
	const FVector& End = MyLocation;
	const FCollisionShape& MyColSphere = FCollisionShape::MakeSphere(500.0f);
	DrawDebugSphere(GetWorld(), GetActorLocation(), MyColSphere.GetSphereRadius(), 25, FColor::Green, true);

	if (GetWorld()->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, ECC_Destructible, MyColSphere))
	{
		for(auto& OutHit : OutHits)
		{
			if (ParticleSystem[1]->IsValidLowLevel())
			{
				const TArray<AActor*> IgnoreActors;
				UGameplayStatics::SpawnEmitterAtLocation(this, ParticleSystem[1], GetActorLocation());
				UGameplayStatics::ApplyRadialDamage(this, 2, GetActorLocation(), 500,
								UDamageType::StaticClass(), IgnoreActors, this, GetInstigatorController());
				UPrimitiveComponent* HitComp = OutHit.GetComponent();

				if (HitComp && HitComp->IsSimulatingPhysics())
				{
					FVector ImpulseDirection = HitComp->GetComponentLocation() - GetActorLocation();
					ImpulseDirection.Normalize();
					HitComp->AddImpulse(ImpulseDirection * 40000.0f);  
					Destroy();
				}
			}
		}
	}
}


void AFlameBarrel::Debug()
{
	const FString& TEXT = FString::Printf(TEXT("Health: %2.0f"), Health); 
	const FVector& Location = GetActorLocation();
	const FColor& Color = FColor::White;

	DrawDebugString(GetWorld(), Location, TEXT, nullptr, Color, 0);

}


void AFlameBarrel::ChainsDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                AController* InstigatedBy, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.0f, 20.0f);

	if (Health <= 12)
	{
		BarrelBurns();
	}

}


void AFlameBarrel::BarrelBurns() 
{	
	GetWorld()->GetTimerManager().SetTimer(TimerExplode, this, &AFlameBarrel::ReduceHealth, 0.5f, true);
	
	if (ParticleSystem[0]->IsValidLowLevel())
	{
		UGameplayStatics::SpawnEmitterAttached(ParticleSystem[0], BarrelMesh);

	}
}


void AFlameBarrel::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(TimerExplode);

}



/* 
void AFlameBarrel::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
	}
	
}
*/