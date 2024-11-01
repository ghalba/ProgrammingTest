// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "DashAbility.h"
#include "SmokeGrenadeAbility.h"
#include "SmokeGrenade.h"
#include "Components/SpotLightComponent.h"
#include "PTestCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class APTestCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LightAction;
	
public:
	APTestCharacter();

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	UDashAbility* DashAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	USmokeGrenadeAbility* SmokeGrenadeAbility;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities|Light", meta = (AllowPrivateAccess = "true"))
	UPointLightComponent* LightComponent;  // Light component for the ability



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Light")
	float LightIntensity = 3000.0f;  // Intensity of the light

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Light")
	float LightRadius = 500.0f;  // Radius of the light


	
	void UseDash();
	void ThrowSmokeGrenade();	

	// Function to activate the light
	void ToggleLight();
private:
	bool bIsLightActive = false;


protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);



	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface


};

