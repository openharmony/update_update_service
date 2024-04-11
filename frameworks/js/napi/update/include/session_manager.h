/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef UPDATE_SESSION_MGR_H
#define UPDATE_SESSION_MGR_H

#include <map>
#include <memory>
#include <mutex>
#include <string>

#include "js_native_api_types.h"

#include "base_session.h"
#include "event_classify_info.h"
#include "event_handler.h"
#include "event_info.h"

namespace OHOS::UpdateEngine {
class SessionManager {
public:
    SessionManager(napi_env env, napi_ref thisReference);
    ~SessionManager();

    void AddSession(std::shared_ptr<BaseSession> session);
    void RemoveSession(uint32_t sessionId);

    int32_t ProcessUnsubscribe(const std::string &eventType, size_t argc, napi_value arg);
    void Unsubscribe(const EventClassifyInfo &eventClassifyInfo, napi_value handle);

    BaseSession *FindSessionByHandle(napi_env env, const std::string &eventType, napi_value arg);
    BaseSession *FindSessionByHandle(napi_env env, const EventClassifyInfo &eventClassifyInfo, napi_value arg);

    void Emit(const EventClassifyInfo &eventClassifyInfo, const EventInfo &eventInfo);

private:
    void PublishToJS(const EventClassifyInfo &eventClassifyInfo, const EventInfo &eventInfo);
    bool GetFirstSessionId(uint32_t &sessionId);
    bool GetNextSessionId(uint32_t &sessionId);

private:
    napi_env env_ {};
    napi_ref thisReference_ {};
    std::map<uint32_t, std::shared_ptr<BaseSession>> sessions_;
    std::recursive_mutex sessionMutex_;
    std::shared_ptr<AppExecFwk::EventHandler> handler_;
};
} // namespace OHOS::UpdateEngine
#endif // UPDATE_SESSION_MGR_H