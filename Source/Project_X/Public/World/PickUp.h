// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "PickUp.generated.h"

class UItemBase;

UCLASS()
class PROJECT_X_API APickUp : public AActor , public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	
	APickUp();

	void InitializePickUp(const TSubclassOf<UItemBase> BaseClass,const int InQuantity);
	void InitializeDrop(UItemBase* ItemToDrop,const int32 InQuantity);

	FORCEINLINE UItemBase* GetItemData(){return ItemReference;}

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	
protected:
	UPROPERTY(VisibleAnywhere, Category="PickUp | Components")
	UStaticMeshComponent* PickUpMesh;

	UPROPERTY(EditInstanceOnly, Category="PickUp | Item Initialization")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category="PickUp | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category="PickUp | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly, Category="PickUp | Item Initialization")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category="PickUp | Interaction")
	FInteractableData InstanceInteractableData;
	
	virtual void BeginPlay() override;
	virtual void Interact(AProject_XCharacter* PlayerCharacter) override;
	void UpdateInteractableData();
	void TakePickUp(const AProject_XCharacter* Taker);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};


