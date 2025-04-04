// Copyright by Infinity Branch Studios


#include "MyCommonUIActionRouterBase.h"

#include "Kismet/GameplayStatics.h"


bool FCcCommonAnalogCursor::ShouldVirtualAcceptSimulateMouseButton(const FKeyEvent& InKeyEvent, EInputEvent InputEvent) const
{
	//get the player character
	APlayerController* PC = UGameplayStatics::GetPlayerController(ActionRouter.GetWorld(), ActionRouter.GetLocalPlayerIndex());

	//return whether or not the PlayerContorller is in ui only input mode
	return GetCurrentViewMode(PC) == 1;
}

int FCcCommonAnalogCursor::GetCurrentViewMode(const APlayerController* PlayerController)
{
	if (IsValid(PlayerController))
    {
        UGameViewportClient* GameViewportClient = PlayerController->GetWorld()->GetGameViewport();
        ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

        bool ignore = GameViewportClient->IgnoreInput();
        EMouseCaptureMode capt = GameViewportClient->GetMouseCaptureMode();

        if (ignore == false && capt == EMouseCaptureMode::CaptureDuringMouseDown)
        {
            return 0;  // Game And UI
        }
        
        if (ignore == true && capt == EMouseCaptureMode::NoCapture)
        {
            return 1;  // UI Only
        }

        return 2;  // Game Only
    }

    return -1;
}


TSharedRef<FCommonAnalogCursor> UMyCommonUIActionRouterBase::MakeAnalogCursor() const
{
	return FCommonAnalogCursor::CreateAnalogCursor<FCcCommonAnalogCursor>(*this);
}
