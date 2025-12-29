// Copyright (c) 2025 Rafael Valoto. All Rights Reserved.
// Project: GamepadCore
// Description: Cross-platform library for DualSense and generic gamepad input support.
// Targets: Windows, Linux, macOS.
#pragma once
#include "test_linux_device_info.h"
#include "GCore/Templates/TGenericHardwareInfo.h"


// Sample Linux hardware policy adapter template
//
// This example satisfies the `IsHardwarePolicy` concept used by
// `GamepadCore::TGenericHardwareInfo`. Replace the bodies with calls to your
// concrete Linux implementation in
// (e.g., forward to your FCommonsDeviceInfo logic that uses SDL HID).
namespace Ftest_linux_platform
{
	struct Ftest_linux_hardware_policy;
	using Ftest_linux_hardware = GamepadCore::TGenericHardwareInfo<Ftest_linux_hardware_policy>;
	
    struct Ftest_linux_hardware_policy
    {
        Ftest_linux_hardware_policy() = default;

        void Read(FDeviceContext* Context)
        {
			Ftest_linux_device_info::Read(Context);
        }

        void Write(FDeviceContext* Context)
        {
        	Ftest_linux_device_info::Write(Context);
        }

        void Detect(std::vector<FDeviceContext>& Devices)
        {
        	Ftest_linux_device_info::Detect(Devices);
        }

        bool CreateHandle(FDeviceContext* Context)
        {
        	return Ftest_linux_device_info::CreateHandle(Context);
        }

        void InvalidateHandle(FDeviceContext* Context)
        {
        	Ftest_linux_device_info::InvalidateHandle(Context);
        }

        void ProcessAudioHaptic(FDeviceContext* Context)
        {
        	Ftest_linux_device_info::ProcessAudioHapitc(Context);
        }
    };
}
