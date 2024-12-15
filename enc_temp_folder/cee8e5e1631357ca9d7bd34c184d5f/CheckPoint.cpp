// Copyright by person HDD  


#include "CheckPoint/CheckPoint.h"
#include "Components/SphereComponent.h"

ACheckPoint::ACheckPoint(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = false;

    Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
    Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    Sphere->SetupAttachment(GetRootComponent());

    CheckPointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckPointMesh");
    CheckPointMesh->SetupAttachment(GetRootComponent());
    CheckPointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CheckPointMesh->SetCollisionResponseToAllChannels(ECR_Block);

}

void ACheckPoint::OnSphereOverlap(UPrimitiveComponent* OverlamppedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(OtherActor->ActorHasTag(FName("Player")))
    {
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
