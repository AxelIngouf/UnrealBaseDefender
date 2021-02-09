// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"

#include "BaseDefenderCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

#define MAXCOINZ 25
#define MOVEMENT_REFRESH 0.03

// Sets default values
ACoin::ACoin()
{
    MoneyAmount = 10.f;
    TimerAutoDestruct = 120.f;

    CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
    Cast<UCapsuleComponent>(CollisionComponent)->InitCapsuleSize(10, 10);
    RootComponent = CollisionComponent;

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere")).Object);
    StaticMesh->AddRelativeRotation(FRotator(0, 0, 90));
    StaticMesh->SetCollisionProfileName(FName(TEXT("NoCollision")));
    StaticMesh->SetupAttachment(RootComponent);

}

void ACoin::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                         int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(ABaseDefenderCharacter* Player = Cast<ABaseDefenderCharacter>(OtherActor))
    {
        if(Player)
        {
            Player->AddCoins(MoneyAmount);
            OnPickedUp.Broadcast();
            AutoDestruct();
        }
    }
}

void ACoin::BeginPlay()
{
    Super::BeginPlay();

    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACoin::BeginOverlap);

    GetWorldTimerManager().SetTimer(TimerHandle_AutoDestruct, this, &ACoin::AutoDestruct, TimerAutoDestruct);
    GetWorldTimerManager().SetTimer(TimerHandle_Movement, this, &ACoin::Movement, MOVEMENT_REFRESH, true);
}

void ACoin::AutoDestruct()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_AutoDestruct);
    Destroy();
}

void ACoin::Movement()
{
    if (bAscending)
    {
        if (MovementSum >= MAXCOINZ)
        {
            ZMovement = -.4f;
            bAscending = false;
        }
    }
    else
    {
        if (MovementSum <= 0.f)
        {
            ZMovement = .4f;
            bAscending = true;
        }
    }
    MovementSum += ZMovement;
    AddActorLocalOffset(FVector(0.f, 0.f, ZMovement));

    AddActorLocalRotation(FRotator(0.f, 4.f, 0.f));
}

