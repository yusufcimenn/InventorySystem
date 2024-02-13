// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "IDetailTreeNode.h"
#include "Items/ItemBase.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


UItemBase* UInventoryComponent::FindingMatchingItem(UItemBase* ItemIn) const
{
	if(ItemIn)
	{
		if(InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindingNextItemByID(UItemBase* ItemIn) const
{
	if(ItemIn)
	{
		if(const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{
			return *Result;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindingNextPartialStack(UItemBase* ItemIn) const
{
	//lambda kullanımı bunu incele!!!!
	if(const TArray<TObjectPtr<UItemBase>>::ElementType* Result =
		InventoryContents.FindByPredicate([ItemIn](const UItemBase* InventoryItem)
		{
			return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsFullItemStack();
		}  ))
	{
		return *Result;
	}
	return nullptr; 
}
int32 UInventoryComponent::CalculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity()- InventoryTotalWeight)/ ItemIn->GetItemSingleWeight());
	if(WeightMaxAddAmount >= RequestedAmount)
	{
		return RequestedAmount;
	}
	return WeightMaxAddAmount; 
}

int32 UInventoryComponent::CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->ItemNumericData.MaximumStackSize - StackableItem->Quantity;

	return FMath::Min(InitialRequestedAmount,AddAmountToMakeFullStack);
}

void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);
	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);

	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove);
	InventoryTotalWeight -= ActualAmountToRemove * ItemIn->GetItemSingleWeight();
	OnInventoryUpdated.Broadcast();
	
	return ActualAmountToRemove;
}

void UInventoryComponent::SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit)
{
	if(!(InventoryContents.Num() + 1 >InventorySlotsCapacity ))
	{
		RemoveAmountOfItem(ItemIn,AmountToSplit);
		AddNewItem(ItemIn,AmountToSplit);
	}
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(UItemBase* InputItem)
{
	// check if in input item has valid weight
	if( FMath::IsNearlyZero(InputItem -> GetItemSingleWeight()) || InputItem ->GetItemSingleWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0}. Item has invalid weight value."),InputItem->ItemTextData.Name));
	}
	// will the item weight overflow weight capacity
	if(InventoryTotalWeight + InputItem ->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. Item would overflow weight limit."),InputItem->ItemTextData.Name));
	}
	
	// will the item  overflow slot capacity
	if(InventoryContents.Num() +1 > InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. All inventory slots are full."),InputItem->ItemTextData.Name));
	}
	AddNewItem(InputItem,1);
	return FItemAddResult::AddedAll(1,FText::Format(
			FText::FromString("Succeffully added {0}  to the inventory."), InputItem->ItemTextData.Name));
}

int32 UInventoryComponent::HandleStackableItems(UItemBase* ItemIn, int32 RequestedAmount)
{
	if(RequestedAmount <= 0 || FMath::IsNearlyZero(ItemIn->GetItemStackWeight()))
	{
		//invalid item data
		return 0;
	}
	int32 AmountToDistribute = RequestedAmount;

	UItemBase* ExistinItemStack = FindingNextPartialStack(ItemIn);

	//distribute item stack over existing stack
	while(ExistinItemStack)
	{
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistinItemStack,AmountToDistribute);
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistinItemStack,AmountToMakeFullStack);

		if(WeightLimitAddAmount>0)
		{
			ExistinItemStack->SetQuantity(ExistinItemStack->Quantity + WeightLimitAddAmount);
			InventoryTotalWeight += (ExistinItemStack->GetItemSingleWeight()*WeightLimitAddAmount);
			AmountToDistribute -= WeightLimitAddAmount;

			ItemIn->SetQuantity(AmountToDistribute);

			// TODO: REFINE THIS LOGIC SINCE GOIND OVER WEIGHT CAPACITY SHOULD NOT EVER BE POSSIBLE
			if(InventoryTotalWeight >= InventoryWeightCapacity)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAmount - AmountToDistribute;
			}
		}
		else if (WeightLimitAddAmount <= 0)
		{
			if(AmountToDistribute != RequestedAmount)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAmount - AmountToDistribute;
			}
			return 0;
		}
		if(AmountToDistribute <= 0)
		{
			OnInventoryUpdated.Broadcast();
			return RequestedAmount;
		}

		ExistinItemStack = FindingNextPartialStack(ItemIn);
		
	}
	// no more partial stacks found, check if a new stack can be added
	if(InventoryContents.Num() +1 <= InventorySlotsCapacity)
	{
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn,AmountToDistribute);
			

		if(WeightLimitAddAmount > 0)
		{
			if(WeightLimitAddAmount<AmountToDistribute)
			{
				AmountToDistribute -= WeightLimitAddAmount;
				ItemIn ->SetQuantity(AmountToDistribute);

				AddNewItem(ItemIn->CreateItemCopy(),WeightLimitAddAmount);
				return RequestedAmount - AmountToDistribute;
			}
			
			AddNewItem(ItemIn,AmountToDistribute);
			return RequestedAmount;
		}
	}
	OnInventoryUpdated.Broadcast();
	return RequestedAmount - AmountToDistribute;
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem )
{
	if(GetOwner())
	{
		const int32 InitialRequestAddAmount = InputItem -> Quantity;

		// handle non-stackable items
		if(!InputItem->ItemNumericData.IsStackable)
		{
			return HandleNonStackableItems(InputItem); 
		}
		//handle stackable items
		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestAddAmount);
		if(StackableAmountAdded == InitialRequestAddAmount)
		{
			return FItemAddResult::AddedAll(InitialRequestAddAmount,FText::Format(
				FText::FromString("Successfully added {0} {1} to the inventory "),
				InitialRequestAddAmount,
				InputItem->ItemTextData.Name));
		}
		if(StackableAmountAdded < InitialRequestAddAmount && StackableAmountAdded > 0)
		{
			return FItemAddResult::AddedParital(StackableAmountAdded,FText::Format(
				FText::FromString("Partiail amount of  {0}  {1} added  to the inventory "),
				StackableAmountAdded,
				InputItem->ItemTextData.Name));
		}
		if(StackableAmountAdded <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format(
				FText::FromString("Could not add {0} to the inventory. "),
				InputItem->ItemTextData.Name));
		}
	}
	check(false);
	return FItemAddResult::AddedNone(FText::FromString("TryAddItem fallthrough error. GetOwner() check somehow?? failed!!"));
}

void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
	UItemBase* NewItem;

	if(Item->IsCopy || Item->IsPickUp)
	{
		// if the item is already copy, or is world pickup
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		// used when split or dragging to/from another inventory
		NewItem = Item->CreateItemCopy();
	}
	NewItem -> OwningInventory = this;
	NewItem ->SetQuantity(AmountToAdd);

	InventoryContents.Add(NewItem);
	InventoryTotalWeight += NewItem->GetItemStackWeight();
	OnInventoryUpdated.Broadcast();
}


