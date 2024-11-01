#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
    GENERATED_BODY()
};

/**
 * Interface for interactable actors.
 */
class PTEST_API IInteractable
{
    GENERATED_BODY()

public:
    // Define interaction behavior
    virtual void Interact(AActor* Interactor) = 0;
};
