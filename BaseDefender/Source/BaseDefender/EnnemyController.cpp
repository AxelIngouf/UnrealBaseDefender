// Fill out your copyright notice in the Description page of Project Settings.


#include "EnnemyController.h"
#include "Kismet\GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "BaseDefender/Ennemy.h"
#include "BaseDefender/BaseDefenderCharacter.h"

AEnnemyController::AEnnemyController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(FName("AIPerception"));
}

void AEnnemyController::BeginPlay()
{
	Super::BeginPlay();

	if (EnnemyBehavior != nullptr)
	{
		bool bSuccess = RunBehaviorTree(EnnemyBehavior);
		if (!bSuccess)
		{
			UE_LOG(LogTemp, Warning, TEXT("Error while running behavior tree"));
			return;
		}
	}

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetTag, ActorsWithTag);

	if (ActorsWithTag.Num() == 0)
	{
		return;
	}

	AActor* AITarget = ActorsWithTag[0];
	if (AITarget != nullptr)
	{
		GetBlackboardComponent()->SetValueAsObject(TargetActorKey, AITarget);
	}


}

ETeamAttitude::Type AEnnemyController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (Other.IsA<AEnnemy>())
	{
		return ETeamAttitude::Friendly;
	}
	if (Other.IsA<ABaseDefenderCharacter>())
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Neutral;
}
