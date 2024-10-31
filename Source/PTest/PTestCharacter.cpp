// Copyright Epic Games, Inc. All Rights Reserved.

#include "PTestCharacter.h"
#include "PTestProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Components/SpotLightComponent.h"
#include "DashAbility.h"
#include "SmokeGrenadeAbility.h"



DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APTestCharacter

APTestCharacter::APTestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// Set up the Dash ability
	DashAbility = CreateDefaultSubobject<UDashAbility>(TEXT("DashAbility"));

	// Set up the Smoke Grenade ability
	SmokeGrenadeAbility = CreateDefaultSubobject<USmokeGrenadeAbility>(TEXT("SmokeGrenadeAbility"));

	
	LightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	LightComponent->SetupAttachment(RootComponent);  // Attach to the root component
	LightComponent->SetIntensity(LightIntensity);
	LightComponent->SetAttenuationRadius(LightRadius);
	LightComponent->SetVisibility(false); // Initially, the light is off

}

//////////////////////////////////////////////////////////////////////////// Input

void APTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APTestCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APTestCharacter::Look);
		EnhancedInputComponent->BindAction(LightAction, ETriggerEvent::Triggered, this, &APTestCharacter::ToggleLight);


		// Bind the actions to the corresponding functions
		//PlayerInputComponent->BindAction("FireAction", IE_Pressed, this, &APTestCharacter::FireObject);
		//PlayerInputComponent->BindAction("PickUpAction", IE_Pressed, this, &APTestCharacter::PickupObject);
		PlayerInputComponent->BindAction("UseDash", IE_Pressed, this, &APTestCharacter::UseDash);
		PlayerInputComponent->BindAction("ThrowSmokeGrenade", IE_Pressed, this, &APTestCharacter::ThrowSmokeGrenade);
		// Bind the input action for toggling light
		PlayerInputComponent->BindAction("LightAction", IE_Pressed, this, &APTestCharacter::ToggleLight);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void APTestCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void APTestCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}






void APTestCharacter::UseDash()
{
	if (DashAbility)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("usedash"));
		DashAbility->ActivateAbility(this);
	}
}
void APTestCharacter::ThrowSmokeGrenade()
{
	if (SmokeGrenadeAbility)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Orange, TEXT("throwSmokeGrenade"));
		SmokeGrenadeAbility->ActivateAbility(this);
	}
}

void APTestCharacter::ToggleLight()
{
	UE_LOG(LogTemp, Log, TEXT("ToggleLight activated."));
	if (LightComponent)
	{
		bIsLightActive = !bIsLightActive;  // Toggle the state

		if (bIsLightActive)
		{
			LightComponent->SetVisibility(true);  // Turn on the light
			UE_LOG(LogTemp, Log, TEXT("Light activated with intensity: %f"), LightIntensity);
		}
		else
		{
			LightComponent->SetVisibility(false);  // Turn off the light
			UE_LOG(LogTemp, Log, TEXT("Light deactivated."));
		}
	}
}