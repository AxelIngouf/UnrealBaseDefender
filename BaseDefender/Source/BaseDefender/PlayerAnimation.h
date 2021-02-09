// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimation.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENDER_API UPlayerAnimation : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	bool	IsWalking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	bool	IsMovingForward = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	bool	IsMovingRight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	bool	IsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	bool	IsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	bool	IsJumping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player, meta = (ClampMin = -100.0, ClampMax = 100.0))
	float	WalkBlendRatio = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player, meta = (ClampMin = -100.0, ClampMax = 100.0))
	float	SideWalkBlendRatio = 0.f;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

};
