// Fill out your copyright notice in the Description page of Project Settings.


#include "DashAbility.h"
#include "GameFramework/Character.h"

UDashAbility::UDashAbility()
{
	AbilityName = "Dash";

}

void UDashAbility::ActivateAbility(AActor* Instigator)
{
	Super::ActivateAbility(Instigator);  // Check cooldown

	if (ACharacter* Character = Cast<ACharacter>(Instigator))
	{
		FVector DashDirection = Character->GetActorForwardVector();
		FVector NewLocation = Character->GetActorLocation() + DashDirection * DashDistance;
		Character->SetActorLocation(NewLocation, true);
	}
}
