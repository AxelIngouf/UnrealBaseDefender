// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseDefenderGameMode.h"

#include "PlayerControllerBaseDefender.h"
#include "BaseDefender/Fortress.h"
#include "BaseDefender/HealthComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ABaseDefenderGameMode::ABaseDefenderGameMode() : AGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerControllerClass = APlayerControllerBaseDefender::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	Timer = 360;

	TArray<AActor*> Fortresses;
	UGameplayStatics::GetAllActorsOfClass(this, AFortress::StaticClass(), Fortresses);

	for (auto& Fort : Fortresses)
	{
		AFortress* Fortress = Cast<AFortress>(Fort);
		if (Fortress)
		{
			this->FortressHealthMax += Fortress->GetHealthComponent()->GetMaxHealthPoints();
		}
	}
	FortressHealth = FortressHealthMax;
}

float ABaseDefenderGameMode::GetTimer() const
{
	return Timer;
}

void ABaseDefenderGameMode::ApplyFortressDamage(float Damage)
{
	FortressHealth -= Damage;
	if (FortressHealth <= 0.f)
	{
		bWon = false;
		OnGameEnd.Broadcast();
	}
}

float ABaseDefenderGameMode::GetFortressHealthMax() const
{
	return FortressHealthMax;
}

float ABaseDefenderGameMode::GetFortressHealth() const
{
	return FortressHealth;
}

void ABaseDefenderGameMode::CheckPlayerHealth(float PlayerHealthPercent)
{
	if (PlayerHealthPercent <= 0.f)
	{
		bWon = false;
		OnPlayerDeath.Broadcast();
		OnGameEnd.Broadcast();
	}
}

void ABaseDefenderGameMode::AllWavesDone()
{
	bWon = true;
	OnGameEnd.Broadcast();
}

void ABaseDefenderGameMode::TrapDestroyedAtLocation(const FVector TrapLocation) const
{
	OnTrapDestroyed.Broadcast(TrapLocation);
}
