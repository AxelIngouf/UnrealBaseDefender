// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnnemyAnimation.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENDER_API UEnnemyAnimation : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ennemy)
	bool	IsWalking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ennemy)
	bool	IsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ennemy)
	bool	IsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ennemy, meta = (ClampMin = 0.0, ClampMax = 100.0))
	float	WalkBlendRatio = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ennemy, meta = (ClampMin = 0.0, ClampMax = 2.0))
	float	AttackRate = 1.f;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
};
