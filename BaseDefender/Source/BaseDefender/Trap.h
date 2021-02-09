// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trap.generated.h"

UCLASS()
class BASEDEFENDER_API ATrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrap();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION()
	virtual void	BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}

	UFUNCTION()
	virtual void	EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {}

	UFUNCTION()
	virtual void	AutoDestruct();

	void	ProgressiveAutoDestruction();

	virtual void	DeactivateTrap();

	virtual void	EffectIntensity(float IntensityRatio) {}

	FString	GetTrapName() const { return TrapName; };

	void	ResizeTrap(float ResizeRatio) const;

	void	SetTrapIndex(int Index);

	class UHealthComponent*	GetHealthComponent() const { return Health; }

protected:
	virtual float	Damaged();

	/**
	 * @brief Trap Collision component
	 */
	UPROPERTY(EditAnywhere, Category = Collision)
	class UShapeComponent*		CollisionComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = Mesh)
	class UStaticMeshComponent*	StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = AntiTrap)
	class UAntiTrapComponent*	AntiTrapComponent = nullptr;

	/**
	 * @brief Trap Health component. Containing health and maximum health values.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trap)
	UHealthComponent*	Health;

    /**
	 * @brief Amount of damage taken every time the trap is used. Slow/Weak trap when an Enemy leave the zone. Damage trap when damage is done.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trap)
	float	DamageTakenOnUse;

    /**
	 * @brief Amount of damage taken every AutoDamage interval. Used to simulate a progressive auto-destruction.
	 */
	UPROPERTY(EditAnywhere, Category = Trap)
	float	DamageAutoDestruction=5.f;

    /**
	 * @brief Indicates if the current trap is activated or not, thus if it's a player placeholder trap or not
	 */
	UPROPERTY(BlueprintReadOnly, Category = Trap)
	bool	bActivated;

	int		TrapIndexPlayerInventory;

    /**
	 * @brief Timer interval of trap auto-destruction. This will simulate a progressive loss of health points.
	 */
	UPROPERTY(EditAnywhere, Category = Trap)
	float	TimerAutoDamage = 2.f;

    /**
	 * @brief Trap name displayed in the shop.
	 */
	UPROPERTY(EditAnywhere, Category = Trap)
	FString	TrapName;

	FTimerHandle	TimerHandle_AutoDamage;
};
