// Fill out your copyright notice in the Description page of Project Settings.


#include "Ennemy.h"


#include "EnnemyController.h"
#include "LifePotion.h"
#include "TrapSlowDown.h"
#include "Perception/AIPerceptionComponent.h"
#include "BaseDefender\Spawner.h"
#include "BaseDefender\HealthComponent.h"
#include "BaseDefender\Coin.h"
#include "BaseDefender\EnnemyAnimation.h"
#include "BaseDefender\TrapWeakZone.h"
#include "BaseDefender\BaseDefenderCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnnemy::AEnnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MaxSpeed = 400.f;
	TrapSlowDownSpeed = 150.f;
	TrapWeakEffectRatio = 2.f;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;

	Health = this->CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	bAttacking = false;
	Power = 92;

	bSlowedDown = false;

	ChanceCoinDrop = 2;
	MaxCoinsOnDeath = 10;
	ChanceHealthDrop = 10;

	LifePotionType = ALifePotion::StaticClass();
	CoinType = ACoin::StaticClass();
}

float AEnnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float TotalDamages = DamageAmount;
	if(WeakEffectSource != nullptr)
	{
		TotalDamages *= TrapWeakEffectRatio;
	}

	Health->ApplyComponentDamage(TotalDamages);

	if(Health->GetHealthPoints() <= 0.f)
	{
		TotalDamages += Health->GetHealthPoints();
		Die();
	}

	// Notify TrapWeakZone if the enemy is in one
	if (WeakEffectSource != nullptr)
	{
		WeakEffectSource->WeakenedEnemy();
	}

	OnDamaged.Broadcast();

	return TotalDamages;
}

void AEnnemy::FellOutOfWorld(const UDamageType& dmgType)
{
	if (SpawnOrigin != nullptr)
	{
		SpawnOrigin->DeadEnemy(WaveId, SubWaveId);
	}
	OnDeath.Broadcast();
	Destroy();
}

void AEnnemy::Die()
{
	if(SpawnOrigin != nullptr)
	{
		SpawnOrigin->DeadEnemy(WaveId, SubWaveId);
	}

	Health->HealthPoints = 0.f;
	GetController()->UnPossess();
	SetActorEnableCollision(false);
	GetCharacterMovement()->StopMovementImmediately();

	Loot();

	OnDeath.Broadcast();

	GetWorldTimerManager().SetTimer(TimerHandle_DestroyAfterDeath, this, &AEnnemy::AutoDestruct, TimerDestroyAfterDeath);
}

void AEnnemy::Loot() const
{
	if(UKismetMathLibrary::RandomInteger(ChanceCoinDrop) == 0)
	{
		const int CoinsNumber = UKismetMathLibrary::RandomIntegerInRange(MaxCoinsOnDeath / 2, MaxCoinsOnDeath);
		for(int i = 0; i < CoinsNumber; i++)
		{
			const FVector Position(UKismetMathLibrary::RandomInteger(100), UKismetMathLibrary::RandomInteger(100), 30);
			GetWorld()->SpawnActor<ACoin>(CoinType, GetActorLocation() + Position, FRotator(0.f, UKismetMathLibrary::RandomInteger(90), 0.f));
		}
	}

	if(UKismetMathLibrary::RandomInteger(ChanceHealthDrop) == 0)
	{
		GetWorld()->SpawnActor<ALifePotion>(LifePotionType, GetActorLocation(), FRotator::ZeroRotator);
	}
	
}

void AEnnemy::Attack()
{
	UEnnemyAnimation* EnemyAnim = Cast<UEnnemyAnimation>(GetMesh()->GetAnimInstance());

	if (EnemyAnim != nullptr)
	{
		EnemyAnim->IsAttacking = true;
	}
}

void AEnnemy::StopAttack()
{
	UEnnemyAnimation* EnemyAnim = Cast<UEnnemyAnimation>(GetMesh()->GetAnimInstance());

	if (EnemyAnim != nullptr)
	{
		EnemyAnim->IsAttacking = false;
	}
}

bool AEnnemy::IsAttack() const
{
	return bAttacking;
}

UHealthComponent* AEnnemy::GetHealthComponent() const
{
	return Health;
}

bool AEnnemy::IsSlowedDown() const
{
	return bSlowedDown;
}

void AEnnemy::SetSlowDown(const bool Slow)
{
	if(Slow != bSlowedDown)
	{
		if(Slow)
		{
			GetCharacterMovement()->MaxWalkSpeed = TrapSlowDownSpeed;
		}
		else
		{
			TArray<AActor*> OverlappedEnemyActor;
			GetOverlappingActors(OverlappedEnemyActor);
			bool InSlowTrap = false;

			for (AActor* OtherActor : OverlappedEnemyActor)
			{
				if (Cast<ATrapSlowDown>(OtherActor))
				{
					if (InSlowTrap)
					{
						// Checks if enemy is in another SlowDown trap than the one being destroyed, and abort if he is
						return;
					}
					InSlowTrap = true;
				}
			}
			GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
		}
		this->bSlowedDown = Slow;
	}
	
}

void AEnnemy::DeleteWeakEffectSource()
{
	TArray<AActor*> OverlappedActors;
	GetOverlappingActors(OverlappedActors, ATrapWeakZone::StaticClass());

	for(AActor* Actor : OverlappedActors)
	{
		ATrapWeakZone* WeakZone = Cast<ATrapWeakZone>(Actor);
		if(WeakZone)
		{
		    if(WeakZone != WeakEffectSource)
		    {
				WeakEffectSource = WeakZone;
				return;
		    }
		}
	}
	WeakEffectSource = nullptr;
}

void AEnnemy::SetSpawnOrigin(ASpawner* Spawn, const int Wave, const int SubWave)
{
	this->SpawnOrigin = Spawn;
	this->WaveId = Wave;
	this->SubWaveId = SubWave;
}

void AEnnemy::RotateTowardPlayer()
{
	AEnnemyController* EnemyController = Cast<AEnnemyController>(GetController());
	if (EnemyController != nullptr)
	{
		TArray<AActor*> PerceivedHostileActors;
		EnemyController->PerceptionComponent->GetPerceivedHostileActors(PerceivedHostileActors);

		if (PerceivedHostileActors.Num() > 0)
		{
			const FRotator RotationTowardHostile = (PerceivedHostileActors[0]->GetActorLocation() - GetActorLocation()).GetSafeNormal2D().Rotation();
			SetActorRotation(RotationTowardHostile);
		}
	}
}

void AEnnemy::AutoDestruct()
{
	Destroy();
}

