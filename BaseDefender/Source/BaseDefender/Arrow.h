// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDestroy);

UCLASS()
class BASEDEFENDER_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

    /**
	 * @brief Projectile hitbox.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hitbox)
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
	class UProjectileMovementComponent* MovementComponent;

	float	GetDamage() const;

	void	SetDamage(float NewDamage);

	UFUNCTION(BlueprintCallable, Category = Destruct)
	void	AutoDestruct();

	UPROPERTY(BlueprintAssignable, Category = DestroyEvent)
	FDestroy OnDestroy;

private:
	float	Damage;

	class AController* DamageInstigator;

};
