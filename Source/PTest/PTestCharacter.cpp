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

}

void APTestCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	//this to test spawn at the begining
	if (SmokeGrenadeClass)
	{
		//FVector Location = GetActorLocation() + GetActorForwardVector() * 200.0f;
		//GetWorld()->SpawnActor<ASmokeGrenade>(SmokeGrenadeClass, Location, FRotator::ZeroRotator);
		/*FVector SpawnLocation = Instigator->GetActorLocation() +
			Instigator->GetActorForwardVector() * 200.0f +
			FVector(0.0f, 0.0f, 50.0f); // Raise by 50 units*/
		UE_LOG(LogTemp, Warning, TEXT("Test spawn of Smoke Grenade in BeginPlay."));


	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SmokeGrenadeClass is NULL!"));
	}
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
	
		// Bind the actions to the corresponding functions
		PlayerInputComponent->BindAction("FireAction", IE_Pressed, this, &APTestCharacter::FireObject);
		PlayerInputComponent->BindAction("PickUpAction", IE_Pressed, this, &APTestCharacter::PickupObject);
		PlayerInputComponent->BindAction("UseDash", IE_Pressed, this, &APTestCharacter::UseDash);
		PlayerInputComponent->BindAction("ThrowSmokeGrenade", IE_Pressed, this, &APTestCharacter::ThrowSmokeGrenade);

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
void APTestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HeldObject)
	{

		FVector TargetLocation = GetFirstPersonCameraComponent()->GetComponentLocation() + (GetFirstPersonCameraComponent()->GetForwardVector() * HoldDistance)
			+ FVector(0.0f, 0.0f, HoldHeightOffset);

		// Move the object smoothly to the target location
		FVector CurrentLocation = HeldObject->GetActorLocation();
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 10.0f);

		HeldObject->SetActorLocation(NewLocation);
	}
}

void APTestCharacter::PickupObject()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, TEXT("pickup object"));
	FVector CameraLocation;
	FRotator CameraRotation;
	GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector End = CameraLocation + (CameraRotation.Vector() * PickupRadius);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, End, ECC_PhysicsBody, Params);
	/*FColor LineColor = bHit ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), CameraLocation, End, LineColor, false, 1.0f, 0, 2.0f);*/

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
				UE_LOG(LogTemp, Warning, TEXT("Picked up : %s"), *HeldObject->GetName());
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

void APTestCharacter::FireObject()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Emerald, TEXT("fire object"));
	if (HeldObject)
	{
		FVector FireDirection = GetActorForwardVector();
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