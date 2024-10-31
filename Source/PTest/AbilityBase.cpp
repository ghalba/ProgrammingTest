// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityBase.h"
#include "TimerManager.h"
#include "Engine/World.h"

void UAbilityBase::ActivateAbility(AActor* Instigator)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// Check if cooldown is still active
	if (CurrentTime - LastActivatedTime < CooldownDuration)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability on cooldown. Try again later."));
		return;
	}

	LastActivatedTime = CurrentTime;	

	UE_LOG(LogTemp, Log, TEXT("%s activated!"), *AbilityName);

    USceneComponent* ThrowPoint = Instigator->FindComponentByClass<USceneComponent>();
    if (!ThrowPoint)
    {
        UE_LOG(LogTemp, Error, TEXT("No valid Throw Point found on the Instigator!"));
        return;
    }
    // Use ThrowPoint's location and rotation for spawning
    FVector SpawnLocation = ThrowPoint->GetComponentLocation();
    FRotator SpawnRotation = ThrowPoint->GetComponentRotation();

    // Spawn the actor if a valid class is assigned
    if (SpawnClass!= nullptr)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = Instigator;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AActor* SpawnedActor = Instigator->GetWorld()->SpawnActor<AActor>(SpawnClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

        if (SpawnedActor)
        {
            UPrimitiveComponent* RootComponent = Cast<UPrimitiveComponent>(SpawnedActor->GetRootComponent());
            if (RootComponent)
            {
                if (!RootComponent->IsSimulatingPhysics())
                {
                    RootComponent->SetSimulatePhysics(true);  // Enable physics if it's not enabled
                }
                // Check if the root component can simulate physics
            
                if (RootComponent && RootComponent->IsSimulatingPhysics())
                {
                    FVector ThrowDirection = Instigator->GetActorForwardVector() + FVector(0, 0, 0.5f);
                    RootComponent->AddImpulse(ThrowDirection * ThrowForce, NAME_None, true);
                }
            }
            UE_LOG(LogTemp, Warning, TEXT("Actor spawned successfully!"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn actor."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnClass is NULL! Please assign it in the Editor."));
    }
	
}

void UAbilityBase::ResetCooldown()
{

	UE_LOG(LogTemp, Log, TEXT("%s is ready!"), *AbilityName);
}