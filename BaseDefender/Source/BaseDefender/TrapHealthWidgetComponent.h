// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "TrapHealthWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENDER_API UTrapHealthWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = Health)
	class UHealthComponent* HealthComponentReference;
	
};
