// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "SmokeGrenadeAbility.generated.h"

/**
 * 
 */
UCLASS()
class PTEST_API USmokeGrenadeAbility : public UAbilityBase
{
	GENERATED_BODY()

public:
	USmokeGrenadeAbility();

	// Override ActivateAbility to spawn smoke grenade
	virtual void ActivateAbility(AActor* Instigator) override;


};
