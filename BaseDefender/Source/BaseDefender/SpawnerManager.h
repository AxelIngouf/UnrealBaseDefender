// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnerManager.generated.h"

class ABaseDefenderGameMode;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyNumChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaveStateChange);

/**
 *
 */
UCLASS()
class BASEDEFENDER_API ASpawnerManager : public AActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void SpawnerDone();
	void DeadEnemy();
	void SpawnedEnemy();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Wave)
		float DelayBeforeFirstWave = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Wave)
		float DefaultDelayIfNotPresent = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Wave)
		TArray<float> DelayAfterWave;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Wave)
		bool bIsOnBreak = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spawner)
		int EnemiesLeft = 0;

	UPROPERTY(BlueprintAssignable, Category = Enemy)
		FEnemyNumChange OnEnemyNumChange;

	UPROPERTY(BlueprintAssignable, Category = Wave)
		FWaveStateChange OnWaveStateChange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spawner)
		float BreakTimeLeft = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Wave)
		int WaveId = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Wave)
		int MostWaves = 0;

private:
	TArray<class ASpawner*> Spawners;

	void SpawnNextWave();

	FTimerHandle TimerHandle_Wave;

	ABaseDefenderGameMode* GameMode = nullptr;
};
