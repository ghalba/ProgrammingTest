// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityBase.h"
#include "TimerManager.h"
#include "Engine/World.h"


void UAbilityBase::ActivateAbility(AActor* Instigator)
{
	if (IsOnCooldown())
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability is on cooldown."));
		return;
	}
	LastActivatedTime = GetWorld()->GetTimeSeconds();
	UE_LOG(LogTemp, Log, TEXT("Ability %s activated."), *GetName());

	// Trigger the cooldown reset
	StartCooldown();
}

void UAbilityBase::StartCooldown()
{
	if (CooldownDuration > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UAbilityBase::ResetCooldown, CooldownDuration, false);
	}
}

void UAbilityBase::ResetCooldown()
{
	UE_LOG(LogTemp, Log, TEXT("Ability %s cooldown reset."), *GetName());
}
bool UAbilityBase::IsOnCooldown() const
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	return (CurrentTime - LastActivatedTime) < CooldownDuration;
}
