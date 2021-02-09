// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopItemTrapSize.h"
#include "BaseDefender/BaseDefenderCharacter.h"

UShopItemTrapSize::UShopItemTrapSize()
{
    Price = 100;
    RatioBonus = 0.10;

    ItemName = FName("Trap Size");
    Description = FString("Improve trap size by 0.10 of their initial size.");

	MaxItemQuantity = 0;
	MaxPrice = 0;
	MaxPriceOverridesMaxQuantity = false;
}

bool UShopItemTrapSize::ActivateItem()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    ABaseDefenderCharacter* Player = Cast<ABaseDefenderCharacter>(PlayerPawn);

    if(Player)
    {
        Player->AddTrapSize(RatioBonus);
        return true;
    }

    return false;
}
