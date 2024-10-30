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
	// Store the activation time
	LastActivatedTime = CurrentTime;	

	UE_LOG(LogTemp, Log, TEXT("%s activated!"), *AbilityName);
    // Spawn the actor if a valid class is assigned
    if (SpawnClass!= nullptr)
    {
        FVector SpawnLocation = Instigator->GetActorLocation() + Instigator->GetActorForwardVector() * 200.0f;
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = Instigator;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        UE_LOG(LogTemp, Log, TEXT("%s activated!"), *SpawnLocation.ToString());

        AActor* SpawnedActor = Instigator->GetWorld()->SpawnActor<AActor>(SpawnClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

        if (SpawnedActor)
        {
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