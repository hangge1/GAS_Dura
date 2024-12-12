// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "DuraAbilitiesTypes.h"
#include "DuraAbilitySystemLibrary.generated.h"

class UDuraOverlayWidgetController;
class UAttributeMenuWidgetController;
class USpellMenuWidgetController;
class UAbilityInfo;

/**
 * 
 */
UCLASS()
class DURA_API UDuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
    
    /* Widget Controller */

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, 
        FWidgetControllerParams& OutWCParams, ADuraHUD*& OutDuraHUD);
    

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UDuraOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);


    /* Ability System Startup */

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, 
		ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

    /* Effect Comtext Getter */

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
    static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
    static FVector GetKocnbackForce(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
    static bool IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
    static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
    static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
    static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle);

    /* Effect Comtext Setter */

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDebuff);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float bInDebuffDamage);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float bInDebuffDuration);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float bInDebuffFrequency);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, FGameplayTag InDamageType);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
    static void SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle, FVector InDeathImpulse);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
    static void SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle, FVector InKnockbackForce);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInsRadialDamage);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageInnerRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float bInRadialDamageInnerRadius);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOuterRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float bInRadialDamageOuterRadius);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
    static void SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle, FVector InRadialDamageOrigin);

    /* Gameplay Mechanics */

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, 
		const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereLocation);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
    static void GetClosetTargets(int32 MaxTargets, const TArray<AActor*>& Actors, const FVector& Origin, TArray<AActor*>& OutClosestTargets);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
    static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect")
    static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
    static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
    static TArray<FVector> EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors);
    
    static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject,const ECharacterClass CharacterClass,const int32 CharacterLevel);

};
