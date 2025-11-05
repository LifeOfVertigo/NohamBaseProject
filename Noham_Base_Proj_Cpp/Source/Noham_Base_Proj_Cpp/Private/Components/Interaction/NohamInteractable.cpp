// Copyright Noham Studios. All Rights Reserved.

#include "Components/Interaction/NohamInteractable.h"

// Default implementations for interface functions
// These can be overridden in C++ or Blueprint

FNohamInteractionData INohamInteractable::GetInteractionData_Implementation(AActor* InteractingActor)
{
	// Default interaction data
	FNohamInteractionData Data;
	Data.InteractionName = FText::FromString("Object");
	Data.InteractionAction = FText::FromString("Interact");
	Data.bCanInteract = true;
	Data.InteractionDuration = 0.0f;
	return Data;
}

bool INohamInteractable::OnInteract_Implementation(AActor* InteractingActor)
{
	// Default: interaction succeeds
	return true;
}

void INohamInteractable::OnInteractionFocusGained_Implementation(AActor* InteractingActor)
{
	// Default: no visual feedback (implement in derived classes)
}

void INohamInteractable::OnInteractionFocusLost_Implementation(AActor* InteractingActor)
{
	// Default: no visual feedback (implement in derived classes)
}

FVector INohamInteractable::GetInteractionLocation_Implementation() const
{
	// Default: return actor location if this is an actor
	if (const AActor* Actor = Cast<AActor>(this))
	{
		return Actor->GetActorLocation();
	}
	return FVector::ZeroVector;
}
