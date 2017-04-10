// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "pch.h"
#include "Input.h"
#include "PlayerManager.h"

using namespace Windows::Foundation;
using namespace Concurrency;

enum ButtonPress
{
    SignIn,
    LeftGroup,
    RightGroup,
};

InputManager::InputManager()
{

}

InputManager::~InputManager()
{

}

void InputManager::Initialize(Windows::UI::Core::CoreWindow^ window)
{
    window->KeyDown += ref new TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::KeyEventArgs^>(this, &InputManager::OnKeyDown);
    window->KeyUp += ref new TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::KeyEventArgs^>(this, &InputManager::OnKeyUp);

    // Init game input
    RegisterKey(Windows::System::VirtualKey::A, ButtonPress::SignIn);
    RegisterKey(Windows::System::VirtualKey::GamepadA, ButtonPress::SignIn);
    RegisterKey(Windows::System::VirtualKey::Left, ButtonPress::LeftGroup);
    RegisterKey(Windows::System::VirtualKey::GamepadDPadLeft, ButtonPress::LeftGroup);
    RegisterKey(Windows::System::VirtualKey::GamepadLeftThumbstickLeft, ButtonPress::LeftGroup);
    RegisterKey(Windows::System::VirtualKey::Right, ButtonPress::RightGroup);
    RegisterKey(Windows::System::VirtualKey::GamepadDPadRight, ButtonPress::RightGroup);
    RegisterKey(Windows::System::VirtualKey::GamepadLeftThumbstickRight, ButtonPress::RightGroup);
}

void 
InputManager::OnKeyDown(
    _In_ Windows::UI::Core::CoreWindow^ window,
    _In_ Windows::UI::Core::KeyEventArgs^ args
    )
{
    Windows::System::VirtualKey key = args->VirtualKey;
    if (m_registeredKeys.count(key))
    {
        uint32_t registeredKeyEnum = m_registeredKeys[key];
        PlayerManager::instance()->OnKeyDown(args->DeviceId, registeredKeyEnum);
    }
}

void
InputManager::OnKeyUp(
    _In_ Windows::UI::Core::CoreWindow^ window,
    _In_ Windows::UI::Core::KeyEventArgs^ args
)
{
    Windows::System::VirtualKey key = args->VirtualKey;
    if (m_registeredKeys.count(key))
    {
        uint32_t registeredKeyEnum = m_registeredKeys[key];
        PlayerManager::instance()->OnKeyUp(args->DeviceId, registeredKeyEnum);
    }
}

void
InputManager::RegisterKey(
    _In_ Windows::System::VirtualKey key,
    _In_ uint32_t keyEnum
    )
{
    m_registeredKeys[key] = keyEnum;
}

void 
InputManager::ClearKeys()
{
    m_registeredKeys.clear();
}

void
PlayerInput::ClearKeyHash()
{
    critical_section::scoped_lock lock(m_lock);
    m_keyHash.clear();
}

bool 
PlayerInput::IsKeyDown(
    _In_ uint32_t keyEnum
    )
{
    critical_section::scoped_lock lock(m_lock);

    return m_keyHash[keyEnum];
}

const std::unordered_map<uint32_t, bool>&
PlayerInput::GetKeys()
{
    critical_section::scoped_lock lock(m_lock);

    return m_keyHash;
}

void
PlayerInput::OnKeyDown(
    _In_ uint32_t registeredKeyEnum
)
{
    m_keyHash[registeredKeyEnum] = true;
}

void
PlayerInput::OnKeyUp(
    _In_ uint32_t registeredKeyEnum
)
{
    m_keyHash[registeredKeyEnum] = false;
}