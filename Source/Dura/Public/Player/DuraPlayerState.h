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

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<ULevelUpInfo> LevelUpInfoDataAsset;
	
    FOnPlayerStatChanged OnXPChangedDelegate;
    FOnPlayerStatChanged OnLevelChangedDelegate;
    FOnPlayerStatChanged OnAttributePointsChangedDelegate;
    FOnPlayerStatChanged OnSpellPointsChangedDelegate;

    FORCEINLINE int32 GetPlayerLevel() const { return Level; }
    FORCEINLINE int32 GetXP() const { return XP; }
    FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
    FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }

    void AddToXP(int32 InXP);
    void AddToLevel(int32 InLevel);
    void AddToAttributePoints(int32 InAttributePoints);
    void AddToSpellPoints(int32 InSpellPoints);


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

    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints);
	int32 AttributePoints = 0;

    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints);
	int32 SpellPoints = 0;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

    UFUNCTION()
	void OnRep_XP(int32 OldXP);

    UFUNCTION()
    void OnRep_AttributePoints(int32 Old_AttributePoints);

    UFUNCTION()
    void OnRep_SpellPoints(int32 Old_SpellPoints);
};
