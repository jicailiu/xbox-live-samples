// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "pch.h"

#include "SampleGUI.h"
#include "LiveResources.h"
#include "PlayerManager.h"

#include <robuffer.h>

using namespace Windows::Storage::Streams;
using Microsoft::WRL::ComPtr;

ATG::LiveResources::LiveResources()
{
    m_supportedUserCount = PlayerManager::IsMultiUserApplication()? MAX_USER : 1;
}

void ATG::LiveResources::Initialize(std::shared_ptr<ATG::UIManager> &ui, ATG::IPanel *userDependentPanel, ATG::IPanel *nouserDependentPanel)
{
    ui->LoadLayout(L".\\Assets\\LiveInfoHUD.csv", L".\\Assets");

    for (int i = 0; i < MAX_USER; i++)
    {
        m_gamertag[i] = ui->FindControl<ATG::Legend>(1000, 1002+i*2);
        m_gamerPic[i] = ui->FindControl<ATG::Image>(1000, 1001+i*2);

        m_userDependentPanel[i] = userDependentPanel;
        m_nouserDependentPanel[i] = nouserDependentPanel;
    }

    m_sandboxLabel = ui->FindControl<ATG::TextLabel>(1000, 1011);
    m_titleIdLabel = ui->FindControl<ATG::TextLabel>(1000, 1013);
    m_scidLabel = ui->FindControl<ATG::TextLabel>(1000, 1015);
    m_signInErrorLabel = ui->FindControl<ATG::TextLabel>(1000, 1009);
    m_signInErrorLabel->SetVisible(false);

    ui->FindPanel<ATG::HUD>(1000)->Show();

    Refresh();
}

void ATG::LiveResources::Refresh()
{
    auto appConfig = xbox::services::xbox_live_app_config::get_app_config_singleton();

    m_titleId = appConfig->title_id();
    m_scid = appConfig->scid();

    if (!appConfig->sandbox().empty())
    {
        m_sandboxLabel->SetText(appConfig->sandbox().c_str());
    }

    wchar_t hexTitleId[16] = {};
    swprintf_s(hexTitleId, L"0x%08X", m_titleId);

    m_titleIdLabel->SetText(hexTitleId);
    m_scidLabel->SetText(m_scid.c_str());

}

//void ATG::LiveResources::SignIn()
//{
//    m_user->signin(Windows::UI::Core::CoreWindow::GetForCurrentThread()->Dispatcher)
//    .then([this](xbox::services::xbox_live_result<xbox::services::system::sign_in_result> result) // use task_continuation_context::use_current() to make the continuation task running in current apartment 
//    {
//        HandleSignInResult(result);
//
//    }, concurrency::task_continuation_context::use_current());
//}

//void ATG::LiveResources::HandleSignInResult(
//    xbox::services::xbox_live_result<xbox::services::system::sign_in_result>& signInResult
//    )
//{
//    if (!signInResult.err())
//    {
//        auto result = signInResult.payload();
//        _Raise_service_call_routed_event(m_user, result.status());
//        switch (result.status())
//        {
//        case xbox::services::system::sign_in_status::success:
//            Refresh();
//            break;
//
//        case xbox::services::system::sign_in_status::user_cancel:
//            m_signInErrorLabel->SetText(L"Error: User canceled");
//            m_signInErrorLabel->SetVisible(true);
//            UpdateCurrentUser();
//            break;
//
//        case xbox::services::system::sign_in_status::user_interaction_required:
//            m_signInErrorLabel->SetText(L"Error: User interaction required");
//            m_signInErrorLabel->SetVisible(true);
//            UpdateCurrentUser();
//            break;
//        }
//    }
//    else
//    {
//        string_t errorStr = L"Sign in failed:" + utility::conversions::utf8_to_utf16(signInResult.err_message());
//        m_signInErrorLabel->SetText(errorStr.c_str());
//        m_signInErrorLabel->SetVisible(true);
//        UpdateCurrentUser();
//    }
//}

void ATG::LiveResources::UpdateCurrentUser(int index)
{
    auto player = PlayerManager::instance()->GetPlayer(index);
    if (player!= nullptr && player->XboxliveUser().is_signed_in())
    {
        if (m_nouserDependentPanel[index] != nullptr)
        {
            m_nouserDependentPanel[index]->Close();
        }

        if (m_userDependentPanel[index] != nullptr)
        {
            m_userDependentPanel[index]->Show();
        }

        m_gamertag[index]->SetText(player->XboxliveUser().gamertag().c_str());
        m_signInErrorLabel->SetVisible(false);
    }
    else
    {
        if (m_userDependentPanel[index] != nullptr)
        {
            m_userDependentPanel[index]->Close();
        }

        if (m_nouserDependentPanel[index] != nullptr)
        {
            m_nouserDependentPanel[index]->Show();
        }

        m_gamertag[index]->SetText(L"Press [A] to sign in");
    }
}

//function_context ATG::LiveResources::add_signin_handler(
//    _In_ std::function<void(std::shared_ptr<xbox::services::system::xbox_live_user>, xbox::services::system::sign_in_status)> handler
//    )
//{
//    std::lock_guard<std::mutex> lock(m_writeLock);
//
//    function_context context = -1;
//    if (handler != nullptr)
//    {
//        context = ++m_signinRoutedHandlersCounter;
//        m_signinRoutedHandlers[m_signinRoutedHandlersCounter] = std::move(handler);
//    }
//
//    return context;
//}
//


