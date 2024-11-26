// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FDuraLevelUpInfo
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    int32 LevelUpRequirement = 0;

    UPROPERTY(EditDefaultsOnly)
    int32 AttributePointAward = 1;

    UPROPERTY(EditDefaultsOnly)
    int32 SpellPointAward = 1;

};

/**
 * 
 */
UCLASS()
class DURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
public:

    UPROPERTY(EditDefaultsOnly)
    TArray<FDuraLevelUpInfo> LevelUpInformation;

    int32 FindLevelForXP(int32 XP) const;
};
