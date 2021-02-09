// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTeleport);

UCLASS()
class BASEDEFENDER_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    /**
     * @brief Gives the exact point where the player needs to be teleported, which is represented by TeleportationPoint StaticMeshComponent
     * @return Teleport Location FVector
     */
    FVector GetTeleportLocation() const;

    UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    /**
	 * @brief Portal collision. The player has to touch this component to get teleported.
	 */
	UPROPERTY(EditAnywhere, Category = BoxCollision)
	class UShapeComponent* CollisionComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = AntiTrap)
	class UAntiTrapComponent* AntiTrapComponent = nullptr;

    /**
	 * @brief Portal mesh.
	 */
	UPROPERTY(EditAnywhere, Category = Mesh)
	class UStaticMeshComponent* PortalMesh = nullptr;

    /**
	 * @brief The exact point the player get sent to when he teleports to this Portal.
	 */
	UPROPERTY(EditAnywhere, Category = TeleportationPoint)
	class UStaticMeshComponent* TeleportationPoint = nullptr;

    /**
	 * @brief Other Portal where this one will bring the player to when he enters it.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DestinationPortal)
	APortal* AssociatedPortal = nullptr;

	UPROPERTY(BlueprintAssignable, Category = Teleport)
	FTeleport OnTeleport;
};
