// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDefender/ShopItem.h"

#include "ShopItemAttackDamage.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENDER_API UShopItemAttackDamage : public UShopItem
{
	GENERATED_BODY()

public:
	UShopItemAttackDamage();

protected:
	virtual bool ActivateItem() override;

private:
    /**
	 * @brief Player attacks damage bonus. Player attacks damage is multiplied by this number.
	 */
	UPROPERTY(EditAnywhere, Category = Shop)
	float	AttackDamageRatio;
};
