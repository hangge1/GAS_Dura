// Copyright by person HDD  


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTFunctionLibrary.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwninigPawn = AIOwner->GetPawn();

	const FName TargetTag = OwninigPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwninigPawn, TargetTag, ActorsWithTag);
	
	float ClosetDistance = TNumericLimits<float>::Max();
	AActor* ClosetActor = nullptr;
	for (AActor* Actor : ActorsWithTag)
	{
		//GEngine->AddOnScreenDebugMessage(2, .5f, FColor::Orange, *Actor->GetName());

		if (IsValid(Actor) && IsValid(OwninigPawn))
		{
			const float Distance = OwninigPawn->GetDistanceTo(Actor);
			if (Distance < ClosetDistance)
			{
				ClosetDistance = Distance;
				ClosetActor = Actor;
			}
		}
	}

	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosetActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosetDistance);
}
