// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "Character/DuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "DuraCharacter.generated.h"

class UNiagaraComponent;
class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class DURA_API ADuraCharacter : public ADuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
	
public:
	ADuraCharacter();

    // Player Interface
    virtual void AddToXP_Implementation(int32 InXP) override;
    virtual void LevelUp_Implementation() override;
    virtual int32 GetXP_Implementation() const override;
    virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;

    virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
    virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override; 
    virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
    virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
    virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;

    virtual int32 GetAttributePoints_Implementation() const override;
    virtual int32 GetSpellPoints_Implementation() const override;
    // EndPlayer Interface

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

    virtual void OnRep_Stunned() override;

    virtual void OnRep_Burned() override;
protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual int32 GetPlayerLevel_Implementation() const override;
private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCameraComponent> TopDownCameraComponent;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USpringArmComponent> CameraBoom;

	virtual void InitAbilityActorInfo() override;

    UFUNCTION(NetMulticast, Reliable)
    void MulticastLevelUpParticles() const;
};
