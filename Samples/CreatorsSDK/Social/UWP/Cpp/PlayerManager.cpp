// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "pch.h"
#include "PlayerManager.h"

using namespace Windows::System;
using namespace Platform;
using namespace xbox::services;
using namespace xbox::services::system;

std::shared_ptr<PlayerManager> PlayerManager::s_instance;
std::mutex PlayerManager::s_mutex;

std::shared_ptr<PlayerManager> PlayerManager::instance()
{
    std::lock_guard<std::mutex> lock(s_mutex);
    if (s_instance == nullptr)
    {
        s_instance = std::make_shared<PlayerManager>();

    }

    return s_instance;
}


PlayerManager::PlayerManager()
{
    xbox_live_user::add_sign_out_completed_handler(
        [this](const xbox::services::system::sign_out_completed_event_args&)
    {
    });

    CreateDefaultUser();
}

void PlayerManager::CreateDefaultUser()
{
    auto defaultPlayer = std::make_shared<Player>(m_launchUser);
    m_players.push_back(defaultPlayer);
}

void PlayerManager::SetLaunchUser(Windows::System::User^ user)
{
    m_launchUser = user;
}

std::shared_ptr<Player> PlayerManager::GetPlayer(uint32 index)
{
    if (index < m_players.size())
    {
        return m_players[index];
    }

    return nullptr;
}

bool PlayerManager::IsMultiUserApplication()
{
    // Save the result in memory, as we only need to check once;
    static int isSupported = -1;

    if (isSupported == -1)
    {
        try
        {
            bool APIExist = Windows::Foundation::Metadata::ApiInformation::IsMethodPresent("Windows.System.UserPicker", "IsSupported");
            isSupported = (APIExist && Windows::System::UserPicker::IsSupported()) ? 1 : 0;
        }
        catch (...)
        {
            isSupported = 0;
        }
    }
    return isSupported == 1;
};

std::shared_ptr<GameInput> PlayerManager::GetInput(Windows::System::User^ systemUser)
{
    if (systemUser == nullptr)
    {
        return nullptr;
    }

    std::shared_ptr<GameInput> input;

    for (const auto& player : m_players)
    {
        if (player->XboxliveUser().windows_system_user() == systemUser)
        {
            if (player->Input() == nullptr)
            {
                input = std::make_shared<GameInput>();
                player->AttachInput(input);
            }
            else
            {
                input = player->Input();
            }
        }
    }

    return input;
}


void PlayerManager::OnKeyDown(String^ deviceId, uint32_t keyEnum)
{
    auto user = UserDeviceAssociation::FindUserFromDeviceId(deviceId);
    auto input = GetInput(user);

    if (input != nullptr)
    {
        input->OnKeyDown(keyEnum);
    }
}

void PlayerManager::OnKeyUp(String^ deviceId, uint32_t keyEnum)
{
    auto user = UserDeviceAssociation::FindUserFromDeviceId(deviceId);
    auto input = GetInput(user);

    if (input != nullptr)
    {
        input->OnKeyUp(keyEnum);
    }
}

void PlayerManager::Update()
{
    for (auto player: m_players)
    {
        //auto player = playerPair.second;
        if (player->Input() != nullptr)
        {
            /*if (player.Input()->IsKeyDown(ButtonPress::SignIn))
            {
                player.SignIn();
            }
            auto toggleRight = false;
            auto toggleLeft = false;

            if (player.Input()->IsKeyDown(ButtonPress::LeftGroup))
            {
                toggleLeft = true;
            }

            if (player.Input()->IsKeyDown(ButtonPress::RightGroup))
            {
                toggleRight = true;
            }

            if (toggleLeft)
            {
                int newVal = ((int)m_selectedFriendList) - 1;

                if (newVal < 0)
                {
                    newVal = 3;
                }

                m_selectedFriendList = (friendListType)newVal;

                std::lock_guard<std::mutex> guard(m_socialManagerLock);
                RefreshUserList();
            }

            if (toggleRight)
            {
                int newVal = ((int)m_selectedFriendList) + 1;

                if (newVal > 3)
                {
                    newVal = 0;
                }

                m_selectedFriendList = (friendListType)newVal;

                std::lock_guard<std::mutex> guard(m_socialManagerLock);
                RefreshUserList();
            }*/
        }
    }

    
}