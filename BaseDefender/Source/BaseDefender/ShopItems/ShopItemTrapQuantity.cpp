// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopItemTrapQuantity.h"

#include "BaseDefender/BaseDefenderCharacter.h"

UShopItemTrapQuantity::UShopItemTrapQuantity()
{
    Price = 300;
    TrapQuantityBonus = 1;
    ItemInflation = 1.25;

    ItemName = FName("Trap Quantity");

	MaxItemQuantity = 7;
	MaxPrice = 0;
	MaxPriceOverridesMaxQuantity = false;
}

bool UShopItemTrapQuantity::ActivateItem()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    ABaseDefenderCharacter* Player = Cast<ABaseDefenderCharacter>(PlayerPawn);
    if(Player)
    {
        Player->SetAvailableTrap(TrapIndex, Player->GetAvailableTrap(TrapIndex) + TrapQuantityBonus);
        return true;
    }

    return false;
}

void UShopItemTrapQuantity::SetTrapIndex(const int Index)
{
    TrapIndex = Index;
    SetItemDescription();
}

void UShopItemTrapQuantity::SetItemDescription()
{
    ABaseDefenderCharacter* Player = Cast<ABaseDefenderCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
    if (Player)
        Description = FString("Increases the amount of ") + Player->GetTrapName(TrapIndex) + FString(" traps by ") + FString::FromInt(TrapQuantityBonus) + FString(".");
}