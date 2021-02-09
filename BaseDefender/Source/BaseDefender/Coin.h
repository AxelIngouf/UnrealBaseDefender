// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coin.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCoinPickedUp);

UCLASS()
class BASEDEFENDER_API ACoin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoin();

    UFUNCTION()
    virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Collision)
	class UShapeComponent* CollisionComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = Mesh)
	class UStaticMeshComponent* StaticMesh = nullptr;

	UPROPERTY(BlueprintAssignable, Category=Coin)
	FCoinPickedUp OnPickedUp;

private:
	void AutoDestruct();

	void Movement();

	UPROPERTY(EditAnywhere, Category = Coin)
	int	MoneyAmount;

	float	TimerAutoDestruct;
	bool	bAscending;
	float	ZMovement;
	float	MovementSum;

	FTimerHandle TimerHandle_AutoDestruct;
	FTimerHandle TimerHandle_Movement;
};
