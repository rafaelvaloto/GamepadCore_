<h1 align="center"><i>GamepadCore_</i></h1>

<p align="center">
Modern, policy-based C++ library for advanced gamepad features (DualSense/DS4). Engine-agnostic architecture designed for Unreal, Unity, Godot, and O3DE.
<br />
<br />

<div align="center">

![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![macOS](https://img.shields.io/badge/macOS-000000?style=for-the-badge&logo=apple&logoColor=white)
![PlayStation](https://img.shields.io/badge/PlayStation-003791?style=for-the-badge&logo=playstation&logoColor=white)

![Godot](https://img.shields.io/badge/Godot-478CBF?style=for-the-badge&logo=godotengine&logoColor=white)
![O3DE](https://img.shields.io/badge/O3DE-FF6D00?style=for-the-badge&logo=op3n&logoColor=white)
![Unreal Engine](https://img.shields.io/badge/Unreal_Engine-313131?style=for-the-badge&logo=unrealengine&logoColor=white)
![Unity](https://img.shields.io/badge/Unity-000000?style=for-the-badge&logo=unity&logoColor=white)


</div>

---

**GamepadCore** is a low-level, flexible, and modular C++ library designed to provide robust gamepad support, initially focusing on the Sony **DualSense** controller and generic gamepads.

The project’s primary goal is to **decouple hardware communication and device registration** from specific game engine logic (such as Unreal Engine or Unity). This separation allows developers to **easily create custom plugins** and integrations for any environment.

### ✨ Key Advantage: Build Your Plugin, Connect Your Engine

**GamepadCore** is built on an adapter-based philosophy (`Policy` and `HardwareInfo`) that empowers you to define *how* the library communicates with your specific development environment:

* **Hardware Abstraction:** The core utilizes interfaces (`IPlatformHardwareInfo`) that allow concrete, OS-specific implementations (Windows, Linux, macOS, etc.) to be injected at runtime.
* **Engine Abstraction:** The device registry (`FDeviceRegistry`) uses a **Policy** system to handle device ID allocation and user management. For example, you can map the library to use `FInputDeviceId` in Unreal Engine, or raw `int` handles in a custom engine.

**This means you can integrate GamepadCore into *any* game engine or C++ application that requires fine-grained control over input devices.**


## 🚀 Key Features

* **⚡ Engine Agnostic**
    * Zero dependencies on specific game engines within the Core logic. The library is built with standard C++ to fit seamlessly into any development environment.

* **🧩 Policy-Based Design**
    * **Registry Policy:** Adapt container types (e.g., `TMap`, `std::map`) and ID types (e.g., `FInputDeviceId`, `int`) to fit your engine's ecosystem.
    * **Hardware Policy:** Inject platform-specific drivers (Windows HID, Linux udev, macOS IOKit, PS5 SDK) without modifying the core library code.

* **🎮 Advanced Support**
    * Native, low-level support for **DualSense** specific features including **Adaptive Triggers**, **Haptic Feedback**, **Motion Sensors (Gyro/Accel)**, and **Lightbar Control**.

* **🏎️ Zero-Cost Abstraction**
    * Heavy use of modern C++ templates ensures that policy resolution happens at **compile-time**, providing maximum performance with no runtime overhead for abstractions.


## 🛠️ Architecture and Structure

The architecture is streamlined into three main pillars to ensure maintainability and scalability:

1.  **`IPlatformHardwareInfo` (The Driver Layer):**
    The raw hardware interface. It receives the concrete OS implementation (e.g., `FLinuxHardware`, `FWindowsHardware`) to handle physical I/O.

2.  **`FDeviceRegistry` (The Manager Layer):**
    Manages the lifecycle of connected controllers. It uses your defined `Policy` to integrate with the specific Engine's user and input system.

3.  **`ISonyGamepad` (The Application Layer):**
    The standardized interfaces your Engine consumes to interact with the gamepad, regardless of the underlying hardware or OS.


## 🛠️ Usage Example (Concept)

**GamepadCore** is designed to be engine-agnostic. Whether you are using a custom engine, a console application, or a commercial framework, the integration flow follows three simple logical steps:

### 1. Setup & Initialization
To start using the library, you must first configure the **Hardware Abstraction Layer**. This involves instantiating the specific adapter for your current operating system (Windows, Linux, or macOS) and injecting it into the core system. Once the hardware layer is set, you initialize the **Device Registry**, which acts as the central hub for managing controller lifecycles and mapping them to your application's user system.

```cpp
#include "Core/Interfaces/IPlatformHardwareInfo.h"
#include "Implementations/Adapters/DeviceRegistry.h"
// Include the concrete implementation for your target OS
#include "Implementations/Platforms/Windows/WindowsHardware.h" 

void InitializeGamepadSystem()
{
    // 1. Create the Platform Hardware Adapter (e.g., Windows, Linux, or Mac)
    std::unique_ptr<IPlatformHardwareInfo> PlatformInstance = 
        std::make_unique<FWindowsPlatform::FWindowsHardware>(); 

    // 2. Inject the hardware instance into the Core system
    IPlatformHardwareInfo::SetInstance(std::move(PlatformInstance));

    // 3. Initialize the Device Registry
    // This establishes the singleton responsible for managing device lifecycles.
    FDeviceRegistry::Initialize();
}
```

### 2. Continuous Device Discovery
The library relies on a periodic update cycle to handle **Plug-and-Play** events efficiently. In your application's main loop (or a dedicated input thread), you must trigger the discovery process. This ensures that the library detects newly connected gamepads, handles disconnections gracefully, and maintains the registry up-to-date without blocking your main application thread.

```cpp
// This function should be called every frame or at a fixed interval
void MyApplication::GameLoop(float DeltaTime)
{
    // Scans for new devices, handles timeouts, and updates connection states.
    FDeviceRegistry::DiscoverDevices(DeltaTime);
    
    // ... Your game logic here
}
```
    

### 3. Interacting with the Gamepad
Once a device is detected and registered, you can retrieve its interface instance using a generic Device ID. This interface (`ISonyGamepad`) gives you direct, low-level access to the controller's features. From here, you can read input data (buttons, analog sticks, sensors) and send output commands (haptics, adaptive triggers, lightbar colors) using a unified API, regardless of the platform running underneath.

```cpp
#include "Core/Interfaces/ISonyGamepad.h"

void HandlePlayerInput(FInputDeviceId DeviceId)
{
    // 1. Retrieve the interface for the specific device ID
    ISonyGamepad* Gamepad = FDeviceRegistry::GetLibraryInstance(DeviceId);

    if (Gamepad && Gamepad->IsConnected())
    {
        // --- INPUT: Reading State ---
        if (Gamepad->GetButtonState(EGamepadButton::Cross))
        {
            Jump();
        }

        // --- OUTPUT: Sending Haptics & Lights ---
        // Set Lightbar to Blue
        Gamepad->SetLightbarColor(0, 0, 255);
        
        // Trigger generic vibration (Left Motor, Right Motor)
        Gamepad->SetVibration(0.8f, 0.2f);

        // Set Adaptive Trigger (e.g., Weapon Recoil on Right Trigger)
        Gamepad->SetTriggerEffect(ETriggerLocation::Right, ETriggerEffectType::Weapon, 0, 8, 2);
    }
}
```

### 🚀 Engine Integration & Platform Examples

While the core logic is generic, handling hardware specifics (like HID communication on Windows or Linux) is complex. **Good news: we have already done the heavy lifting.**

You can find complete, production-ready implementations for **Windows, Linux, and macOS** hardware layers in our example folder. Although the example demonstrates integration with **Unreal Engine**, the platform-specific code (`Implementations/Platforms/...`) is **100% reusable** and can be dropped into your own engine with minimal adaptation.

👉 **Explore the Implementation Reference:**
[https://github.com/rafaelvaloto/GamepadCore_/tree/main/Examples/Unreal](https://github.com/rafaelvaloto/GamepadCore_/tree/main/Examples/Unreal)

```cpp
// 1.
// Initialize PlatformHardware, (e.g., FLinuxHardware FWindowsHardware FMacHardware, FSonyHardware)
std::unique_ptr<IPlatformHardwareInfo> LinuxInstance = std::make_unique<FLinuxPlatform::FLinuxHardware>();
IPlatformHardwareInfo::SetInstance(std::move(LinuxInstance));

// 2.	
// Initialize DeviceResgistry
FDeviceRegistry::Initialize();

// 3.
// Register a new device
std::unique_ptr<FDeviceRegistry::FRegistryLogic> FDeviceRegistry::RegistryImplementation = nullptr;
RegistryImplementation = std::make_unique<FRegistryLogic>();


void FDeviceRegistry::DiscoverDevices(float DeltaTime)
{
	if (RegistryImplementation)
	{
		return RegistryImplementation->PlugAndPlay(DeltaTime);
	}
}

ISonyGamepad* FDeviceRegistry::GetLibraryInstance(FInputDeviceId DeviceId)
{
	if (RegistryImplementation)
	{
		return RegistryImplementation->GetLibrary(DeviceId);
	}
	return nullptr;
}

```

1. Initialize with Injection
GamepadCore uses dependency injection to link the Core with your specific platform/engine environment.
```cpp
   // In your Engine's Startup Module (e.g., Unreal Engine)
   void StartupModule()
   {
   // 1. Inject Hardware Driver (Windows, Linux, or Console)
   IPlatformHardwareInfo::SetInstance(new FWindowsHardwareDriver());

   // 2. Initialize Registry with Engine Policy
   // Here we tell the core to use Unreal's TMap and FInputDeviceId
   Registry = std::make_unique<TBasicDeviceRegistry<FUnrealRegistryPolicy>>();
   }
```

2. The Policy (Your Engine Adapter)
   You only need to define how to store the device. The Core handles when to connect it.
```cpp
// Example implementation of a Policy method
EngineIdType AllocEngineDevice(CoreDeviceId CoreId) 
{
    // The Core detected a physical device! Now we tell the engine.
    int32_t NewId = EngineInputSystem::AddDevice();
    return NewId;
}

void DispatchNewGamepad(CoreDeviceId CoreId) 
{
    // Notify the game via Delegate/Event
    OnControllerConnected.Broadcast();
}

void FreeEngineDevice(CoreDeviceId CoreId, EngineIdType EngineId) 
{
    // Cleanup when unplugged
    EngineInputSystem::RemoveDevice(EngineId);
}
```

## 🧑‍💻 Contributing (Build & sanity checks)

GamepadCore is meant to be consumed **from source** (e.g., compiled inside an engine/plugin build).  
This repository still provides a CMake project so contributors can quickly validate changes locally.

### Requirements
- CMake >= 3.20
- C++20 compiler (Clang/GCC/MSVC)
- Ninja (recommended) or Make

### 1) Configure (one time per build type)

```bash
cmake -S . -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
cmake -S . -B cmake-build-release -DCMAKE_BUILD_TYPE=Release
```

### 2) Format (before pushing)
```bash
cmake --build cmake-build-debug --target GamepadCoreFormat -j
```
### 3) Compile (after any change)

```bash
cmake --build cmake-build-debug --target GamepadCore -j
cmake --build cmake-build-release --target GamepadCore -j
````

### Notes
- Build artifacts (like `libGamepadCore.a`) are generated inside the build folder (e.g. `cmake-build-release/Source/`).
- Contributors generally should **not** commit build directories (`cmake-build-*`).


## 📄 License

![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

This project is licensed under the **MIT License**. See the `LICENSE` file for details.

Copyright (c) 2025 **Rafael Valoto**

---

## ⚖️ Disclaimer and Trademarks

This software is an independent and unofficial project. It is **not** affiliated, associated, authorized, endorsed by, or in any way officially connected with Sony Interactive Entertainment Inc., Microsoft Corporation, Apple Inc., Epic Games, Unity Technologies, the Godot Engine project, or the Open 3D Foundation.

**Trademarks belong to their respective owners:**

* **Sony:** "PlayStation", "PlayStation Family Mark", "PS5 logo", "PS5", "DualSense", and "DualShock" are registered trademarks or trademarks of Sony Interactive Entertainment Inc. "SONY" is a registered trademark of Sony Corporation.
* **Microsoft:** "Windows" and "Xbox" are registered trademarks of Microsoft Corporation.
* **Apple:** "Mac" and "macOS" are registered trademarks of Apple Inc.
* **Linux:** "Linux" is the registered trademark of Linus Torvalds in the U.S. and other countries.
* **Epic Games:** "Unreal" and "Unreal Engine" are trademarks or registered trademarks of Epic Games, Inc. in the United States of America and elsewhere.
* **Unity:** "Unity", Unity logos, and other Unity trademarks are trademarks or registered trademarks of Unity Technologies or its affiliates in the U.S. and elsewhere.
* **Godot:** "Godot" and the Godot logo are trademarks of the Godot Engine project.
* **O3DE:** "O3DE" and the O3DE logo are trademarks of the Open 3D Foundation.
