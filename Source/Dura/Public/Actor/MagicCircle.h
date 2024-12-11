// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

class UDecalComponent;

UCLASS()
class DURA_API AMagicCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	AMagicCircle();
    virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UDecalComponent> MagicCircleDecay;
};
