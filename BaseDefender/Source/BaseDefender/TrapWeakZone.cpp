// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapWeakZone.h"
#include "AntiTrapComponent.h"
#include "Ennemy.h"
#include "Components/BoxComponent.h"

ATrapWeakZone::ATrapWeakZone() : ATrap()
{
    TrapName = FString("Weak Zone");

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere")).Object);
    StaticMesh->SetCollisionProfileName(FName(TEXT("NoCollision")));
    RootComponent = StaticMesh;

    AntiTrapComponent = CreateDefaultSubobject<UAntiTrapComponent>("AntiTrapComponent");
    AntiTrapComponent->SetupAttachment(RootComponent);

    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComponent"));
    Cast<UBoxComponent>(CollisionComponent)->InitBoxExtent(FVector(300.f, 150.f, 100.f));
    CollisionComponent->SetupAttachment(RootComponent);
}

void ATrapWeakZone::WeakenedEnemy()
{
    this->Damaged();
}

void ATrapWeakZone::BeginPlay()
{
    Super::BeginPlay();

    AddActorLocalOffset({ 0, 0, 20 });
}

void ATrapWeakZone::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(AEnnemy* OtherEnemy = Cast<AEnnemy>(OtherActor))
    {
        if (OtherComp->GetName() == FString("CollisionCylinder"))
        {
            if (OtherEnemy->WeakEffectSource == nullptr)
            {
                OtherEnemy->WeakEffectSource = this;
                NbrEnemies++;
                if(NbrEnemies == 1)
                {
                    GetWorldTimerManager().SetTimer(TimerHandle_Damaged, this, &ATrapWeakZone::WeakenedEnemy, TimerDamageWhenWeakeningEnemies, true);
                }
            }
        }
    }
}

void ATrapWeakZone::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    if (AEnnemy* OtherEnemy = Cast<AEnnemy>(OtherActor))
    {
        if(OtherComp->GetName() == FString("CollisionCylinder"))
        {
            OtherEnemy->DeleteWeakEffectSource();
            NbrEnemies--;
            if (NbrEnemies == 0)
                GetWorldTimerManager().ClearTimer(TimerHandle_Damaged);
        }
    }
}

float ATrapWeakZone::Damaged()
{
    for (int i = 0; i < NbrEnemies; i++)
    {
        Super::Damaged();
    }
    return 1.f;
}