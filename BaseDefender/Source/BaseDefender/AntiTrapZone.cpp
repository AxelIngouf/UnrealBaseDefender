// Fill out your copyright notice in the Description page of Project Settings.


#include "AntiTrapZone.h"
#include "AntiTrapComponent.h"

// Sets default values
AAntiTrapZone::AAntiTrapZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    AntiTrapComponent = CreateDefaultSubobject<UAntiTrapComponent>("AntiTrapComponent");
    RootComponent = AntiTrapComponent;
}

