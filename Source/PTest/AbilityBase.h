// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class PTEST_API UAbilityBase : public UObject
{
	GENERATED_BODY()
public:
	// Ability name for debugging or UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FString AbilityName;

	// Damage value (if applicable)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float Damage;

	// Cooldown duration for the ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float CooldownTime;

	// Checks if the ability is ready to use
	bool bIsOnCooldown = false;

	// BlueprintCallable for easy access in Blueprints
	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual void ActivateAbility(AActor* Instigator);

protected:
	// Handle cooldown timing
	FTimerHandle CooldownTimer;

	// Internal function to reset the cooldown flag
	void ResetCooldown();

	
};
