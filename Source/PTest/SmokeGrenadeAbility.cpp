// Fill out your copyright notice in the Description page of Project Settings.


#include "SmokeGrenadeAbility.h"
#include "SmokeGrenade.h"

USmokeGrenadeAbility::USmokeGrenadeAbility()
{
	AbilityName = "Smoke Grenade";

	Damage = 1.0f;  
}

void USmokeGrenadeAbility::ActivateAbility(AActor* Instigator)
{
	
	Super::ActivateAbility(Instigator);  // Check cooldown

    USceneComponent* ThrowPoint = Instigator->FindComponentByClass<USceneComponent>();
    if (!ThrowPoint)
    {
        UE_LOG(LogTemp, Error, TEXT("No valid Throw Point found on the Instigator!"));
        return;
    }
    // Use ThrowPoint's location and rotation for spawning
    FVector SpawnLocation = ThrowPoint->GetComponentLocation();
    FRotator SpawnRotation = ThrowPoint->GetComponentRotation();
    UE_LOG(LogTemp, Warning, TEXT("ThrowPointLocation: %s"), *SpawnLocation.ToString());
    UE_LOG(LogTemp, Warning, TEXT("ThrowPointSpawnRotation: %s"), *SpawnRotation.ToString());
    // Spawn the actor if a valid class is assigned
    if (SmokeGrenadeClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = Instigator;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        //AActor* SpawnedActor = Instigator->GetWorld()->SpawnActor<AActor>(SmokeGrenadeClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
        SmokeGrenade = GetWorld()->SpawnActor<ASmokeGrenade>(SmokeGrenadeClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
        
        SmokeGrenade->SmokeGrenadeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        SmokeGrenade->SmokeGrenadeMesh->SetSimulatePhysics(true);
        SmokeGrenade->SmokeGrenadeMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
        FVector ThrowDirection = Instigator->GetActorForwardVector() + FVector(0, 0, 0.5f);
        //SmokeGrenade->SmokeGrenadeMesh->AddImpulse(ThrowDirection * ThrowForce, NAME_None, true);
        FVector InitialVelocity = ThrowDirection * ThrowForce;
        SmokeGrenade->SmokeGrenadeMesh->AddImpulse(InitialVelocity, NAME_None, true);

        //SmokeGrenade->SmokeGrenadeMesh->SetPhysicsLinearVelocity(InitialVelocity, true);  // Set initial velocity
        UE_LOG(LogTemp, Warning, TEXT("Initial velocity applied: %s"), *InitialVelocity.ToString());
               
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnClass is NULL! Please assign it in the Editor (in play Mode): Go To BP_FirstPersonCharacter->Abilities|SmokeGrenadeAbility and set the SmokeGrenadeClass to mySmokeGrenade."));
    }
	
}