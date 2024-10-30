// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityBase.h"
#include "TimerManager.h"
#include "Engine/World.h"

void UAbilityBase::ActivateAbility(AActor* Instigator)
{
	if (bIsOnCooldown)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is on cooldown!"), *AbilityName);
		return;
	}

	//UE_LOG(LogTemp, Log, TEXT("%s activated!"), *AbilityName);
	// Start cooldown
	bIsOnCooldown = true;
	Instigator->GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &UAbilityBase::ResetCooldown, CooldownTime, false);
}

void UAbilityBase::ResetCooldown()
{
	bIsOnCooldown = false;
	UE_LOG(LogTemp, Log, TEXT("%s is ready!"), *AbilityName);
}