// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopItemAttackDamage.h"

#include "BaseDefender/BaseDefenderCharacter.h"

UShopItemAttackDamage::UShopItemAttackDamage()
{
    Price = 2000;
    ItemInflation = 2;
    AttackDamageRatio = 2;

    ItemName = FName("Attack Damage");
    Description = FString("Doubles weapon damage.");

	MaxItemQuantity = 1;
	MaxPrice = 0;
	MaxPriceOverridesMaxQuantity = false;
}

bool UShopItemAttackDamage::ActivateItem()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    ABaseDefenderCharacter* Player = Cast<ABaseDefenderCharacter>(PlayerPawn);
    if (Player)
    {
        Player->SetProjectileDamage(Player->GetProjectileDamage() * AttackDamageRatio);
        return true;
    }

    return false;
}
