// Copyright by person HDD  


#include "Actor/MagicCircle.h"
#include "Components/DecalComponent.h"

AMagicCircle::AMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;

    MagicCircleDecay = CreateDefaultSubobject<UDecalComponent>("MagicCircleDecay");
    MagicCircleDecay->SetupAttachment(GetRootComponent());
}


void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}


void AMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMagicCircle::SetMaterial(int32 Index, UMaterialInterface* DecalMaterial)
{
    MagicCircleDecay->SetMaterial(Index, DecalMaterial);
}

