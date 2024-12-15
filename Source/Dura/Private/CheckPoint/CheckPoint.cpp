// Copyright by person HDD  


#include "CheckPoint/CheckPoint.h"
#include "Components/SphereComponent.h"
#include <Interaction\PlayerInterface.h>

ACheckPoint::ACheckPoint(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = false;

    CheckPointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckPointMesh");
    CheckPointMesh->SetupAttachment(GetRootComponent());
    CheckPointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CheckPointMesh->SetCollisionResponseToAllChannels(ECR_Block);

    Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
    Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    Sphere->SetupAttachment(CheckPointMesh);
}

void ACheckPoint::OnSphereOverlap(UPrimitiveComponent* OverlamppedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(OtherActor->Implements<UPlayerInterface>())
    {
        IPlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);
        HandleGlowEffects();
    }
}

void ACheckPoint::BeginPlay()
{
    Super::BeginPlay();

    Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnSphereOverlap);
}

void ACheckPoint::HandleGlowEffects()
{
    Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(CheckPointMesh->GetMaterial(0), this);
    CheckPointMesh->SetMaterial(0, DynamicMaterialInstance);
    CheckpointReached(DynamicMaterialInstance);
}
