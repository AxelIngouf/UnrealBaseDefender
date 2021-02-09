// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnerManager.h"

#include "BaseDefenderGameMode.h"
#include "Spawner.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void ASpawnerManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> SpawnerActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawner::StaticClass(), SpawnerActors);

	GameMode = Cast<ABaseDefenderGameMode>(GetWorld()->GetAuthGameMode());

	for (int i = 0; i < SpawnerActors.Num(); i++)
	{
		Spawners.Add(Cast<ASpawner>(SpawnerActors[i]));
	}

	for (auto& s : Spawners)
	{
		if (s == nullptr)
			continue;

		s->SetSpawnerManger(this);

		if (MostWaves < s->GetNumWaves())
			MostWaves = s->GetNumWaves();
	}
	
	bIsOnBreak = true;
	BreakTimeLeft = DelayBeforeFirstWave;
	OnWaveStateChange.Broadcast();
	OnEnemyNumChange.Broadcast();

	GetWorldTimerManager().SetTimer(
		TimerHandle_Wave,
		this,
		&ASpawnerManager::SpawnNextWave,
		DelayBeforeFirstWave
	);
}

void ASpawnerManager::SpawnerDone()
{
	bool AllSubWavesDone = true;
	for (auto& s : Spawners)
	{
		if (s == nullptr)
			continue;

		if (!s->IsWaveDone(WaveId))
		{
			AllSubWavesDone = false;
			break;
		}
	}

	if (AllSubWavesDone)
	{
		WaveId++;

		if (WaveId >= MostWaves)
		{
			GameMode->AllWavesDone();
		}
		else
		{
			int NextDelay = DefaultDelayIfNotPresent;
			if (DelayAfterWave.Num() > WaveId && DelayAfterWave[WaveId] == 0.f)
				NextDelay = DelayAfterWave[WaveId];

			bIsOnBreak = true;
			BreakTimeLeft = NextDelay;

			GetWorldTimerManager().SetTimer(
				TimerHandle_Wave,
				this,
				&ASpawnerManager::SpawnNextWave,
				NextDelay
			);

			OnWaveStateChange.Broadcast();
		}
	}
}

void ASpawnerManager::SpawnNextWave()
{
	bIsOnBreak = false;
	OnWaveStateChange.Broadcast();

	for (auto& s : Spawners)
	{
		if (s == nullptr)
			continue;

		s->SpawnWave(WaveId);
	}
}

void ASpawnerManager::DeadEnemy()
{
	EnemiesLeft--;
	OnEnemyNumChange.Broadcast();
}

void ASpawnerManager::SpawnedEnemy()
{
	EnemiesLeft++;
	OnEnemyNumChange.Broadcast();
}
