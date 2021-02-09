// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovementComponent.h"

#include "BaseDefenderCharacter.h"

UPlayerMovementComponent::UPlayerMovementComponent()
{
	MaxSpeed = 600.0f;
    MaxWalkSpeed = MaxSpeed;
	SprintSpeedBonus = MaxSpeed;
    SpeedBonusUnit = MaxWalkSpeed / 10.0f;

	bOrientRotationToMovement = true; // Character moves in the direction of input...	
	RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	JumpZVelocity = 600.f;
	AirControl = 0.2f;
}

void UPlayerMovementComponent::AddSpeedBonus()
{
	MaxWalkSpeed += SpeedBonusUnit;
}

void UPlayerMovementComponent::DeleteSpeedBonus()
{
	if(MaxWalkSpeed > MaxSpeed)
	    MaxWalkSpeed -= SpeedBonusUnit;
}

void UPlayerMovementComponent::Sprint()
{
	if(!bSprinting)
	{
		ABaseDefenderCharacter* Player = Cast<ABaseDefenderCharacter>(GetOwner());
		if(Player)
		{
			if(Player->IsMovingForward())
			{
				MaxWalkSpeed += SprintSpeedBonus;
				bHoldingSprint = false;
				bSprinting = true;
			}
			else
			{
				bHoldingSprint = true;
				bSprinting = false;
			}
		}
	}
}

void UPlayerMovementComponent::StopSprint()
{
	if (bSprinting)
	{
		if(!bHoldingSprint)
		    MaxWalkSpeed -= SprintSpeedBonus;
		bSprinting = false;
		bHoldingSprint = false;
	}
}

void UPlayerMovementComponent::SoftStopSprint()
{
	if(bSprinting)
	{
		if(!bHoldingSprint)
		    MaxWalkSpeed -= SprintSpeedBonus;
		bHoldingSprint = true;
		bSprinting = false;
	}
}
