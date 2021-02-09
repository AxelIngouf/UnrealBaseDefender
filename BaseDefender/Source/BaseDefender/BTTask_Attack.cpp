// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include <BaseDefender\Ennemy.h>
#include "AIController.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AEnnemy* Enemy = Cast<AEnnemy>(OwnerComp.GetAIOwner()->GetPawn());

    if (Enemy != nullptr)
    {

        Enemy->Attack();
    }

    return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::AbortTask(OwnerComp, NodeMemory);

    AEnnemy* Enemy = Cast<AEnnemy>(OwnerComp.GetAIOwner()->GetPawn());

    if (Enemy != nullptr)
    {
        Enemy->StopAttack();
    }

    return EBTNodeResult::Aborted;
}
