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

#ifndef UPDATE_SESSION_TYPE_H
#define UPDATE_SESSION_TYPE_H

#include <cstdint>
#include <map>
#include <string>

namespace OHOS::UpdateEngine {
namespace SessionType {
    constexpr uint32_t SESSION_CHECK_VERSION = 0;
    constexpr uint32_t SESSION_DOWNLOAD = 1;
    constexpr uint32_t SESSION_PAUSE_DOWNLOAD = 2;
    constexpr uint32_t SESSION_RESUME_DOWNLOAD = 3;
    constexpr uint32_t SESSION_UPGRADE = 4;
    constexpr uint32_t SESSION_SET_POLICY = 5;
    constexpr uint32_t SESSION_GET_POLICY = 6;
    constexpr uint32_t SESSION_CLEAR_ERROR = 7;
    constexpr uint32_t SESSION_TERMINATE_UPGRADE = 8;
    constexpr uint32_t SESSION_GET_NEW_VERSION = 9;
    constexpr uint32_t SESSION_GET_NEW_VERSION_DESCRIPTION = 10;
    constexpr uint32_t SESSION_SUBSCRIBE = 11;
    constexpr uint32_t SESSION_UNSUBSCRIBE = 12;
    constexpr uint32_t SESSION_GET_UPDATER = 13;
    constexpr uint32_t SESSION_APPLY_NEW_VERSION = 14;
    constexpr uint32_t SESSION_FACTORY_RESET = 15;
    constexpr uint32_t SESSION_VERIFY_PACKAGE = 16;
    constexpr uint32_t SESSION_CANCEL_UPGRADE = 17;
    constexpr uint32_t SESSION_GET_CUR_VERSION = 18;
    constexpr uint32_t SESSION_GET_CUR_VERSION_DESCRIPTION = 19;
    constexpr uint32_t SESSION_GET_TASK_INFO = 20;
    constexpr uint32_t SESSION_REPLY_PARAM_ERROR = 21;
    constexpr uint32_t SESSION_MAX = UINT32_MAX;
} // namespace OHOS::UpdateEngine::SessionType

const std::map<uint32_t, std::string> SESSION_FUNC_MAP = {
    {SessionType::SESSION_CHECK_VERSION,               "checkNewVersion"},
    {SessionType::SESSION_DOWNLOAD,                    "download"},
    {SessionType::SESSION_PAUSE_DOWNLOAD,              "pauseDownload"},
    {SessionType::SESSION_RESUME_DOWNLOAD,             "resumeDownload"},
    {SessionType::SESSION_UPGRADE,                     "upgrade"},
    {SessionType::SESSION_SET_POLICY,                  "setUpgradePolicy"},
    {SessionType::SESSION_GET_POLICY,                  "getUpgradePolicy"},
    {SessionType::SESSION_CLEAR_ERROR,                 "clearError"},
    {SessionType::SESSION_TERMINATE_UPGRADE,           "terminateUpgrade"},
    {SessionType::SESSION_GET_NEW_VERSION,             "getNewVersionInfo"},
    {SessionType::SESSION_GET_NEW_VERSION_DESCRIPTION, "getNewVersionDescription"},
    {SessionType::SESSION_SUBSCRIBE,                   "subscribe"},
    {SessionType::SESSION_UNSUBSCRIBE,                 "unsubscribe"},
    {SessionType::SESSION_GET_UPDATER,                 "getUpdater"},
    {SessionType::SESSION_APPLY_NEW_VERSION,           "applyNewVersion"},
    {SessionType::SESSION_FACTORY_RESET,               "factoryReset"},
    {SessionType::SESSION_VERIFY_PACKAGE,              "verifyPackage"},
    {SessionType::SESSION_CANCEL_UPGRADE,              "cancel"},
    {SessionType::SESSION_GET_CUR_VERSION,             "getCurrentVersionInfo"},
    {SessionType::SESSION_GET_CUR_VERSION_DESCRIPTION, "getCurrentVersionDescription"},
    {SessionType::SESSION_GET_TASK_INFO,               "getTaskInfo"},
    {SessionType::SESSION_REPLY_PARAM_ERROR,           "replyParamError"},
    {SessionType::SESSION_MAX,                         "max"}
};
} // namespace OHOS::UpdateEngine
#endif // UPDATE_SESSION_TYPE_H
