// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDefender/ShopItem.h"

#include "ShopItemAttackSpeed.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENDER_API UShopItemAttackSpeed : public UShopItem
{
	GENERATED_BODY()

public:
	UShopItemAttackSpeed();

protected:
	virtual bool ActivateItem() override;

private:
    /**
	 * @brief Player attack speed bonus. Player attack speed is multiplied by this number.
	 */
	UPROPERTY(EditAnywhere, Category = Shop)
	float	SpeedRatioBonus;
	
};
