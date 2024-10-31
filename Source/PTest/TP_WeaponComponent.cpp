// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "PTestCharacter.h"
#include "PTestProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}
void UTP_WeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Character && HeldObject)
	{
		APTestCharacter* PTestCharacter = Cast<APTestCharacter>(Character);
		FVector TargetLocation = PTestCharacter->GetFirstPersonCameraComponent()->GetComponentLocation() + (PTestCharacter->GetFirstPersonCameraComponent()->GetForwardVector() * HoldDistance)
			+ FVector(0.0f, 0.0f, HoldHeightOffset);

		// Move the object smoothly to the target location
		FVector CurrentLocation = HeldObject->GetActorLocation();
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 10.0f);

		HeldObject->SetActorLocation(NewLocation);
	}
}
void UTP_WeaponComponent::PickupObject()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, TEXT("pickup object"));
	}
	UE_LOG(LogTemp, Warning, TEXT("PickupObject function triggered!"));
	FVector CameraLocation;
	FRotator CameraRotation;

	// Get the owning character
	ACharacter* OwnerCharacter = Character;
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter is invalid"));
		return;
	}

	// Get the player controller
	APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is invalid"));
		return;
	}

	// Get the player's viewpoint
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// Calculate the end point for the line trace
	FVector End = CameraLocation + (CameraRotation.Vector() * PickupRadius);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter); // Ignore the owning actor

	// Perform the line trace
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, End, ECC_PhysicsBody, Params);

	if (bHit)
	{
		HeldObject = Hit.GetActor();
		if (HeldObject)
		{
			UPrimitiveComponent* MeshComp = Cast<UPrimitiveComponent>(HeldObject->GetRootComponent());
			if (MeshComp)
			{
				// Disable physics simulation when holding the object
				MeshComp->SetSimulatePhysics(false);
				UE_LOG(LogTemp, Warning, TEXT("Picked up: %s"), *HeldObject->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No valid object found in line trace!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace did not hit any object!"));
	}
}

void UTP_WeaponComponent::FireObject()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Emerald, TEXT("fire object"));

	// Get the owning character
	ACharacter* OwnerCharacter = Character;
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter is invalid"));
		return;
	}

	if (HeldObject)
	{
		FVector FireDirection = OwnerCharacter->GetActorForwardVector();
		UPrimitiveComponent* ObjectRootComponent = Cast<UPrimitiveComponent>(HeldObject->GetRootComponent());
		if (ObjectRootComponent)
		{
			// Re-enable physics simulation
			ObjectRootComponent->SetSimulatePhysics(true);
			ObjectRootComponent->AddImpulse(FireDirection * FireForce, NAME_None, true);
			UE_LOG(LogTemp, Warning, TEXT("Fired: %s"), *HeldObject->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Root component is not a UPrimitiveComponent!"));
		}
		HeldObject = nullptr;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No object held to fire!"));
	}

	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	/*if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			// Spawn the projectile at the muzzle
			World->SpawnActor<APTestProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}*/
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

bool UTP_WeaponComponent::AttachWeapon(APTestCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UTP_WeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// pickupObject
			EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::PickupObject);
			// FireObject
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::FireObject);
		}
	}

	return true;
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}