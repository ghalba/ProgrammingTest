// Fill out your copyright notice in the Description page of Project Settings.


#include "SmokeGrenadeAbility.h"
#include "SmokeGrenade.h"

USmokeGrenadeAbility::USmokeGrenadeAbility()
{
	AbilityName = "Smoke Grenade";
	CooldownTime = 10.0f;  // 10 seconds cooldown
	Damage = 0.0f;  // No damage, just utility
}

void USmokeGrenadeAbility::ActivateAbility(AActor* Instigator)
{
		// Automatically assign a class at construction
		/*static ConstructorHelpers::FClassFinder<AActor> GrenadeClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_SmokeGrenade"));
		if (GrenadeClassFinder.Succeeded())
		{
			SmokeGrenadeClass = GrenadeClassFinder.Class;
		}*/
	
	Super::ActivateAbility(Instigator);  // Check cooldown
	UE_LOG(LogTemp, Warning, TEXT("Activating Smoke Grenade!"));
	if (SmokeGrenadeClass)
	{
		FVector SpawnLocation = Instigator->GetActorLocation() + Instigator->GetActorForwardVector() * 200.0f;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Instigator;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor* Grenade = Instigator->GetWorld()->SpawnActor<AActor>(
			SmokeGrenadeClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams
		);

		if (Grenade)
		{
			UE_LOG(LogTemp, Warning, TEXT("Smoke Grenade spawned successfully!"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn Smoke Grenade."));
		}

	}
	else
	{

		UE_LOG(LogTemp, Error, TEXT("SmokeGrenadeClass is NULL! Make sure it is assigned in the Editor."));
		return;

	}
}