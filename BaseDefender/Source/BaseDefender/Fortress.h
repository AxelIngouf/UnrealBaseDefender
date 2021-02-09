// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fortress.generated.h"

UCLASS()
class BASEDEFENDER_API AFortress : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFortress();

	class UHealthComponent* GetHealthComponent();

protected:

	UFUNCTION(BlueprintCallable, Category = Damage)
	void	EventDamage(float Damage);

	/**
	 * @brief Allows the player to stop taking damage when true
	 * @param Value Should the player be invulnerable or not
	 */
	UFUNCTION(BlueprintCallable, Category = Damage)
	void	SetFortressGodmode(const bool Value) { Godmode = Value; }

	UPROPERTY(EditAnywhere, Category = Mesh)
	class UStaticMeshComponent* StaticMesh = nullptr;
	
    /**
	 * @brief Fortress Collision.
	 */
	UPROPERTY(VisibleDefaultsOnly, Category = Hitbox)
	class UBoxComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, Category = AntiTrap)
	class UAntiTrapComponent* AntiTrapComponent = nullptr;

    /**
	 * @brief Fortress Health component. Containing health and maximum health values.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	class UHealthComponent* Health;

	bool	Godmode; // Is the fortress invulnerable

};
