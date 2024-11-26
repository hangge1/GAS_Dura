// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "DuraPlayerState.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*StatValue*/);

/**
 * 
 */
UCLASS()
class DURA_API ADuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ADuraPlayerState();
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

    TObjectPtr<ULevelUpInfo> LevelUpInfoDataAsset;
	
    FOnPlayerStatChanged OnXPChangedDelegate;
    FOnPlayerStatChanged OnLevelChangedDelegate;

    FORCEINLINE int32 GetPlayerLevel() const { return Level; }
    FORCEINLINE int32 GetXP() const { return XP; }

    void AddToXP(int32 InXP);
    void AddToLevel(int32 InLevel);


    void SetXP(int32 InXP);
    void SetLevel(int32 InLevel);
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitiesSystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level);
	int32 Level = 1;

    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP);
	int32 XP = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

    UFUNCTION()
	void OnRep_XP(int32 OldXP);
};
