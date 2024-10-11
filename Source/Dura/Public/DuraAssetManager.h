// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "DuraAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class DURA_API UDuraAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static UDuraAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
};
