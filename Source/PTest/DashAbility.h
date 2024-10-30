// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "DashAbility.generated.h"

/**
 * 
 */
UCLASS()
class PTEST_API UDashAbility : public UAbilityBase
{
	GENERATED_BODY()

public:
	UDashAbility();

	// Override ActivateAbility to provide dash logic
	virtual void ActivateAbility(AActor* Instigator) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashDistance = 1000.f;
	
};
