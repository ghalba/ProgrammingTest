// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilityBase.h"
#include "SmokeGrenade.h"
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
	
public:
	//APTestCharacter(const FObjectInitializer& ObjectInitializer);
	APTestCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	UAbilityBase* DashAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	UAbilityBase* SmokeGrenadeAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities/SmokeGrenade")
	TSubclassOf<ASmokeGrenade> SmokeGrenadeClass;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GravityGun", meta = (ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0"))
	float PickupRadius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GravityGun")
	float FireForce = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GravityGun")
	float HoldDistance = 200.0f;  // Distance in front of the player

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GravityGun")
	float HoldHeightOffset = 100.0f;  // Height offset above the player

	UFUNCTION(BlueprintCallable, Category = "GravityGun")
	void PickupObject();
	UFUNCTION(BlueprintCallable, Category = "GravityGun")
	void FireObject();

	void UseDash();
	void ThrowSmokeGrenade();

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;
public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
private:
	AActor* HeldObject = nullptr;
};

