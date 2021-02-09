// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDefender/ShopItem.h"

#include "ShopItemTrapQuantity.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BASEDEFENDER_API UShopItemTrapQuantity : public UShopItem
{
	GENERATED_BODY()

public:
	UShopItemTrapQuantity();

	virtual bool ActivateItem() override;

    /**
	 * @brief Sets the trap index to increase when the bonus item is activated.
	 * @param Index - Trap Index
	 */
	void	SetTrapIndex(int Index);

private:

	void	SetItemDescription();

    /**
	 * @brief Trap quantity bonus. Player will be given this quantity of every trap.
	 */
	UPROPERTY(EditAnywhere, Category = Shop)
	int	TrapQuantityBonus;

	int	TrapIndex;
};
