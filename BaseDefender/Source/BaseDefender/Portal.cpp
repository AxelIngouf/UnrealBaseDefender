// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"

#include "BaseDefenderCharacter.h"
#include "AntiTrapComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	PortalMesh->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone'")).Object);
	RootComponent = PortalMesh;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	Cast<UBoxComponent>(CollisionComponent)->InitBoxExtent({30, 30, 30});
	CollisionComponent->SetupAttachment(RootComponent);

	AntiTrapComponent = CreateDefaultSubobject<UAntiTrapComponent>("AntiTrapComponent");
	AntiTrapComponent->SetupAttachment(CollisionComponent);

	TeleportationPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportationPoint"));
	TeleportationPoint->SetRelativeLocation(FVector(400.f, 0.f, 0.f));
	TeleportationPoint->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone'")).Object);
	TeleportationPoint->SetWorldScale3D(FVector(0.2f));
	TeleportationPoint->SetCollisionProfileName(FName(TEXT("NoCollision")));
	TeleportationPoint->SetHiddenInGame(true);
	TeleportationPoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APortal::BeginOverlap);

	if(AssociatedPortal == nullptr)
	{
		TArray<AActor*> Portals;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APortal::StaticClass(), Portals);

		if (Portals.Num() > 1)
		{
			for (AActor* Actor : Portals)
			{
				if (APortal* OtherPortal = Cast<APortal>(Actor))
				{
					if (OtherPortal != this)
					{
						// Search for other portals on map, and associate the first found to this one
						AssociatedPortal = OtherPortal;
						break;
					}
				}
			}
		}
	}

}

FVector APortal::GetTeleportLocation() const
{
	return TeleportationPoint->GetComponentLocation();
}

void APortal::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(ABaseDefenderCharacter* Player = Cast<ABaseDefenderCharacter>(OtherActor))
	{
	    if(AssociatedPortal)
	    {
			OnTeleport.Broadcast();
			Player->SetActorLocation(AssociatedPortal->GetTeleportLocation());
			Player->SetActorRelativeRotation(AssociatedPortal->TeleportationPoint->GetRelativeRotation());
	    }
	}
}

