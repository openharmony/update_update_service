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

#ifndef NAPI_STRUCTS_BASE_H
#define NAPI_STRUCTS_BASE_H

#include "napi_common_define.h"

#include <functional>

#include "update_helper.h"

namespace OHOS::UpdateEngine {
struct SessionParams {
    SessionType type;
    size_t callbackStartIndex;
    bool isNeedBusinessError;
    bool isAsyncCompleteWork;

    SessionParams(SessionType typeValue = SessionType::SESSION_MAX, size_t callbackPosition = CALLBACK_POSITION_ONE,
        bool isNeedBusinessErrorValue = false, bool isAsyncCompleteWorkValue = false)
        : type(typeValue),
        callbackStartIndex(INDEX(callbackPosition)),
        isNeedBusinessError(isNeedBusinessErrorValue),
        isAsyncCompleteWork(isAsyncCompleteWorkValue) {}
};

struct NapiResult {
    SessionType type;
    BusinessError businessError;

    template <typename T>
    void AssignValue(T *const source, T *&target)
    {
        if (target == nullptr) {
            target = new (std::nothrow) T();
        }
        if ((target != nullptr) && (source != nullptr)) {
            *(target) = *(source);
        }
    }

    template <typename T>
    void ReleaseValue(T *&obj)
    {
        delete obj;
        obj = nullptr;
    }
};

const struct UpdateFuncMap {
    SessionType type;
    std::string func;
} SESSION_FUNC_MAP[] = {
    { SessionType::SESSION_CHECK_VERSION,               "checkNewVersion" },
    { SessionType::SESSION_DOWNLOAD,                    "download" },
    { SessionType::SESSION_PAUSE_DOWNLOAD,              "pauseDownload" },
    { SessionType::SESSION_RESUME_DOWNLOAD,             "resumeDownload" },
    { SessionType::SESSION_UPGRADE,                     "upgrade" },
    { SessionType::SESSION_SET_POLICY,                  "setUpgradePolicy" },
    { SessionType::SESSION_GET_POLICY,                  "getUpgradePolicy" },
    { SessionType::SESSION_CLEAR_ERROR,                 "clearError" },
    { SessionType::SESSION_TERMINATE_UPGRADE,           "terminateUpgrade" },
    { SessionType::SESSION_GET_NEW_VERSION,             "getNewVersionInfo" },
    { SessionType::SESSION_GET_NEW_VERSION_DESCRIPTION, "getNewVersionDescription" },
    { SessionType::SESSION_SUBSCRIBE,                   "subscribe" },
    { SessionType::SESSION_UNSUBSCRIBE,                 "unsubscribe" },
    { SessionType::SESSION_GET_UPDATER,                 "getUpdater" },
    { SessionType::SESSION_APPLY_NEW_VERSION,           "applyNewVersion" },
    { SessionType::SESSION_FACTORY_RESET,               "factoryReset" },
    { SessionType::SESSION_VERIFY_PACKAGE,              "verifyPackage" },
    { SessionType::SESSION_CANCEL_UPGRADE,              "cancel" },
    { SessionType::SESSION_GET_CUR_VERSION,             "getCurrentVersionInfo" },
    { SessionType::SESSION_GET_CUR_VERSION_DESCRIPTION, "getCurrentVersionDescription" },
    { SessionType::SESSION_GET_TASK_INFO,               "getTaskInfo" },
    { SessionType::SESSION_REPLY_PARAM_ERROR,           "replyParamError" },
    { SessionType::SESSION_MAX,                         "max" }
};
} // namespace OHOS::UpdateEngine
#endif // NAPI_STRUCTS_BASE_H