// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseDefender_HUD.generated.h"

/**
 * 
 */
UCLASS()
class BASEDEFENDER_API ABaseDefender_HUD : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* CrosshairTexture;

public:

	virtual void DrawHUD() override;
};
