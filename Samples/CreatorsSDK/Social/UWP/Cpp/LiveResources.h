// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

//
// Handles Users signing in and out and the related Xbox Live Contexts
//
#pragma once

#define MAX_USER 4

namespace ATG
{
    class UIManager;
    class IPanel;

    typedef int32_t function_context;

    class LiveResources
    {
    public:
        LiveResources();

        void Initialize(std::shared_ptr<ATG::UIManager> &ui, ATG::IPanel *userDependantPanel = nullptr, ATG::IPanel *nouserDependantPanel = nullptr);
        //void SignIn();
        void Refresh();

        uint32_t                                           GetTitleId()         const { return m_titleId; }
        std::wstring                                       GetServiceConfigId() const { return m_scid; }

        //std::shared_ptr<xbox::services::system::xbox_live_user> GetUser()       const { return m_user; }

        //function_context add_signin_handler(_In_ std::function<void(std::shared_ptr<xbox::services::system::xbox_live_user>, xbox::services::system::sign_in_status)> handler);

    private:
        //void HandleSignInResult(xbox::services::xbox_live_result<xbox::services::system::sign_in_result> &t);
        void UpdateCurrentUser(int index);

        //std::shared_ptr<xbox::services::system::xbox_live_user> m_user;

        // Title Info
        uint32_t                                           m_titleId;
        std::wstring                                       m_scid;

        uint32_t                                           m_supportedUserCount;

        // UI Elements
        ATG::Legend*                                       m_gamertag[MAX_USER] = {0};
        ATG::Image*                                        m_gamerPic[MAX_USER] = {0};
        ATG::IPanel*                                       m_userDependentPanel[MAX_USER] = {0};
        ATG::IPanel*                                       m_nouserDependentPanel[MAX_USER] = { 0 };;
        ATG::TextLabel*                                    m_sandboxLabel = nullptr;
        ATG::TextLabel*                                    m_titleIdLabel = nullptr;
        ATG::TextLabel*                                    m_scidLabel = nullptr;
        ATG::TextLabel*                                    m_signInErrorLabel = nullptr;

        std::mutex m_writeLock;
        std::unordered_map<function_context, std::function<void(std::shared_ptr<xbox::services::system::xbox_live_user>, xbox::services::system::sign_in_status)>> m_signinRoutedHandlers;
        function_context m_signinRoutedHandlersCounter;
    };
}