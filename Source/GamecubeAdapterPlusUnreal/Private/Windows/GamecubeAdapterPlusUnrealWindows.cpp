// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Windows/GamecubeAdapterPlusUnrealWindows.h"

FGamecubeAdapterPlusUnrealWindows::FGamecubeAdapterPlusUnrealWindows(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
	: FGamecubeAdapterPlusUnreal(InMessageHandler)
{
	//FWindowsApplication* WindowsApplication = (FWindowsApplication*)FSlateApplication::Get().GetPlatformApplication().Get();
	//check(WindowsApplication);

	//WindowsApplication->AddMessageHandler(*this);
};

bool FGamecubeAdapterPlusUnrealWindows::ProcessMessage(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam, int32& OutResult)
{
	bool bHandled = false;
	if (msg == WM_DEVICECHANGE)
	{
		DeviceChanged();
		bHandled = true;
	}
	return bHandled;
}