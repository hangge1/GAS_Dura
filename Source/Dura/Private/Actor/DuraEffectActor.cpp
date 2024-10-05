// Copyright by person HDD  


#include "Actor/DuraEffectActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/DuraAttributeSet.h"


ADuraEffectActor::ADuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());
}


void ADuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ADuraEffectActor::OnOverlap);
}

void ADuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IAbilitySystemInterface* IASInterface = Cast<IAbilitySystemInterface>(OtherActor);
	if (IASInterface == nullptr)
	{
		return;
	}

	const UDuraAttributeSet* DuraAttributeSet = Cast<UDuraAttributeSet>(IASInterface->GetAbilitySystemComponent()->
		GetAttributeSet(UDuraAttributeSet::StaticClass()));

	UDuraAttributeSet* MutableDuraAttributeSet = const_cast<UDuraAttributeSet*>(DuraAttributeSet);
	MutableDuraAttributeSet->SetHealth(MutableDuraAttributeSet->GetHealth() + 25.0f);

	Destroy();
}
