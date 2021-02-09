// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopItemAttackSpeed.h"

#include "BaseDefender/BaseDefenderCharacter.h"

UShopItemAttackSpeed::UShopItemAttackSpeed()
{
    Price = 2000;
    SpeedRatioBonus = 1.89;
    ItemInflation = 2;

    ItemName = FName("Attack Speed");
    Description = FString("Divides time between each attacks by 1.89.");

	MaxItemQuantity = 1;
	MaxPrice = 0;
	MaxPriceOverridesMaxQuantity = false;
}

bool UShopItemAttackSpeed::ActivateItem()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    ABaseDefenderCharacter* Player = Cast<ABaseDefenderCharacter>(PlayerPawn);
    if (Player)
    {
        Player->SetTimeBetweenAttacks(Player->GetTimeBetweenAttacks() / SpeedRatioBonus);
        return true;
    }

    return false;
}
