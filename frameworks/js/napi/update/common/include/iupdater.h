/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef UPDATER_INTERFACE_H
#define UPDATER_INTERFACE_H

#include <memory>

#include "base_client.h"
#include "base_session.h"
#include "client_helper.h"
#include "session_manager.h"
#include "update_result.h"

namespace OHOS::UpdateEngine {
class IUpdater : public BaseClient {
public:
    virtual ~IUpdater() {}

    napi_value On(napi_env env, napi_callback_info info);
    napi_value Off(napi_env env, napi_callback_info info);
    virtual void GetUpdateResult(uint32_t type, UpdateResult &result);

protected:
    virtual void RegisterCallback() {}
    virtual void UnRegisterCallback() {}

    void RemoveSession(uint32_t sessionId) override
    {
        if (sessionsMgr_ == nullptr) {
            return;
        }
        sessionsMgr_->RemoveSession(sessionId);
    }

    napi_value StartSession(napi_env env, napi_callback_info info, SessionParams &sessionParams,
        BaseSession::DoWorkFunction function);
    napi_value StartParamErrorSession(napi_env env, napi_callback_info info, CALLBACK_POSITION callbackPosition);
    void NotifyEventInfo(const EventInfo &eventInfo);

protected:
    std::shared_ptr<SessionManager> sessionsMgr_ = nullptr;
};
} // namespace OHOS::UpdateEngine
#endif // UPDATER_INTERFACE_H