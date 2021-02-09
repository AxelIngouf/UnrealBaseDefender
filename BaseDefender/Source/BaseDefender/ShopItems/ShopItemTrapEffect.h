// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDefender/ShopItem.h"

#include "ShopItemTrapEffect.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENDER_API UShopItemTrapEffect : public UShopItem
{
	GENERATED_BODY()

public:
	UShopItemTrapEffect();

protected:
	virtual bool ActivateItem() override;

private:
    /**
	 * @brief Trap effect bonus. This number will be added to the current Trap effect multiplier of the player.
	 * The Trap effect multiplier is the used to multiply trap effects (damaged, slow down, weakness) every time they are placed.
	 */
	UPROPERTY(EditAnywhere, Category=Shop)
	float	RatioBonusToAdd;
};
