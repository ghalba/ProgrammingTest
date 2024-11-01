// Fill out your copyright notice in the Description page of Project Settings.


#include "SmokeGrenade.h"

// Sets default values
ASmokeGrenade::ASmokeGrenade()
{
	
	PrimaryActorTick.bCanEverTick = true;
	//SetLifeSpan(20.0f);  // Keeps the grenade alive for 5 seconds

	SmokeGrenadeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	RootComponent = SmokeGrenadeMesh;
}

// Called when the game starts or when spawned
void ASmokeGrenade::BeginPlay()
{
	Super::BeginPlay();

	//GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ASmokeGrenade::OnSmokeEnd);

}
void ASmokeGrenade::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    //UE_LOG(LogTemp, Warning, TEXT("SmokeGrenadeMesh->IsSimulatingPhysics(): %s"), SmokeGrenadeMesh->IsSimulatingPhysics() ? TEXT("true") : TEXT("false"));

   /* if (SmokeGrenadeMesh && SmokeGrenadeMesh->IsSimulatingPhysics())
    {
        FVector ThrowDirection = GetActorForwardVector() + FVector(0, 0, 0.5f);
        FVector InitialImpulse = ThrowDirection * 1000.0f; // Adjust as needed

        SmokeGrenadeMesh->AddImpulse(InitialImpulse, NAME_None, true);

        UE_LOG(LogTemp, Warning, TEXT("Tick applying impulse: %s"), *InitialImpulse.ToString());


    }*/
}
void ASmokeGrenade::OnSmokeEnd()
{
	Destroy();
}



