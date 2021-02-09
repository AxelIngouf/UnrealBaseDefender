// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trap.h"
#include "TrapWeakZone.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENDER_API ATrapWeakZone : public ATrap
{
	GENERATED_BODY()

public:
	ATrapWeakZone();

	void	WeakenedEnemy();

protected:
	virtual void	BeginPlay() override;

	virtual void	BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void	EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	virtual float	Damaged() override;

    /**
     * @brief Timer between each time the trap will take damage for every enemy inside it
     */
	UPROPERTY(EditAnywhere, Category = Trap)
	float	TimerDamageWhenWeakeningEnemies = 0.4f;

private:
	int	NbrEnemies=0;
	FTimerHandle	TimerHandle_Damaged;
};
