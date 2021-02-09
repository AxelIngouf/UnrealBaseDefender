// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnnemyController.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENDER_API AEnnemyController : public AAIController
{
	GENERATED_BODY()

public:

	AEnnemyController();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = EnnemyAI)
	class UBehaviorTree* EnnemyBehavior = nullptr;

	/* Tag of the actor you want to target */
	UPROPERTY(EditDefaultsOnly, Category = EnnemyAI)
	FName	TargetTag = "AITarget";

	UPROPERTY(EditDefaultsOnly, Category = EnnemyAI)
	FName	TargetActorKey = "TargetActor";

	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

};
