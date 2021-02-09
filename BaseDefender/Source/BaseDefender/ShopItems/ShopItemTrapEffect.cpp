// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopItemTrapEffect.h"

#include "BaseDefender/BaseDefenderCharacter.h"

UShopItemTrapEffect::UShopItemTrapEffect()
{
    Price = 500;
    RatioBonusToAdd = 0.2;

    ItemName = FName("Trap Effect Multiplier");
    Description = FString(TEXT("Improve traps effect by 0.2"));
	
	MaxItemQuantity = 0;
	MaxPrice = 0;
	MaxPriceOverridesMaxQuantity = false;
}

bool UShopItemTrapEffect::ActivateItem()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    ABaseDefenderCharacter* Player = Cast<ABaseDefenderCharacter>(PlayerPawn);
    if (Player)
    {
        Player->SetTrapEffectRatio(Player->GetTrapEffectRatio() + RatioBonusToAdd);
        return true;
    }

    return false;
}
