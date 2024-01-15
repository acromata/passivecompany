#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemsDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PASSIVECOMPANY_API UItemsDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ScrapValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTwoHanded;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* ItemMesh;
};
