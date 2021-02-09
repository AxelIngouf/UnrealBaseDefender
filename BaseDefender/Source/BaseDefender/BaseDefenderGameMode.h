// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseDefenderGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTrapDestroyed, FVector, TrapLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameEnd);

UCLASS(minimalapi)
class ABaseDefenderGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABaseDefenderGameMode();

	UFUNCTION(BlueprintCallable, Category = Timer)
	float	GetTimer() const;

	UFUNCTION(BlueprintCallable, Category = Fortress)
	void	ApplyFortressDamage(float Damage);

	UFUNCTION(BlueprintCallable, Category = Fortress)
	float	GetFortressHealthMax() const;

	UFUNCTION(BlueprintCallable, Category = Fortress)
	float	GetFortressHealth() const;

	void	CheckPlayerHealth(float PlayerHealthPercent);

	void	AllWavesDone();

    /**
	 * @brief Used for sound purposes
	 * @param TrapLocation Destroyed trap location
	 */
	void	TrapDestroyedAtLocation(FVector TrapLocation) const;

	UPROPERTY(BlueprintReadOnly, Category = Gamemode)
	bool	bWon = false;

	
	UPROPERTY(BlueprintAssignable, Category = Wave)
	FGameEnd OnGameEnd;

	UPROPERTY(BlueprintAssignable, Category = Player)
	FPlayerDeath OnPlayerDeath;

	UPROPERTY(BlueprintAssignable, Category = Trap)
	FTrapDestroyed OnTrapDestroyed;

private:

	UPROPERTY(EditAnywhere, Category = Gamemode)
	float	Timer;

	float	FortressHealth;
	float	FortressHealthMax;
};



