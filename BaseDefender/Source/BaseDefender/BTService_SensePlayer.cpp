// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SensePlayer.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BaseDefender/BaseDefenderCharacter.h"

void UBTService_SensePlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* MyController = OwnerComp.GetAIOwner();
	UAIPerceptionComponent* AIPerception = nullptr;
	
	if (MyController != nullptr)
	{
		AIPerception = MyController->FindComponentByClass<UAIPerceptionComponent>();
	}

	if (AIPerception == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Perception couldn't be initialized"));
		return;
	}

	TArray<AActor*> PerceivedActors;
	AIPerception->GetCurrentlyPerceivedActors(PerceivedSense, PerceivedActors);

	ABaseDefenderCharacter* Player = Cast<ABaseDefenderCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (PerceivedActors.Num() == 0 || !Player || Player->IsDead()) // Didn't find player
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
	else // Found player
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PerceivedActors[0]);
	}

}
