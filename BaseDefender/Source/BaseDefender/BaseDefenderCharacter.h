// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "BaseDefenderCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDamaged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTrapIndexChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTrapAmountChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTrapPlaced, FVector, TrapPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShopOpened, class AShop*, Shop);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHoldingTrap, bool, Holding);

UCLASS(config=Game)
class ABaseDefenderCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
    explicit ABaseDefenderCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;

protected:

	virtual void Jump() override;

	/** Called for forwards/backward input */
	void	MoveForward(float Value);

	/** Called for side to side input */
	void	MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void	TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void	LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void	TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void	TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// APawn interface
	virtual void	SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual float	TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void	BeginPlay() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

    /**
	 * @brief Start attack, but fire is triggered when attack is stopped
	 */
	void	Attack();

    /**
	 * @brief Stop player attack, triggering Fire function if character is able to.
	 */
	void	StopAttack();

    /**
	 * @brief Fire a projectile from Character position
	 */
	void	Fire();

    /**
	 * @brief Add a certain amount of coins to current Character
	 * @param CoinsNumber of coins to add
	 */
	void	AddCoins(int CoinsNumber);

    /**
	 * @brief Spend Character coins if it's possible
	 * @param CoinsNumber of coins to spend
	 * @return true if transaction was successful, false if not
	 */
	bool	SpendCoins(int CoinsNumber);

    /**
	 * @brief Return the number of coins the player has
	 * @return Number of coins
	 */
	UFUNCTION(BlueprintCallable, Category = Coins)
	int		GetCoins() const;

    /**
	 * @brief Place a trap where the character is looking at if it's possible
	 */
	void	PlaceTrap();

	UFUNCTION(BlueprintCallable, Category = Attack)
	bool	IsAttack() const;

	UFUNCTION(BlueprintCallable, Category = Attack)
	bool	IsCooldownAttack() const { return bAttackCooldown; }

	UFUNCTION(BlueprintCallable, Category = Death)
	bool	IsDead() const { return bDead; }

	bool	IsMovingForward() const { return bMovingForward; }

    bool	IsMovingRight() const { return bMovingRight; }

	int		GetSideSpeed() const { return SideSpeed; }

	UFUNCTION(BlueprintCallable, Category = Attack)
	bool	IsHoldingTrap() const { return bHoldingTrap; }

    /**
	 * @brief Zoom camera at the center of the screen
	 */
	void	Zoom();

    /**
	 * @brief UnZoom camera, can only be triggered after Zoom method was activated
	 */
	void	UnZoom();

    /**
	 * @brief Retrieves Projectile Damages
	 * @return Projectile Damages
	 */
	int		GetProjectileDamage() const { return ProjectileDamage; }

    /**
	 * @brief Set Projectile damages
	 * @param Damage New projectile damages
	 */
	void	SetProjectileDamage(const float Damage) { ProjectileDamage = Damage; }

    /**
	 * @brief Retrieve selected trap index
	 * @return Selected trap index
	 */
	UFUNCTION(BlueprintCallable, Category = Trap)
	int		GetTrapIndex() const;

    /**
	 * @brief Returns trap name for given index.
	 * @param TrapIndex - Trap Index
	 * @return Trap name for given index.
	 */
	FString	GetTrapName(int TrapIndex) const;

    /**
	 * @brief Retrieve character Health Component
	 * @return Character Health Component
	 */
	UFUNCTION(BlueprintCallable, Category = Health)
	class UHealthComponent* GetHealthComponent() const;

    /**
	 * @brief Increment number of available traps for specified index with specified amount
	 * @param TrapIndex Index of trap to increment
	 * @param TrapAmount Amount of available traps to add
	 */
	void	IncrementAvailableTrap(int TrapIndex, int TrapAmount);

    /**
	 * @brief Retrieves Trap effect ratio, used to multiply the effect of traps (Only damage traps for now)
	 * @return 
	 */
	float	GetTrapEffectRatio() const { return TrapEffectRatio; }

    /**
	 * @brief Changes the Traps effect ratio, used to multiply the effect of traps (Only damage traps for now)
	 * @param EffectRatio 
	 */
	void	SetTrapEffectRatio(const float EffectRatio) { TrapEffectRatio = EffectRatio; }

    /**
	 * @brief Retrieve number of available traps for specified index
	 * @param TrapIndex Index of trap to check
	 * @return Available number of specified trap
	 */
	UFUNCTION(BlueprintCallable, Category = Trap)
	int		GetAvailableTrap(const int TrapIndex) const { if (TrapIndex >= 0 && TrapIndex < TrapTypes.Num())return AvailableTraps[TrapIndex]; return 0; }

    /**
	 * @brief Retrieves maximum time between each default attacks
	 * @return float Time in seconds between each attacks
	 */
	float	GetTimeBetweenAttacks() const { return TimeBetweenAttacks; }

    /**
	 * @brief Sets a new maximum time between each default attacks
	 * @param Time float Seconds between each attack
	 */
	void	SetTimeBetweenAttacks(const float Time) { TimeBetweenAttacks = Time; }

    /**
	 * @brief Change quantity of available trap for a given trap index
	 * @param TrapIndex Trap index to modify
	 * @param TrapQuantity New trap quantity
	 */
	void	SetAvailableTrap(const int TrapIndex, const int TrapQuantity)
	{
	    if (TrapIndex >= 0 && TrapIndex < TrapTypes.Num())
	    {
			AvailableTraps[TrapIndex] = TrapQuantity;
			OnTrapAmountChanged.Broadcast();
	    }
	}

    /**
	 * @brief Allows to resize the traps
	 * @param TrapSizeToAdd float added to the current Traps size
	 */
	void AddTrapSize(float TrapSizeToAdd);

    /**
	 * @brief Bound to OpenShop Input, calls OnShopOpened event if valid.
	 */
	void	OpenShop();

	/**
	 * @brief Allows the player to stop taking damage when true
	 * @param Value Should the player be invulnerable or not
	 */
	UFUNCTION(BlueprintCallable, Category = Damage)
	void	SetPlayerGodmode(const bool Value) { Godmode = Value; }
	

	UPROPERTY(BlueprintAssignable, Category = Trap)
	FTrapIndexChanged	OnTrapIndexChanged;

	UPROPERTY(BlueprintAssignable, Category = Trap)
	FTrapAmountChanged	OnTrapAmountChanged;

	UPROPERTY(BlueprintAssignable, Category = Shop)
	FShopOpened			OnShopOpened;

	UPROPERTY(BlueprintAssignable, Category = Damage)
	FPlayerDamaged		OnDamaged;

	UPROPERTY(BlueprintAssignable, Category = Trap)
	FHoldingTrap		OnHoldingTrap;

	UPROPERTY(BlueprintAssignable, Category = Trap)
	FTrapPlaced			OnTrapPlaced;
		
protected:

    /**
     * @brief Scroll between trap types if the character is holding them
     * @param ScrollValue 1 or -1, index incrementation between traps
     */
    void	ScrollTraps(const float ScrollValue);

    /**
	 * @brief Allows selection of specific trap with a given index. Bound to ChooseTrap inputs (1, 2, 3, 4)
	 * @param TrapIndex Chosen trap index
	 */
	void	ChooseTrap(const float TrapIndex);

    /**
	 * @brief Load preview trap on world if the Character is holding a trap
	 */
	void	LoadPlaceholderTrap();

    /**
	 * @brief Move loaded trap dynamically, following character eyes
	 */
	void	MovePlaceholderTrap();

    /**
	 * @brief Checks if placeholder trap is currently overlapping an Anti Trap component.
	 * @return True if the placeholder trap is overlapping an Anti Trap component, false if not.
	 */
	bool	IsPlaceholderTrapOverlappingAntiTrapZone(TWeakObjectPtr<UPrimitiveComponent> OverlappedComponent) const;

    /**
	 * @brief Refresh Attack cooldown, allowing the character to fire again
	 */
	void	DeleteAttackCooldown();

    /**
	 * @brief Current projectile type.
	 */
	UPROPERTY(EditAnywhere, Category = Attack)
	TSubclassOf<class AArrow> ProjectileType = nullptr;

    /**
	 * @brief Every type of traps and their order in the player hands.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Attack)
	TArray<TSubclassOf<class ATrap>> TrapTypes;

    /**
	 * @brief Number of available traps.
	 * 1 - DamageZone
	 * 2 - DamageLine
	 * 3 - WeakZone
	 * 4 - SlowDown
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TArray<int>	AvailableTraps;

    /**
	 * @brief Player Health component. Containing health and maximum health values.
	 */
	UPROPERTY(EditAnywhere, Category = Health)
	UHealthComponent* HealthComp = nullptr;
	bool	bDead;
	bool	bMovingForward; // Used to determine Forward and Backward movement, for animation purposes and stop sprint if movement is backward.
	bool	bMovingRight; // Used to determine side direction for animation purposes
	int		SideSpeed; // Current side speed, used for animation
	bool	Godmode; // Is the player invulnerable

    /**
	 * @brief Number of coins the player currently has
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Coins)
	int		Coins;

    /**
	 * @brief Cooldown time between each player attacks.
	 */
	UPROPERTY(EditAnywhere, Category = Attack)
	float	TimeBetweenAttacks;
    /**
	 * @brief Player projectile damage.
	 */
	UPROPERTY(EditAnywhere, Category = Attack)
	float	ProjectileDamage;
	bool	bAttackCooldown; // Attack is cooling down
	bool	bAttacking; // Is currently attacking

    /**
	 * @brief Maximum distance at which the player can place traps
	 */
	UPROPERTY(EditAnywhere, Category = Attack)
	float		TrapPlacingDistance;
	float		TrapSize = 1.f; // Used for Trap Size ShopItem bonus
	float		TrapEffectRatio = 1.f; // Used for Trap Effect ShopItem bonus
	int			SelectedTrapIndex;
	ATrap*		PlaceholderTrap = nullptr;
	FRotator	TrapRotation;

	class ABaseDefenderGameMode* GameModeReference;

	bool	bHoldingWeapon;
	bool	bHoldingTrap;

	bool	Zooming = false;

	FTimerHandle TimerHandle_CoolDownAttack;
	FTimerHandle TimerHandle_PlaceHolderTrapFollowsCamera;
};
