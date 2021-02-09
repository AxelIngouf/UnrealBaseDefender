// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDefender/ShopItem.h"

#include "ShopItemTrapSize.generated.h"


UCLASS()
class BASEDEFENDER_API UShopItemTrapSize : public UShopItem
{
	GENERATED_BODY()

public:
	UShopItemTrapSize();

protected:
	virtual bool ActivateItem() override;

private:
    /**
	 * @brief Trap size bonus. Trap collision component size will be multiplied by this number.
	 */
	UPROPERTY(EditAnywhere, Category=Shop)
	float	RatioBonus;
};
