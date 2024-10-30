// Fill out your copyright notice in the Description page of Project Settings.


#include "SmokeGrenade.h"

// Sets default values
ASmokeGrenade::ASmokeGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetLifeSpan(20.0f);  // Keeps the grenade alive for 5 seconds
}

// Called when the game starts or when spawned
void ASmokeGrenade::BeginPlay()
{
	Super::BeginPlay();

	//GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ASmokeGrenade::OnSmokeEnd);

}
void ASmokeGrenade::OnSmokeEnd()
{
	Destroy();
}

// Called every frame
void ASmokeGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

