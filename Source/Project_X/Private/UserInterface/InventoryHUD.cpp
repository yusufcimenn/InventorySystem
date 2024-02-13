// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/InventoryHUD.h"
#include "UserInterface/MainMenu.h"
#include "UserInterface/Interaction/InteractionWidget.h"
AInventoryHUD::AInventoryHUD()
{
}

void AInventoryHUD::BeginPlay()
{
	Super::BeginPlay();

	if(MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(),MainMenuClass);
		MainMenuWidget -> AddToViewport(5);
		MainMenuWidget ->SetVisibility(ESlateVisibility::Collapsed);
	}
	if(MainMenuClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(),InteractionWidgetClass);
		InteractionWidget -> AddToViewport(-1);
		InteractionWidget ->SetVisibility(ESlateVisibility::Collapsed);
	}
}
void AInventoryHUD::DisplayMenu()
{
	if(MainMenuWidget)
	{
		IsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AInventoryHUD::HideMenu()
{
	if(MainMenuWidget)
	{
		IsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AInventoryHUD::ToggleMenu()
{
	if(IsMenuVisible)
	{
		HideMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayMenu();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void AInventoryHUD::ShowInteractionWidget() const
{
	if(InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AInventoryHUD::HideInteractionWidget() const
{
	if(InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AInventoryHUD::UpdateInterationWidget(const FInteractableData* InteractableData) const
{
	if(InteractionWidget)
	{
		if(InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget -> SetVisibility((ESlateVisibility::Visible));
		}
		 InteractionWidget->UpdateWidget(InteractableData);
	}
}


