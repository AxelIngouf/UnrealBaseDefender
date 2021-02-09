// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LifePotion.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPickedUp);

UCLASS()
class BASEDEFENDER_API ALifePotion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALifePotion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    /**
	 * @brief Potion Collision.
	 */
	UPROPERTY(EditAnywhere, Category = Collision)
	class UShapeComponent* CollisionComponent = nullptr;

    /**
	 * @brief Potion Mesh.
	 */
	UPROPERTY(EditAnywhere, Category = Mesh)
	class UStaticMeshComponent* StaticMesh = nullptr;

    /**
	 * @brief Amount of health the Potion will give back.
	 */
	UPROPERTY(EditAnywhere, Category = Potion)
	float	HealthBonusAmount;

	float	DeltaSave=0.f;
	bool	bAscending; // Used for up-down animation

	UPROPERTY(BlueprintAssignable, Category = Potion)
	FPickedUp OnPickedUp;

};
