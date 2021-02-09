// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop.h"

#include "ShopItem.h"
#include "Components/StaticMeshComponent.h"
#include "ShopItems/ShopItemAttackDamage.h"
#include "ShopItems/ShopItemAttackSpeed.h"
#include "ShopItems/ShopItemTrapQuantity.h"
#include "BaseDefender/BaseDefenderCharacter.h"

// Sets default values
AShop::AShop()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMesh"));
	Mesh->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'")).Object);
	RootComponent = Mesh;

	Items.Empty();
	Items.Reserve(7);

	Items.Add(CreateDefaultSubobject<UShopItemFortressHealth>(FName("FortressHealthBonus")));
	Items.Add(CreateDefaultSubobject<UShopItemAttackDamage>(FName("AttackDamageBonus")));
	Items.Add(CreateDefaultSubobject<UShopItemAttackSpeed>(FName("AttackSpeedBonus")));
	Items.Add(CreateDefaultSubobject<UShopItemTrapQuantity>(FName("DamageZoneQuantityBonus")));
	Items.Add(CreateDefaultSubobject<UShopItemTrapQuantity>(FName("TurretQuantityBonus")));
	Items.Add(CreateDefaultSubobject<UShopItemTrapQuantity>(FName("WeakZoneQuantityBonus")));
	Items.Add(CreateDefaultSubobject<UShopItemTrapQuantity>(FName("SlowDownQuantityBonus")));

}

void AShop::BeginPlay()
{
	Super::BeginPlay();

	for(int i = 0; i < 4; i++)
	{
		UShopItemTrapQuantity* ItemTrap = Cast<UShopItemTrapQuantity>(Items[i + 3]);
		if(ItemTrap)
		{
			ItemTrap->SetTrapIndex(i);
		}
	}
}

void AShop::Tick(float DeltaSeconds)
{
	bCanOpen = false;
	
	TArray<AActor*> OverlappedActors;
	GetOverlappingActors(OverlappedActors);

	for (AActor* Actor : OverlappedActors)
	{
		ABaseDefenderCharacter* Player = Cast<ABaseDefenderCharacter>(Actor);
		if (Player)
		{
			bCanOpen = true;
			return;
		}
	}
}


void AShop::GetItems(TArray<UShopItem*>& ItemsArray) const
{
	ItemsArray = Items;
}

int AShop::GetItemsNumber() const
{
	return Items.Num();
}
