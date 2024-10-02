// Copyright by person HDD  


#include "DuraCharacterBase.h"

// Sets default values
ADuraCharacterBase::ADuraCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADuraCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADuraCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

