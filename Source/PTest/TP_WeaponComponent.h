// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "TP_WeaponComponent.generated.h"

class APTestCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PTEST_API UTP_WeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class APTestProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Pickup Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* PickupAction;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UFUNCTION(BlueprintCallable, Category = "GravityGun")
	void PickupObject();

	UFUNCTION(BlueprintCallable, Category = "GravityGun")
	void FireObject();

	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool AttachWeapon(APTestCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GravityGun", meta = (ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0"))
	float PickupRadius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GravityGun")
	float FireForce = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GravityGun")
	float HoldDistance = 200.0f;  // Distance in front of the player

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GravityGun")
	float HoldHeightOffset = 100.0f;  // Height offset above the player

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	/** The Character holding this weapon*/
	APTestCharacter* Character;

	AActor* HeldObject = nullptr;
};
