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

    virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial) override;
    virtual void HideMagicCircle_Implementation() override;

    virtual void SaveProgress_Implementation(const FName& CheckPointTag) override;
    // EndPlayer Interface

    //Combat Interface 

    virtual int32 GetPlayerLevel_Implementation() const override;
    virtual void Die(const FVector& DeathImpulse) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float DeathTime = 5.f;

    FTimerHandle DeathTimer;
    //End Combat Interface

    virtual void OnRep_Stunned() override;
    virtual void OnRep_Burned() override;

    void LoadProgress();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;
protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCameraComponent> TopDownCameraComponent;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USpringArmComponent> CameraBoom;

	virtual void InitAbilityActorInfo() override;

    UFUNCTION(NetMulticast, Reliable)
    void MulticastLevelUpParticles() const;
};
