// Copyright by person HDD  


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwingAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwingAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	Super::Activate();

	APlayerController* Controller = Ability->GetCurrentActorInfo()->PlayerController.Get();
	if (!Controller)
	{
		return;
	}

	FHitResult hitResult;
	/*if (!Controller->GetHitResultUnderCursor(ECC_Visibility, false, hitResult))
	{
		return;
	}*/

	Controller->GetHitResultUnderCursor(ECC_Visibility, false, hitResult);

	ValidData.Broadcast(hitResult.ImpactPoint);
}
