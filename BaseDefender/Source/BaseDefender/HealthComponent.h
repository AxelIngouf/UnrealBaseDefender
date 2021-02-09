// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASEDEFENDER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

public:
	virtual void BeginPlay() override;

    /**
	 * @brief Apply damages on health points and returns the amount of actual damages taken
	 * @param Damage - Amount of damage to apply to health
	 * @return Amount of final damages inflicted
	 */
	UFUNCTION(BlueprintCallable, Category = "Health")
	float	ApplyComponentDamage(float Damage);

	UFUNCTION(BlueprintCallable, Category = "Health")
	float	GetHealthPoints() const { return HealthPoints; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	float	GetMaxHealthPoints() const { return MaxHealthPoints; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	float	GetHealthPercent() const { return HealthPercent; }

    /**
	 * @brief Changes Max Health points and thus, health points.
	 * @param MaxHealth - New max health points value
	 */
	void	SetMaxHealthPoints(float MaxHealth);

    /**
	 * @brief Add health points to the current number. Cannot go over MaximumHealthPoints variable.
	 * @param Health - Health points to add.
	 */
	void	AddHealthPoints(const float Health);

    /**
	 * @brief Current owner health points.
	 */
	UPROPERTY(BlueprintReadWrite, Category = Health)
	float	HealthPoints;

    /**
	 * @brief Maximum owner health points.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float	MaxHealthPoints;

    /**
	 * @brief Health percentage from 0 to 1. Current owner health points divided by maximum health points.
	 */
	UPROPERTY(BlueprintReadOnly, Category = Health)
	float	HealthPercent;
	
};
