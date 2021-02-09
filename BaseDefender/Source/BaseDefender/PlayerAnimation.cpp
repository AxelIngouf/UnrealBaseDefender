// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimation.h"

#include "PlayerMovementComponent.h"
#include "BaseDefender\BaseDefenderCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseDefender\HealthComponent.h"

void UPlayerAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ABaseDefenderCharacter* Player = Cast<ABaseDefenderCharacter>(TryGetPawnOwner());

	if (Player != nullptr)
	{
		const float MoveSpeed = Player->GetCharacterMovement()->Velocity.Size2D();

		const float FallSpeed = Player->GetCharacterMovement()->Velocity.Z;

		IsWalking = !(FMath::IsNearlyZero(MoveSpeed));

		IsMovingForward = Player->IsMovingForward();

		IsMovingRight = Player->IsMovingRight();

		IsJumping = !(FMath::IsNearlyZero(FallSpeed));

		IsDead = Player->GetHealthComponent()->GetHealthPoints() <= 0;

		IsAttacking = Player->IsAttack();

		UPlayerMovementComponent* PlayerMovement = Cast<UPlayerMovementComponent>(Player->GetMovementComponent());

		if (IsWalking)
		{
			if (PlayerMovement && PlayerMovement->IsSprinting())
			{
				WalkBlendRatio = 100.f;
			}
			else
			{
				const float MaxSpeed = Player->GetCharacterMovement()->MaxWalkSpeed;
				WalkBlendRatio = (MoveSpeed / (MaxSpeed * 2)) * 100.f;
				if (!Player->IsMovingForward())
					WalkBlendRatio *= -1;
				SideWalkBlendRatio = Player->GetSideSpeed() * WalkBlendRatio;
			}
		}


	}
}
