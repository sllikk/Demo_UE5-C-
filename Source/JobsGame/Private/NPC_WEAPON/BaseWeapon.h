﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/MyCharacter.h"
#include "Shared/interact.h"
#include "Shared/PropsDamage.h"
#include "BaseWeapon.generated.h"
class USkeletalMeshComponent;
class USoundBase;
class UInputMappingContext;
class UInputAction;


DECLARE_LOG_CATEGORY_EXTERN(LogWeapon, All, Log);

UENUM(BlueprintType)
enum class EFireMode : uint8
{
	SINGLE		UMETA(DisplayName = "Single"),
	AUTOMATIC	UMETA(DisplayName = "Automatic"),
	ALT         UMETA(DisplayName = "AltFire"),
};

UCLASS()
class JOBSGAME_API ABaseWeapon : public AActor, public Iinteract, public IPropsDamage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="SkeletalMesh", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMeshComponent;

	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> WeaponMappingContext;

	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PrimaryAttackAction;

	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SecondaryAttackAction;
	
	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ReloadAction;
	
public:
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called End Play
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Methods for getting values
	UFUNCTION(BlueprintCallable, Category = "mesh")
	FORCEINLINE USkeletalMeshComponent* GetWeaponMeshComponent() const  { return WeaponSkeletalMeshComponent; }
	FORCEINLINE FName	GetSocketName() const							{ return fSocketName; }
	FORCEINLINE int32	GetMaxAmmo() const								{ return imaxAmmo; }
	FORCEINLINE int32	GetCurrentAmmo() const							{ return icurrentAmmo; }
	FORCEINLINE int32	GetInvAmmo() const								{ return imaxInventoryAmmo; }
	FORCEINLINE float	GetReloadTime() const							{ return m_flReloadTime; }
	FORCEINLINE float	GetBulletSpread() const							{ return m_flBulletSpread; }
	FORCEINLINE float	GetMaxShootDistance() const						{ return m_flmaxTraceLength; }
	FORCEINLINE float	GetImpactImpulse() const						{ return m_flImpulse; }
	FORCEINLINE float   GetAttackRate() const							 { return m_flAttackRate; }
	FORCEINLINE	FVector GetShotForwardVector() const;												// Return forward vector .cpp
	FORCEINLINE FVector CalculateBulletSpread(const FVector& ShotDirection) const;
	
	// Methods for setting values
	FORCEINLINE	void	SetSkeletalMesh(USkeletalMeshComponent* NewMesh) { WeaponSkeletalMeshComponent = NewMesh; }
	FORCEINLINE void	SetMaxShootDistance(float fldistance)			 { m_flmaxTraceLength = fldistance; }
	FORCEINLINE void	SetSocetName(FName newName)					     { fSocketName = newName; }
	FORCEINLINE void	SetMaxAmmo(int32 iAmmo)							 { imaxAmmo = iAmmo; }
	FORCEINLINE void	SetCurrentAmmo(int32 iAmmo)						 { icurrentAmmo = iAmmo; }
	FORCEINLINE void	SetInvAmmo(int32 i_invammo)						 { imaxInventoryAmmo = i_invammo; }
	FORCEINLINE void	SetReloadTime(float flnew_time)					 { m_flReloadTime = flnew_time; }
	FORCEINLINE void    SetAttackRate(float fl_rateattack)				 { m_flAttackRate = fl_rateattack; }
	FORCEINLINE void	SetBulletSpread(float flspread)					 { m_flBulletSpread = flspread; }
  	FORCEINLINE void    SetImpulseImpact(float fl_impulse)				 { m_flImpulse = fl_impulse; }

	// Sound effects and load
	FORCEINLINE void	SetFireSound(USoundBase* NewSound)				{ FireSound = NewSound; }
	FORCEINLINE void	SetReloadSound(USoundBase* NewSound)			{ ReloadSound = NewSound; }
	FORCEINLINE void	SetMuzzleFlash(UParticleSystem* NewEffect)		{ MuzzleFlash = NewEffect; }
	FORCEINLINE void	LoadSkeletalMesh(const FString& Path) const; 
	UFUNCTION(BlueprintCallable)
	FORCEINLINE	void	AttachWeapon(AMyCharacter* Character, const FName&  SocketName);

	virtual void	PrimaryAttack();	
	//virtual void SecondaryAttack();
	virtual void	Reload();
	virtual void	FinishReload();
	virtual void	Interact(AActor* Actor) override;
	virtual void	PhysicsTraceLogic(const FHitResult& HitResult);
	virtual void	ApplyDamage(float Damage, FVector HitLocation) override;
	virtual void	StartAttack();
	virtual void	StopAttack();
			void	SpawnEmitter() const;
			void	SpawnTraceDecals() const;
			void	ConsumeAmmo(int32 iAmmo);
	
	EFireMode CurrentFireMode;	

	
private:

	UPROPERTY()
	TObjectPtr<AMyCharacter> Player; 
	UPROPERTY()	
	TObjectPtr<USoundBase> FireSound;
	UPROPERTY()	
	TObjectPtr<USoundBase> ReloadSound;
	UPROPERTY()
	TObjectPtr<UParticleSystem> MuzzleFlash;
	UPROPERTY(EditAnywhere, Category="Anim")
	TObjectPtr<UAnimMontage> aminPrimaryAttack;
	
	FTimerHandle ReloadTimer;
	FTimerHandle PrimaryAttackTimer;

	int32	 imaxAmmo;
	int32	 icurrentAmmo;
	int32	 imaxInventoryAmmo;
	FName	 fSocketName;			// Socket for shoot 

	float    m_flImpulse;	
	float	 m_flmaxTraceLength;
	float    m_flBulletSpread;
	float    m_flReloadTime;
	float    m_flAttackRate;
	
	bool     blsReload;
	bool     blsPrimaryAttack;
	
};
