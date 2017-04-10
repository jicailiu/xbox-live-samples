// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once
#include <Xinput.h>

class PlayerInput
{
public:
    void OnKeyDown(uint32_t keyEnum);
    void OnKeyUp(uint32_t keyEnum);

    void ClearKeyHash();
    bool IsKeyDown(_In_ uint32_t keyEnum);
    const std::unordered_map<uint32_t, bool>& GetKeys();

private:
    std::unordered_map<uint32_t, bool> m_keyHash;   // maps an enumerated key state
    Concurrency::critical_section m_lock;
    std::wstring m_deviceId;
    std::wstring m_assciatedSystemUserId;
};

ref class InputManager
{
public:
    InputManager();
    ~InputManager();

    void Initialize(Windows::UI::Core::CoreWindow^ window);

    void RegisterKey(_In_ Windows::System::VirtualKey key, _In_ uint32_t keyEnum);
    void ClearKeys();
    void OnKeyDown(_In_ Windows::UI::Core::CoreWindow^ window, _In_ Windows::UI::Core::KeyEventArgs^ args);
    void OnKeyUp(_In_ Windows::UI::Core::CoreWindow^ window, _In_ Windows::UI::Core::KeyEventArgs^ args);

private:
    std::unordered_map<Windows::System::VirtualKey, uint32_t> m_registeredKeys;
    std::unordered_map<std::wstring, PlayerInput> m_playerInputs;   // maps an enumerated key state
};
