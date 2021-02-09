// Fill out your copyright notice in the Description page of Project Settings.


#include "EnnemyAnimation.h"
#include "BaseDefender\Ennemy.h"
#include "BaseDefender\HealthComponent.h"

void UEnnemyAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AEnnemy* Enemy = Cast<AEnnemy>(this->GetOwningActor());

	if (Enemy != nullptr)
	{
		const float MoveSpeed = Enemy->GetVelocity().Size2D();

		IsWalking = !(FMath::IsNearlyZero(MoveSpeed));

		IsDead = Enemy->GetHealthComponent()->GetHealthPoints() <= 0;

		WalkBlendRatio = (MoveSpeed / 500) * 100.f;

		AttackRate = Enemy->GetAttackSpeed();
	}
}
