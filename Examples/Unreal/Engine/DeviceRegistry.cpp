// Copyright (c) 2025 Rafael Valoto/Publisher. All rights reserved.
// Created for: WindowsDualsense_ds5w - Plugin to support DualSense controller on Windows.
// Planned Release Year: 2025

#include "Implementations/DeviceRegistry.h"
#include "GamepadCore/Source/Public/Core/Interfaces/ISonyGamepad.h"
#include "HAL/PlatformProcess.h"
#include "Runtime/Launch/Resources/Version.h"

TUniquePtr<FDeviceRegistry> FDeviceRegistry::Instance;
std::unique_ptr<FRegistryLogic> FDeviceRegistry::RegistryImplementation;

FDeviceRegistry::~FDeviceRegistry()
{
}

void FDeviceRegistry::Initialize()
{
	if (!Instance)
	{
		Instance.Reset(new FDeviceRegistry());
	}
}

void FDeviceRegistry::Shutdown()
{
	Instance.Reset();
}

FDeviceRegistry* FDeviceRegistry::Get()
{
	check(IsInGameThread());
	return Instance.Get();
}

ISonyGamepad* FDeviceRegistry::GetLibraryInstance(FInputDeviceId DeviceId)
{
	if (RegistryImplementation)
	{
		return RegistryImplementation->GetLibrary(DeviceId);
	}
	return nullptr;
}
