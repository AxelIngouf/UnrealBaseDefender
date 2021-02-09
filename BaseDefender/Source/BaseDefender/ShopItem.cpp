// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopItem.h"

#include "BaseDefenderCharacter.h"

// Sets default values
UShopItem::UShopItem()
{
    ItemInflation = 1.2;
}

bool UShopItem::ItemClicked()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    ABaseDefenderCharacter* Player = Cast<ABaseDefenderCharacter>(PlayerPawn);

    if (!Player)
        return false;

    if (Player->GetCoins() < Price)
        return false;

	// Check if the player has less than the max amount of this item
	if(MaxItemQuantity != 0)
		if(ItemQuantityPurchased >= MaxItemQuantity)
	        return false;

	// Check if the price is below the max price
	if(MaxPrice != 0)
		if(MaxPriceOverridesMaxQuantity && Price >= MaxPrice)
	        return false;

    if(ActivateItem())
    {
        Player->SpendCoins(Price);
        ItemQuantityPurchased++;
    	
        Price *= ItemInflation;
    	
        if(MaxPrice != 0)
    		if(Price > MaxPrice)
	            Price = MaxPrice;
        return true;
    }
    return false;
}

