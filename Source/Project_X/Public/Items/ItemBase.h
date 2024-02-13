// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/ItemDataStructs.h"
#include "Project_X/Project_XCharacter.h"
#include "ItemBase.generated.h"

class UInventoryComponent;

UCLASS()
class PROJECT_X_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UInventoryComponent* OwningInventory;
	
	UPROPERTY(VisibleAnywhere , Category="Item")
	FName ID;
	
	UPROPERTY(VisibleAnywhere, Category="Item")
	int32 Quantity;
	
	UPROPERTY(VisibleAnywhere , Category="Item")
	EItemType ItemType;
	
	UPROPERTY(VisibleAnywhere , Category="Item")
	EItemQuality ItemQuality;
	
	UPROPERTY(VisibleAnywhere , Category="Item")
	FItemStatistics ItemStatistics;
	
	UPROPERTY(VisibleAnywhere , Category="Item")
	FItemAssetData ItemAssetData;
	
	UPROPERTY(VisibleAnywhere , Category="Item")
	FItemTextData ItemTextData;
	
	UPROPERTY(VisibleAnywhere , Category="Item")
	FItemNumericData ItemNumericData;

	bool IsCopy;
	bool IsPickUp;

	UItemBase();

	void ResetItemFlags();
	
	UFUNCTION(Category="Item")
	UItemBase* CreateItemCopy() const;

	UFUNCTION(Category="Item")
	FORCEINLINE float  GetItemStackWeight() const {return Quantity*ItemNumericData.Weight;};

	UFUNCTION(Category="Item")
	FORCEINLINE float GetItemSingleWeight() const {return ItemNumericData.Weight;};

	UFUNCTION(Category="Item")
	FORCEINLINE bool IsFullItemStack() const {return Quantity == ItemNumericData.MaximumStackSize;};

	UFUNCTION(Category="Item")
	void SetQuantity (const int32 NewQuantity);

	UFUNCTION(Category="Item")
	virtual void Use(AProject_XCharacter* Character);	

protected:
	bool operator == (const FName& OtherID) const
	{
		return this -> ID == OtherID;
	}
	
};
