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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	FString AbilityName;

	// Damage value (if applicable)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float Damage;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Cooldown")
	float CooldownDuration = 5.0f;  // Cooldown time in seconds

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Spawning")
	TSubclassOf<AActor> SpawnClass;  // Class of the actor to spawn
	
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void ActivateAbility(AActor* Instigator);

	float LastActivatedTime = -9999.0f;  // Stores the last activation timestamp


private:

	

	
protected:
	

	// Internal function to reset the cooldown flag
	void ResetCooldown();

	
};
