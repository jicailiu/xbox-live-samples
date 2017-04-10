// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once
#include "Input.h"

class Player
{
public:
    Player(Windows::System::User^ systemUser)
    {
        m_xboxliveUser = xbox::services::system::xbox_live_user(systemUser);
    }

    const xbox::services::system::xbox_live_user& XboxliveUser() const
    {
        return m_xboxliveUser;
    }

    const std::shared_ptr<GameInput>& Input() const
    {
        return m_input;
    }

    void AttachInput(const std::shared_ptr<GameInput>& input)
    {
        m_input = input;
    }

private:
    xbox::services::system::xbox_live_user m_xboxliveUser;
    std::shared_ptr<GameInput> m_input;
};

class PlayerManager
{
public:
    static std::shared_ptr<PlayerManager> instance();

    PlayerManager();
    void CreateDefaultUser();

    void SetLaunchUser(Windows::System::User^ user);

    static bool IsMultiUserApplication();

    void OnKeyDown(Platform::String^ deviceId, uint32_t keyEnum);
    void OnKeyUp(Platform::String^ deviceId, uint32_t keyEnum);

    std::shared_ptr<GameInput> GetInput(Windows::System::User^ systemUser);
    std::shared_ptr<Player> GetPlayer(uint32 index);

    void Update();


private:
    Windows::System::User^ m_launchUser;

    std::vector<std::shared_ptr<Player>> m_players;

    static std::shared_ptr<PlayerManager> s_instance;
    static std::mutex s_mutex;
};