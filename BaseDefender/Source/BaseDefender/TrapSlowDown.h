// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Trap.h"
#include "TrapSlowDown.generated.h"

class ABaseDefenderCharacter;
/**
 * 
 */
UCLASS()
class BASEDEFENDER_API ATrapSlowDown : public ATrap
{
	GENERATED_BODY()
		
public:
	ATrapSlowDown();

private:
	virtual void	BeginPlay() override;

	virtual void	BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void	EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	virtual float	Damaged() override;

	virtual void	AutoDestruct() override;

	void	SlowedEnemy();

	void	SlowDownPlayer() const;
	void	SpeedUpPlayer() const;

    /**
	 * @brief Timer between each time the trap will take damage for every enemy inside it
	 */
	UPROPERTY(EditAnywhere, Category = Trap)
	float	TimerDamageWhenSlowingEnemies = 0.4f;

	int		NumberActorOverlapped;


	FTimerHandle TimerHandle_Damaged;
};
