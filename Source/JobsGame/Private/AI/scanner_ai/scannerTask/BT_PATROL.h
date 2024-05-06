﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BT_PATROL.generated.h"

UCLASS()
class JOBSGAME_API UBT_PATROL : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBT_PATROL();

public:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category="AI")
	float SearchLocation = 500;
};

