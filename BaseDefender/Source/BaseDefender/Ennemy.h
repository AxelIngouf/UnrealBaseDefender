// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Ennemy.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDamaged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeath);

class ATrapWeakZone;
UCLASS()
class BASEDEFENDER_API AEnnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnnemy();

protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:

	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;

	void	Die();
	void	Loot() const;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void	Attack();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void	StopAttack();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	bool	IsAttack() const;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void	RotateTowardPlayer();

	UFUNCTION(BlueprintCallable, Category = "Health")
	class UHealthComponent* GetHealthComponent() const;

	float	GetAttackSpeed() const { return AttackSpeed; }
	bool	IsSlowedDown() const;
	void	SetSlowDown(const bool Slow);
	void	DeleteWeakEffectSource();
	void	SetSpawnOrigin(class ASpawner* Spawn, int Wave, int SubWave);

	UPROPERTY(BlueprintAssignable, Category = Damage)
	FDamaged OnDamaged;

	UPROPERTY(BlueprintAssignable, Category = Death)
	FDeath OnDeath;

	/* Variables */

	UPROPERTY(EditAnywhere, Category = Characteristics)
	TSubclassOf<class ALifePotion> LifePotionType = nullptr;

	UPROPERTY(EditAnywhere, Category = Characteristics)
	TSubclassOf<class ACoin> CoinType = nullptr;

    /**
	 * @brief Enemy Health component. Containing health and maximum health values.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristics)
	UHealthComponent* Health = nullptr;

    /**
	 * @brief Enemy power. Number of damages the enemy can deal to its target.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristics)
	float	Power;

    /**
	 * @brief Enemy maximum speed.
	 */
	UPROPERTY(EditAnywhere, Category = Characteristics)
	float	MaxSpeed;

    /**
	 * @brief SlowDown trap effect. Enemy speed will be replaced by this one when it's in a SlowDown trap.
	 */
	UPROPERTY(EditAnywhere, Category = Characteristics)
	float	TrapSlowDownSpeed;

    /**
	 * @brief Ratio of the WeakZone trap effect. Damage taken will be multiplied by this one.
	 */
	UPROPERTY(EditAnywhere, Category = Characteristics)
	float	TrapWeakEffectRatio;

    /**
	 * @brief Chances the enemy have to drop Coins. The bigger the number, the smaller the chance.
	 */
	UPROPERTY(EditAnywhere, Category = Characteristics)
	int		ChanceCoinDrop;

    /**
	 * @brief Maximum number of coins an Enemy can drop on death. From 0 to this number.
	 */
	UPROPERTY(EditAnywhere, Category = Characteristics)
	int		MaxCoinsOnDeath;

    /**
	 * @brief Chances the enemy have to drop a Health Potion. The bigger the number, the smaller the chance.
	 */
	UPROPERTY(EditAnywhere, Category = Characteristics)
	int		ChanceHealthDrop;

    /**
	 * @brief Speed at which the enemy attacks. Also changes animation speed.
	 */
	UPROPERTY(EditAnywhere, Category = Characteristics, meta = (ClampMin = 0.0, ClampMax = 1.0))
	float	AttackSpeed = 1.f;

	float	TimerDestroyAfterDeath = 30.f;

	bool	bAttacking;
	bool	bSlowedDown;

	class ATrapWeakZone*	WeakEffectSource = nullptr;

	ASpawner*	SpawnOrigin = nullptr;
	int	WaveId = 0;
	int SubWaveId = 0;

private:
	void	AutoDestruct();

	FTimerHandle TimerHandle_DestroyAfterDeath;
};
