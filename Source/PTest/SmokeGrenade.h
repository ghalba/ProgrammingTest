// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmokeGrenade.generated.h"

UCLASS()
class PTEST_API ASmokeGrenade : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASmokeGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	float SmokeDuration = 5.0f;

	UFUNCTION()
	void OnSmokeEnd();

};
