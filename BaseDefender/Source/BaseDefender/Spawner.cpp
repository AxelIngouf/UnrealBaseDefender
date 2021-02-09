// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner.h"

#include "BaseDefender/Ennemy.h"
#include "Kismet/GameplayStatics.h"
#include "BaseDefender/SpawnerManager.h"

// Sets default values
ASpawner::ASpawner()
{
	Waves.Add(FSpawnerWave()); // Wave default example
	CurrentWaveIndex = 0;
	CurrentSubWaveIndex = 0;
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
}

void ASpawner::DeadEnemy(const int WaveId, const int SubWaveId)
{
	Waves[WaveId][SubWaveId].AliveEnemies -= 1;

	if (SManager != nullptr)
	{
		SManager->DeadEnemy();
		
		if (IsWaveDone(WaveId))
			SManager->SpawnerDone();
	}
}

void ASpawner::SpawnWave(const int WaveId)
{
	CurrentWaveIndex = WaveId;
	CurrentSubWaveIndex = 0;

	if (Waves.Num() > WaveId)
		if (Waves[CurrentWaveIndex].SubWaves.Num() > 0)
			SpawnSubWave();
}

void ASpawner::SpawnSubWave()
{
	UWorld* MyWorld = GetWorld();

	if (MyWorld == nullptr)
		return;

	// If the previous wave needs to wait for the current one to end, then you wait for every previous wave Enemies to die and retry 2 seconds later
	if (
		CurrentSubWaveIndex > 0
		&& Waves[CurrentWaveIndex][CurrentSubWaveIndex].bWaitPreviousWaveEnd
		&& Waves[CurrentWaveIndex][CurrentSubWaveIndex - 1].AliveEnemies > 0
		)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_Spawn, this, &ASpawner::SpawnSubWave, 2.f);
		return;
	}

	for (int iEnemy = 0; iEnemy < Waves[CurrentWaveIndex][CurrentSubWaveIndex].NumberOfEnemies; iEnemy++)
	{
		// Try to spawn the enemy until it finds a valid position.

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AEnnemy* Enemy = MyWorld->SpawnActor<AEnnemy>(
			Waves[CurrentWaveIndex][CurrentSubWaveIndex].SpawnedGruntType,
			GetActorLocation(),
			FRotator::ZeroRotator,
			SpawnParameters
			);

		if (Enemy == nullptr)
			continue;
		

		Enemy->SetSpawnOrigin(this, CurrentWaveIndex, CurrentSubWaveIndex);
		Waves[CurrentWaveIndex][CurrentSubWaveIndex].AliveEnemies += 1;

		if (SManager != nullptr)
			SManager->SpawnedEnemy();

		
	}

	CurrentSubWaveIndex++;

	if (Waves[CurrentWaveIndex].SubWaves.Num() <= CurrentSubWaveIndex)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Spawn);
	}
	else
	{
		GetWorldTimerManager().SetTimer(
			TimerHandle_Spawn,
			this,
			&ASpawner::SpawnSubWave,
			Waves[CurrentWaveIndex][CurrentSubWaveIndex].SpawnTimer
		);
	}
}

bool ASpawner::IsWaveDone(int WaveId)
{
	if (Waves.Num() <= WaveId)
		return true;

	if (CurrentSubWaveIndex < Waves[WaveId].SubWaves.Num())
		return false;

	for (const auto& sw : Waves[WaveId].SubWaves)
	{
		if (sw.AliveEnemies != 0)
			return false;
	}

	return true;
}

void ASpawner::SetSpawnerManger(ASpawnerManager* Mgr)
{
	SManager = Mgr;
}

int ASpawner::GetNumWaves()
{
	return Waves.Num();
}