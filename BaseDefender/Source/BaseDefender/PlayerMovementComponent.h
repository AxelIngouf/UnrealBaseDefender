// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENDER_API UPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UPlayerMovementComponent();

	void	AddSpeedBonus();
	void	DeleteSpeedBonus();
	void	Sprint();
	void	StopSprint();
	void	SoftStopSprint();

	bool	IsHoldingSprint() const { return bHoldingSprint; }

	bool	IsSprinting() const { return bSprinting; }

private:

    /**
	 * @brief Maximum player speed.
	 */
	UPROPERTY(EditAnywhere, Category = Speed)
	float	MaxSpeed;

    /**
	 * @brief Speed bonus the player will get each time an Enemy enter a SlowDown trap. The same is subtracted once the enemy left or died.
	 */
	UPROPERTY(EditAnywhere, Category = Speed)
	float	SpeedBonusUnit;

    /**
	 * @brief Speed bonus added to the current speed when the player starts sprinting.
	 */
	UPROPERTY(EditAnywhere, Category = Speed)
	float	SprintSpeedBonus;

	bool	bSprinting;

	bool	bHoldingSprint = false;
};
