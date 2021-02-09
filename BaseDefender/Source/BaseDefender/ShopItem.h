// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "BaseDefenderGameMode.h"
#include "Fortress.h"
#include "HealthComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

#include "ShopItem.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BASEDEFENDER_API UShopItem : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	explicit UShopItem();

	UFUNCTION(BlueprintCallable, Category = Shop)
	bool	ItemClicked();

	UFUNCTION(BlueprintCallable, Category = Shop)
	int		GetPrice() const { return Price; }

	UFUNCTION(BlueprintCallable, Category = Shop)
	void	SetPrice(const int NewPrice) { Price = NewPrice; }

	UFUNCTION(BlueprintCallable, Category = Shop)
	FName	GetItemName() const { return ItemName; }

protected:

	virtual bool ActivateItem() { return false; }

    /**
	 * @brief Item name.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shop)
	FName		ItemName;

    /**
	 * @brief Item price.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shop)
	int			Price;

    /**
	 * @brief Item description.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shop)
	FString		Description;

    /**
	 * @brief Item inflation. The Item price will be multiplied by this number every time it's bought.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shop)
	float		ItemInflation;

	 /**
	 * @brief Maximum quantity of the item the player can buy. 0 for unlimited. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shop)
	int			MaxItemQuantity;

	 /**
	 * @brief Maximum item price. 0 for unlimited. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shop)
	int			MaxPrice;

	 /**
	 * @brief When true, if the item reaches the max price before the max quantity, it will stop the player from buying.
	 *			When false, the price of the item will cap at MaxPrice but the player will still be able to buy items. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shop)
	bool		MaxPriceOverridesMaxQuantity;


	/**
	 * @brief Number of this item the player has purchased. 
	 */
	int ItemQuantityPurchased = 0;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BASEDEFENDER_API UShopItemFortressHealth : public UShopItem
{
	GENERATED_BODY()

public:
	explicit UShopItemFortressHealth()
	{
		Price = 200;
		HealthBonus = 2000;
		ItemInflation = 1.5;
		MaxItemQuantity = 5;
		MaxPrice = 0;

		ItemName = FName("Fortress Repair");
		Description = FString("Restores 10% health points to the Fortress.");
	}

protected:

	virtual bool ActivateItem() override
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(this, AFortress::StaticClass(), Actors);

		for(AActor* Actor : Actors)
		{
			AFortress* Fortress = Cast<AFortress>(Actor);
			if(Fortress)
			{
			    if(Fortress->GetHealthComponent()->HealthPercent < 1.f && Fortress->GetHealthComponent()->HealthPercent > 0.f)
			    {
					ABaseDefenderGameMode* GameMode = Cast<ABaseDefenderGameMode>(GetWorld()->GetAuthGameMode());
					if(GameMode)
					{
						GameMode->ApplyFortressDamage(- HealthBonus); // Notify negative damages to gamemode, same as giving health back
						Fortress->GetHealthComponent()->AddHealthPoints(HealthBonus);
						return true;
					}
					
			    }
			}
		}

		return false;
	}

private:

    /**
	 * @brief Health points given back to the fortress.
	 */
	UPROPERTY(EditAnywhere, Category = Shop)
	float	HealthBonus;
};