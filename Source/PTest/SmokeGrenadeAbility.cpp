// Fill out your copyright notice in the Description page of Project Settings.


#include "SmokeGrenadeAbility.h"
#include "SmokeGrenade.h"

USmokeGrenadeAbility::USmokeGrenadeAbility()
{
	AbilityName = "Smoke Grenade";

	Damage = 0.0f;  // No damage, just utility
}

void USmokeGrenadeAbility::ActivateAbility(AActor* Instigator)
{
	
	Super::ActivateAbility(Instigator);  // Check cooldown
	//UE_LOG(LogTemp, Warning, TEXT("Activating Smoke Grenade!"));
	
	
}