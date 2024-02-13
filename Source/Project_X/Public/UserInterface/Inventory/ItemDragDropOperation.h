// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

class UInventoryComponent;
class UItemBase;
/**
 * 
 */
UCLASS()
class PROJECT_X_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UItemBase* SourceItem;
	UPROPERTY()
	UInventoryComponent* SourceInventory;
};
