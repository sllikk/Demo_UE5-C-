// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "TestCheats.generated.h"

/**
 * 
 */
UCLASS()
class JOBSGAME_API UTestCheats : public UCheatManager
{
	GENERATED_BODY()

public:
	// Useful for identifying hanging pointers that could cause a crash when
	UFUNCTION( exec )
	void ForceGarbageCollection();
	
	// Spawn Props
	UFUNCTION( exec )
	void spawn_flamebarrel();

	UFUNCTION( exec )
	void spawn_healthkit();	
	
	UFUNCTION( exec )
	void spawn_batterykit();	

	
private:

	FORCEINLINE void SpawnInteractObject(TSubclassOf<AActor>) const;
};
