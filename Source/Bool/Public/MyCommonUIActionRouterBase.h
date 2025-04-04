// Copyright by Infinity Branch Studios

#pragma once

#include "CoreMinimal.h"
#include "Input/CommonUIActionRouterBase.h"
#include <Input/CommonAnalogCursor.h>
#include "MyCommonUIActionRouterBase.generated.h"

//from https://forums.unrealengine.com/t/why-does-left-mouse-click-register-when-i-press-gamepad-face-button-bottom-on-gamepad/1809814/3
class FCcCommonAnalogCursor : public FCommonAnalogCursor
{
public:
	FCcCommonAnalogCursor(const UCommonUIActionRouterBase& InActionRouter) : FCommonAnalogCursor(InActionRouter) {}

public:
	bool ShouldVirtualAcceptSimulateMouseButton(const FKeyEvent& InKeyEvent, EInputEvent InputEvent) const override;
	static int GetCurrentViewMode(const APlayerController *PlayerController);
};

/**
 * 
 */
UCLASS()
class BOOL_API UMyCommonUIActionRouterBase : public UCommonUIActionRouterBase
{
	GENERATED_BODY()

	TSharedRef<FCommonAnalogCursor> MakeAnalogCursor() const override;
	
};
