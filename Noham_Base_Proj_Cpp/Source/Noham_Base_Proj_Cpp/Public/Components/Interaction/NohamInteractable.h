// Copyright Noham Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NohamInteractable.generated.h"

/**
 * FNohamInteractionData
 * Data structure containing interaction information
 */
USTRUCT(BlueprintType)
struct FNohamInteractionData
{
	GENERATED_BODY()

	/** Display name of the interactable object */
	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	FText InteractionName = FText::FromString("Interact");

	/** Action text (e.g., "Open", "Pick Up", "Use") */
	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	FText InteractionAction = FText::FromString("Interact");

	/** Optional description or hint */
	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	FText InteractionHint = FText::GetEmpty();

	/** Can the player currently interact (enables/disables prompt) */
	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	bool bCanInteract = true;

	/** Interaction duration in seconds (0 = instant, >0 = hold interaction) */
	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	float InteractionDuration = 0.0f;
};

/**
 * UNohamInteractable
 * UObject interface class for UE5 reflection system
 * This is the class that Blueprint implements
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UNohamInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * INohamInteractable
 * Native C++ interface for interactable objects
 *
 * Implement this interface on actors that players can interact with
 *
 * References:
 * - docs/unreal-engine/core-systems/objects.md - Interface patterns
 */
class NOHAM_BASE_PROJ_CPP_API INohamInteractable
{
	GENERATED_BODY()

public:
	/**
	 * Get interaction data for display to player
	 * @param InteractingActor - The actor attempting to interact (usually the player character)
	 * @return Interaction data for prompts and validation
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FNohamInteractionData GetInteractionData(AActor* InteractingActor);
	virtual FNohamInteractionData GetInteractionData_Implementation(AActor* InteractingActor);

	/**
	 * Called when player begins interacting with this object
	 * @param InteractingActor - The actor performing the interaction
	 * @return true if interaction started successfully
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool OnInteract(AActor* InteractingActor);
	virtual bool OnInteract_Implementation(AActor* InteractingActor);

	/**
	 * Called when player looks at this interactable (for highlight/outline effects)
	 * @param InteractingActor - The actor looking at this object
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteractionFocusGained(AActor* InteractingActor);
	virtual void OnInteractionFocusGained_Implementation(AActor* InteractingActor);

	/**
	 * Called when player stops looking at this interactable
	 * @param InteractingActor - The actor that was looking at this object
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteractionFocusLost(AActor* InteractingActor);
	virtual void OnInteractionFocusLost_Implementation(AActor* InteractingActor);

	/**
	 * Get the interaction location for distance calculation
	 * Default implementation returns actor location
	 * @return World location for interaction distance checks
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FVector GetInteractionLocation() const;
	virtual FVector GetInteractionLocation_Implementation() const;
};
