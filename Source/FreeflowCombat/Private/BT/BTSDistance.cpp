// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTSDistance.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTSDistance::UBTSDistance()
{
	NodeName = "Distance";
}

void UBTSDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AActor* A = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AKey.SelectedKeyName));
	AActor* B = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BKey.SelectedKeyName));

	if (A && B)
	{
		float Distance = FVector::Dist(A->GetActorLocation(), B->GetActorLocation());
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(DistanceKey.SelectedKeyName, Distance);
	}
}

