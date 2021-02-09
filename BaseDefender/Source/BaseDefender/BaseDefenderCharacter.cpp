// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseDefenderCharacter.h"



#include "AntiTrapComponent.h"
#include "BaseDefenderGameMode.h"
#include "BaseDefender/Arrow.h"
#include "BaseDefender/HealthComponent.h"
#include "BaseDefender/PlayerMovementComponent.h"
#include "BaseDefender/Shop.h"
#include "BaseDefender/Trap.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#define PLACEHOLDER_TRAP_REFRESH_RATE 0.05

//////////////////////////////////////////////////////////////////////////
// ABaseDefenderCharacter

ABaseDefenderCharacter::ABaseDefenderCharacter(const class FObjectInitializer& ObjectInitializer)
 : Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(ACharacter::CharacterMovementComponentName))
{	
	PrimaryActorTick.bCanEverTick = false;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	HealthComp = this->CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	bDead = false;
	ProjectileDamage = 250.f;
	bAttacking = false;
	bAttackCooldown = false;
	TimeBetweenAttacks = 0.5f;
	TrapPlacingDistance = 1500.f;
	Coins = 0.f;
	bHoldingTrap = false;
	TrapRotation = FRotator(0, 0, 0);

	ProjectileType = AArrow::StaticClass();

	SelectedTrapIndex = -1;

	AvailableTraps.Reserve(4);
	AvailableTraps.Add(1);
	AvailableTraps.Add(1);
	AvailableTraps.Add(1);
	AvailableTraps.Add(1);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

//////////////////////////////////////////////////////////////////////////
// Input
//////////////////////////////////////////////////////////////////////////
void ABaseDefenderCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseDefenderCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABaseDefenderCharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseDefenderCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseDefenderCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ABaseDefenderCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ABaseDefenderCharacter::TouchStopped);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABaseDefenderCharacter::Attack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &ABaseDefenderCharacter::StopAttack);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, Cast<UPlayerMovementComponent>(GetCharacterMovement()), &UPlayerMovementComponent::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, Cast<UPlayerMovementComponent>(GetCharacterMovement()), &UPlayerMovementComponent::StopSprint);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ABaseDefenderCharacter::Zoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ABaseDefenderCharacter::UnZoom);

	PlayerInputComponent->BindAxis("ScrollTraps", this, &ABaseDefenderCharacter::ScrollTraps);
	PlayerInputComponent->BindAxis("ChooseTrap", this, &ABaseDefenderCharacter::ChooseTrap);

	PlayerInputComponent->BindAction("OpenShop", IE_Pressed, this, &ABaseDefenderCharacter::OpenShop);
}

float ABaseDefenderCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
	if(Godmode)
		return 0.f;
	
	DamageAmount = HealthComp->ApplyComponentDamage(DamageAmount);
	if(HealthComp->GetHealthPoints() <= 0.f)
	{
		GetCharacterMovement()->StopMovementImmediately();
		bDead = true;
		this->bUseControllerRotationYaw = false;

		// Check Lose condition on player death
		GameModeReference->CheckPlayerHealth(HealthComp->GetHealthPercent());
	}
	OnDamaged.Broadcast();


	return DamageAmount;
}

void ABaseDefenderCharacter::BeginPlay()
{
	Super::BeginPlay();

	GameModeReference = Cast<ABaseDefenderGameMode>(GetWorld()->GetAuthGameMode());

	OnTrapAmountChanged.Broadcast();
	OnTrapIndexChanged.Broadcast();
}

void ABaseDefenderCharacter::Fire()
{
	if(!bDead && !bAttackCooldown)
	{
		const FVector LookAt = FollowCamera->GetForwardVector() * 10000 + FollowCamera->GetComponentLocation();

		FHitResult HitResult;

        FVector PlayerDirectionLookAt;

		// Set projectile target point
		if (GetWorld()->LineTraceSingleByChannel(HitResult, FollowCamera->GetComponentLocation(), LookAt, ECollisionChannel::ECC_Visibility))
		{
			PlayerDirectionLookAt = HitResult.Location - GetActorLocation();
		}
		else
		{
			PlayerDirectionLookAt = LookAt - GetActorLocation();
		}

		// Start projectile spawn
		AArrow* Projectile = GetWorld()->SpawnActorDeferred<AArrow>(ProjectileType, UKismetMathLibrary::MakeTransform(GetActorLocation() + PlayerDirectionLookAt.GetSafeNormal(),
			PlayerDirectionLookAt.GetSafeNormal().Rotation(), FVector::OneVector));

		// Setting projectile parameters
		Projectile->AddActorLocalRotation(FRotator(4, 0, 0));
		Projectile->AddActorLocalTransform(FTransform(FVector(0, 15, 10)));
		Projectile->SetDamage(ProjectileDamage);

		// End projectile spawn
		Projectile->FinishSpawning(Projectile->GetTransform());

		bAttackCooldown = true;
		GetWorldTimerManager().SetTimer(TimerHandle_CoolDownAttack, this, &ABaseDefenderCharacter::DeleteAttackCooldown, TimeBetweenAttacks);
	}
}

void ABaseDefenderCharacter::AddCoins(int CoinsNumber)
{
	this->Coins += CoinsNumber;
}

bool ABaseDefenderCharacter::SpendCoins(int CoinsNumber)
{
	if(Coins > this->Coins)
	{
		return false;
	}
	this->Coins -= CoinsNumber;
	return true;
}

int ABaseDefenderCharacter::GetCoins() const
{
	return Coins;
}

void ABaseDefenderCharacter::Attack()
{
	if (bDead)
		return;

	// If the player is holding a weapon, you set a timer to increase the charge of the projectile every 0.1 seconds
	if(!bHoldingTrap)
	{
		bAttacking = true;
		Fire();
	}
	else
	{
		PlaceTrap();
	}
}

void ABaseDefenderCharacter::StopAttack()
{
	if (bDead)
		return;

	bAttacking = false;
}

void ABaseDefenderCharacter::IncrementAvailableTrap(int TrapIndex, int TrapAmount)
{
	if(TrapIndex >= 0 && TrapIndex < TrapTypes.Num())
	{
		AvailableTraps[TrapIndex] += TrapAmount;
		OnTrapAmountChanged.Broadcast();
	}
}

void ABaseDefenderCharacter::AddTrapSize(float TrapSizeToAdd)
{
	TrapSize += TrapSizeToAdd;
}

void ABaseDefenderCharacter::OpenShop()
{
	TArray<AActor*> OverlappedActors;
	GetOverlappingActors(OverlappedActors);

	for (AActor* Actor : OverlappedActors)
	{
		AShop* Shop = Cast<AShop>(Actor);
		if (Shop)
		{
			OnShopOpened.Broadcast(Shop);
			return;
		}
	}
}

void ABaseDefenderCharacter::PlaceTrap()
{
	if(SelectedTrapIndex >= 0 && SelectedTrapIndex < TrapTypes.Num() && AvailableTraps[SelectedTrapIndex] > 0)
	{
		UWorld* MyWorld = GetWorld();

		if (PlaceholderTrap)
		{
			ATrap* Trap = MyWorld->SpawnActorDeferred<ATrap>(TrapTypes[SelectedTrapIndex], UKismetMathLibrary::MakeTransform(PlaceholderTrap->GetActorLocation(), PlaceholderTrap->GetActorRotation(), FVector::OneVector));
			if(Trap)
			{
				AvailableTraps[SelectedTrapIndex] -= 1;
				Trap->EffectIntensity(TrapEffectRatio);
				Trap->ResizeTrap(TrapSize);
				Trap->SetTrapIndex(SelectedTrapIndex);
				Trap->FinishSpawning(Trap->GetTransform());
				OnTrapAmountChanged.Broadcast();
				OnTrapPlaced.Broadcast(Trap->GetActorLocation());
			}
		}
	}
}

void ABaseDefenderCharacter::ScrollTraps(const float ScrollValue)
{
	if(ScrollValue != 0.f)
	{
		SelectedTrapIndex = (SelectedTrapIndex - static_cast<int>(ScrollValue))%5;
		if(SelectedTrapIndex == -1 || SelectedTrapIndex == 4)
		{
			SelectedTrapIndex = -1;
            bHoldingTrap = false;
		}
		else
		{
			if (SelectedTrapIndex < -1)
				SelectedTrapIndex = 3;
			bHoldingTrap = true;
			bAttacking = false;
		}
		LoadPlaceholderTrap();
		OnTrapIndexChanged.Broadcast();
		OnHoldingTrap.Broadcast(bHoldingTrap);
	}
}

void ABaseDefenderCharacter::ChooseTrap(const float TrapIndex)
{
	if(TrapIndex > 0.f)
	{
		bHoldingTrap = true;
		SelectedTrapIndex = TrapIndex - 1;
		LoadPlaceholderTrap();
		OnTrapIndexChanged.Broadcast();
		OnHoldingTrap.Broadcast(true);
	}

}

void ABaseDefenderCharacter::LoadPlaceholderTrap()
{
	if (bDead)
		return;

	UWorld* MyWorld = GetWorld();
	FHitResult HitResult;
	if (PlaceholderTrap != nullptr)
	{
		PlaceholderTrap->AutoDestruct();
		PlaceholderTrap = nullptr;
	}

	if (bHoldingTrap && SelectedTrapIndex >= 0  && SelectedTrapIndex < TrapTypes.Num()
		&& MyWorld->LineTraceSingleByChannel(HitResult, FollowCamera->GetComponentLocation(),
		FollowCamera->GetForwardVector() * TrapPlacingDistance + FollowCamera->GetComponentLocation(), ECollisionChannel::ECC_Visibility)
		&& !IsPlaceholderTrapOverlappingAntiTrapZone(HitResult.Component))
	{
		PlaceholderTrap = MyWorld->SpawnActorDeferred<ATrap>(TrapTypes[SelectedTrapIndex], UKismetMathLibrary::MakeTransform(HitResult.ImpactPoint, { 0, CameraBoom->GetTargetRotation().Yaw, CameraBoom->GetTargetRotation().Roll }, FVector::OneVector));
		PlaceholderTrap->DeactivateTrap();
		PlaceholderTrap->ResizeTrap(TrapSize);
        PlaceholderTrap->FinishSpawning(PlaceholderTrap->GetTransform());

		GetWorldTimerManager().SetTimer(TimerHandle_PlaceHolderTrapFollowsCamera, this, &ABaseDefenderCharacter::MovePlaceholderTrap, PLACEHOLDER_TRAP_REFRESH_RATE, true);
	}
	else
	{
		if (PlaceholderTrap != nullptr)
		{
			PlaceholderTrap->AutoDestruct();
			PlaceholderTrap = nullptr;
		}
		if (!bHoldingTrap)
		{
			GetWorldTimerManager().ClearTimer(TimerHandle_PlaceHolderTrapFollowsCamera);
		}
		else
		{
			GetWorldTimerManager().SetTimer(TimerHandle_PlaceHolderTrapFollowsCamera, this, &ABaseDefenderCharacter::LoadPlaceholderTrap, PLACEHOLDER_TRAP_REFRESH_RATE, true);
		}
	}

}

void ABaseDefenderCharacter::MovePlaceholderTrap()
{
	if (bDead)
		return;

	if(PlaceholderTrap && bHoldingTrap)
	{
		FHitResult HitResult;

		if(GetWorld()->LineTraceSingleByChannel(HitResult, FollowCamera->GetComponentLocation(),
			FollowCamera->GetForwardVector() * TrapPlacingDistance + FollowCamera->GetComponentLocation(), ECollisionChannel::ECC_Visibility)
			&& !IsPlaceholderTrapOverlappingAntiTrapZone(HitResult.Component))
		{
			PlaceholderTrap->SetActorLocation(HitResult.Location);
			PlaceholderTrap->SetActorRotation({0, CameraBoom->GetTargetRotation().Yaw, CameraBoom->GetTargetRotation().Roll });
		}
		else
		{
			PlaceholderTrap->AutoDestruct();
			GetWorldTimerManager().SetTimer(TimerHandle_PlaceHolderTrapFollowsCamera, this, &ABaseDefenderCharacter::LoadPlaceholderTrap, PLACEHOLDER_TRAP_REFRESH_RATE, true);
		}
	}
	else if (!bHoldingTrap)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_PlaceHolderTrapFollowsCamera);
	}
	else
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PlaceHolderTrapFollowsCamera, this, &ABaseDefenderCharacter::LoadPlaceholderTrap, PLACEHOLDER_TRAP_REFRESH_RATE, true);
	}
}

bool ABaseDefenderCharacter::IsPlaceholderTrapOverlappingAntiTrapZone(TWeakObjectPtr<UPrimitiveComponent> OverlappedComponent) const
{
	if(Cast<UAntiTrapComponent>(OverlappedComponent))
	{
		return true;
	}
	return false;
}

void ABaseDefenderCharacter::DeleteAttackCooldown()
{
	bAttackCooldown = false;
	if (bAttacking)
		Fire();
}

bool ABaseDefenderCharacter::IsAttack() const
{
	return bAttacking;
}

void ABaseDefenderCharacter::Zoom()
{
	if(!Zooming)
	{
		FollowCamera->FieldOfView -= 50;
		Zooming = true;
	}
}

void ABaseDefenderCharacter::UnZoom()
{
	if(Zooming)
	{
		FollowCamera->FieldOfView += 50;
		Zooming = false;
	}
}

int ABaseDefenderCharacter::GetTrapIndex() const
{
	return SelectedTrapIndex;
}

FString ABaseDefenderCharacter::GetTrapName(const int TrapIndex) const
{
	return TrapTypes[TrapIndex].GetDefaultObject()->GetTrapName();
}

UHealthComponent* ABaseDefenderCharacter::GetHealthComponent() const
{
	return HealthComp;
}

void ABaseDefenderCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	HealthComp->HealthPoints = 0.f;
	GetCharacterMovement()->StopMovementImmediately();
	bDead = true;
	this->bUseControllerRotationYaw = false;

	// Check Lose condition on player death
	GameModeReference->CheckPlayerHealth(0.f);
	
	//Destroy();

	this->SetActorHiddenInGame(true);
}

void ABaseDefenderCharacter::Jump()
{
	if (!bDead)
		Super::Jump();
}

void ABaseDefenderCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	if(!bDead)
	Jump();
}

void ABaseDefenderCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if(!bDead)
    StopJumping();
}

void ABaseDefenderCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABaseDefenderCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABaseDefenderCharacter::MoveForward(float Value)
{
	if (!bDead && (Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		UPlayerMovementComponent* PlayerMovement = Cast<UPlayerMovementComponent>(GetMovementComponent());
		if(PlayerMovement)
		{
			if (Value > 0.f)
			{
				bMovingForward = true;
				if(PlayerMovement->IsHoldingSprint())
				{
					PlayerMovement->Sprint();
				}
			}
			else
			{
				bMovingForward = false;
				PlayerMovement->SoftStopSprint();
			}
		}
	}

}

void ABaseDefenderCharacter::MoveRight(float Value)
{
	if (!bDead && (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);

		if (Value > 0.f)
		{
			bMovingRight = true;
		}
		else
		{
			bMovingRight = false;
		}
	}
	SideSpeed = Value;
}