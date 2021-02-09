// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shop.generated.h"

UCLASS()
class BASEDEFENDER_API AShop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShop();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = Shop)
	void	GetItems(TArray<class UShopItem*>& ItemsArray) const;

	UFUNCTION(BlueprintCallable, Category = Shop)
	int		GetItemsNumber() const;

	virtual void Tick(float DeltaSeconds) override;
	
protected:

	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* Mesh;

    /**
	 * @brief List of items sold in the Shop.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shop)
	TArray<UShopItem*> Items;

	UPROPERTY(BlueprintReadOnly, Category = Shop)
	bool	bOpened;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Shop)
	bool	bCanOpen = false;
};
