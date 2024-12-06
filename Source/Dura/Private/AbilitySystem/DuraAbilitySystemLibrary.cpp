// Copyright by person HDD  


#include "AbilitySystem/DuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/DuraHUD.h"
#include "Player/DuraPlayerState.h"
#include "Player/DuraPlayerController.h"
#include "UI/WidgetController/DuraOverlayWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "Game/DuraGameModeBase.h"
#include "AbilitySystemComponent.h"
#include "DuraAbilitiesTypes.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DuraGameplayTags.h"


bool UDuraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,
    FWidgetControllerParams& OutWCParams, ADuraHUD*& OutDuraHUD)
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ADuraHUD* DuraHUD = Cast<ADuraHUD>(PC->GetHUD()))
		{
			ADuraPlayerState* PS = PC->GetPlayerState<ADuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

            OutWCParams.AS = AS;
            OutWCParams.PS = PS;
            OutWCParams.PC = PC;
            OutWCParams.ASC = ASC;

            OutDuraHUD = DuraHUD;
			return true;
		}
	}
    return false;
}

UDuraOverlayWidgetController* UDuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
    FWidgetControllerParams WidgetControllerParams;
    ADuraHUD* DuraHUD = nullptr;

    if(!MakeWidgetControllerParams(WorldContextObject, WidgetControllerParams, DuraHUD))
    {
        return nullptr;
    }

	return DuraHUD->GetOverlayWidgetController(WidgetControllerParams);
}

UAttributeMenuWidgetController* UDuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
    FWidgetControllerParams WidgetControllerParams;
    ADuraHUD* DuraHUD = nullptr;

    if(!MakeWidgetControllerParams(WorldContextObject, WidgetControllerParams, DuraHUD))
    {
        return nullptr;
    }

	return DuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
}

USpellMenuWidgetController* UDuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
    FWidgetControllerParams WidgetControllerParams;
    ADuraHUD* DuraHUD = nullptr;

    if(!MakeWidgetControllerParams(WorldContextObject, WidgetControllerParams, DuraHUD))
    {
        return nullptr;
    }

	return DuraHUD->GetSpellMenuWidgetController(WidgetControllerParams);
}

void UDuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	
	AActor* AvatarActor = ASC->GetAvatarActor();

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data);

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data);

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data);
}

void UDuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (!CharacterClassInfo) return;

	for (const auto& AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}

	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

    
    if(ASC->GetAvatarActor()->Implements<UCombatInterface>())
    {
        int32 PlayerLevel = 1;
        PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor());

        for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	    {	
           FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, PlayerLevel);
	        ASC->GiveAbility(AbilitySpec);
	    }
    }	
}

int32 UDuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, 
    const ECharacterClass CharacterClass, const int32 CharacterLevel)
{
    UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (!CharacterClassInfo) return 0;

    const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
    const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

    return static_cast<int32>(XPReward);
}

UCharacterClassInfo* UDuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	ADuraGameModeBase* GameMode = Cast<ADuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GameMode == nullptr) return nullptr;
	return GameMode->CharacterClassInfo;
}

UAbilityInfo* UDuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
    ADuraGameModeBase* GameMode = Cast<ADuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GameMode == nullptr) return nullptr;
	return GameMode->AbilityInfo;
}

bool UDuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDuraGameplayEffectContext* DuraEffectContext = static_cast<const FDuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return DuraEffectContext->IsBlockedHit();
	}
	return false;
}

bool UDuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDuraGameplayEffectContext* DuraEffectContext = static_cast<const FDuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return DuraEffectContext->IsCriticalHit();
	}
	return false;
}

void UDuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, 
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereLocation)
{
	OutOverlappingActors.Reset();

	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereLocation, FQuat::Identity, 
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), 
			FCollisionShape::MakeSphere(Radius), SphereParams);

		for (FOverlapResult& Overlap : Overlaps)
		{
			const bool bImplementsCombatInterface = Overlap.GetActor()->Implements<UCombatInterface>();		
			if (bImplementsCombatInterface)
			{
				const bool IsAlive = !ICombatInterface::Execute_IsDead(Overlap.GetActor());
				if (IsAlive)
				{
					OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
				}
			}
		}
	}


}

bool UDuraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
    const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
    const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
    const bool bFriends = bBothArePlayers || bBothAreEnemies;
    return !bFriends;
}



FGameplayEffectContextHandle UDuraAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
    const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

    FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
    EffectContextHandle.AddSourceObject(SourceAvatarActor);

    FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(
        DamageEffectParams.DamageGameplayEffectClass, 
        DamageEffectParams.AbilityLevel, 
        EffectContextHandle
    );

    const FDuraGameplayTags& GameplayTags = FDuraGameplayTags::Get();

    //Set By Caller 
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, 
    DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);

    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, 
    GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);

    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, 
    GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);

    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, 
    GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);

    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, 
    GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);

    DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

    return EffectContextHandle;
}

void UDuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FDuraGameplayEffectContext* DuraEffectContext = static_cast<FDuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		DuraEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UDuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FDuraGameplayEffectContext* DuraEffectContext = static_cast<FDuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		DuraEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}
