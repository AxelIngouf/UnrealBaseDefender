// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Trap.h"
#include "TrapDamage.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENDER_API ATrapDamage : public ATrap
{
	GENERATED_BODY()
	
public:
	ATrapDamage();

	virtual void EffectIntensity(float IntensityRatio) override;

protected:

	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

    /**
	 * @brief Causes pain to selected actor if possible. Returns trap current health.
	 * @param Actor - Actor to damage
	 * @return Trap current health points
	 */
	float	CausePainTo(AActor* Actor);

	void	PainTimer();

	class AController* DamageInstigator;

    /**
	 * @brief Time between each trap damage.
	 */
	UPROPERTY(EditAnywhere, Category = Trap)
	float	PainInterval;

    /**
	 * @brief Amount of damages dealt by the trap every second.
	 */
	UPROPERTY(EditAnywhere, Category = Trap)
	float	DamagePerSecond;

	FTimerHandle	TimerHandle_AutoDestruct;
	FTimerHandle	TimerHandle_PainTimer;
};
