// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h" 
#include <Windows/WindowsApplication.h>
#include <GamecubeAdapterPlusUnreal.h>

class FGamecubeAdapterPlusUnrealWindows : public FGamecubeAdapterPlusUnreal//, IWindowsMessageHandler
{
public:
	FGamecubeAdapterPlusUnrealWindows(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler);

	virtual ~FGamecubeAdapterPlusUnrealWindows() {}

	virtual bool ProcessMessage(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam, int32& OutResult);
};