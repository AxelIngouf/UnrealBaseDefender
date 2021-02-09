// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

USTRUCT(BlueprintType)
struct FSpawnerSubWave
{
	GENERATED_USTRUCT_BODY()

	FSpawnerSubWave() : AliveEnemies(0)
	{
		SpawnTimer = 5.f;
		NumberOfEnemies = 5.f;
		bWaitPreviousWaveEnd = true;
	}

	/**
	 * @brief Number of seconds the current wave waits after the previous one was spawned before trying to spawn.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	float SpawnTimer;

	/**
	 * @brief Number of enemies spawned in this wave.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	int NumberOfEnemies;

	/**
	 * @brief Indicate if the current wave wait for previous wave enemies to die before spawning.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	bool bWaitPreviousWaveEnd;

	int	AliveEnemies; // Number of enemies still alive

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	TSubclassOf<class AEnnemy> SpawnedGruntType = nullptr;
};

USTRUCT(BlueprintType)
struct FSpawnerWave
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	TArray<FSpawnerSubWave> SubWaves;

	FSpawnerSubWave& operator[](size_t idx)
	{
		return SubWaves[idx];
	}
};

UCLASS()
class BASEDEFENDER_API ASpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawner();

	void	SpawnWave(int WaveId);
	void	SpawnSubWave();
	void	DeadEnemy(int WaveId, int SubWaveId);
	bool	IsWaveDone(int WaveId);

	void	SetSpawnerManger(class ASpawnerManager* Mgr);

	int		GetNumWaves();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * @brief Waves list.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	TArray<FSpawnerWave> Waves;

private:
	int CurrentWaveIndex;
	int CurrentSubWaveIndex;

	FTimerHandle TimerHandle_Spawn;

	ASpawnerManager* SManager = nullptr;
};
