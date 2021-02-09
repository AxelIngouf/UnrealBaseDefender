// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrapDamage.h"
#include "TrapDamageLine.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENDER_API ATrapDamageLine : public ATrapDamage
{
	GENERATED_BODY()

public:
	ATrapDamageLine();

	virtual void DeactivateTrap() override;
};
