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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	FString AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Cooldown")
	float CooldownDuration = 5.0f;  // Cooldown time in seconds
	
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void ActivateAbility(AActor* Instigator);


protected:
	

	bool IsOnCooldown() const;
	void StartCooldown();
	// Internal function to reset the cooldown flag
	void ResetCooldown();
	float LastActivatedTime = -9999.0f;  // Stores the last activation timestamp
	FTimerHandle CooldownTimerHandle;

	
};
