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

#include "session_type.h"

namespace OHOS::UpdateEngine {
std::map<uint32_t, std::string> SessionType::sessionFuncMap_ = {
    {SESSION_CHECK_VERSION,               "checkNewVersion"},
    {SESSION_DOWNLOAD,                    "download"},
    {SESSION_PAUSE_DOWNLOAD,              "pauseDownload"},
    {SESSION_RESUME_DOWNLOAD,             "resumeDownload"},
    {SESSION_UPGRADE,                     "upgrade"},
    {SESSION_SET_POLICY,                  "setUpgradePolicy"},
    {SESSION_GET_POLICY,                  "getUpgradePolicy"},
    {SESSION_CLEAR_ERROR,                 "clearError"},
    {SESSION_TERMINATE_UPGRADE,           "terminateUpgrade"},
    {SESSION_GET_NEW_VERSION,             "getNewVersionInfo"},
    {SESSION_GET_NEW_VERSION_DESCRIPTION, "getNewVersionDescription"},
    {SESSION_SUBSCRIBE,                   "subscribe"},
    {SESSION_UNSUBSCRIBE,                 "unsubscribe"},
    {SESSION_GET_UPDATER,                 "getUpdater"},
    {SESSION_APPLY_NEW_VERSION,           "applyNewVersion"},
    {SESSION_FACTORY_RESET,               "factoryReset"},
    {SESSION_VERIFY_PACKAGE,              "verifyPackage"},
    {SESSION_CANCEL_UPGRADE,              "cancel"},
    {SESSION_GET_CUR_VERSION,             "getCurrentVersionInfo"},
    {SESSION_GET_CUR_VERSION_DESCRIPTION, "getCurrentVersionDescription"},
    {SESSION_GET_TASK_INFO,               "getTaskInfo"},
    {SESSION_REPLY_PARAM_ERROR,           "replyParamError"},
    {SESSION_MAX,                         "max"}
};

std::map<uint32_t, std::string> SessionType::GetSessionFuncs()
{
    return sessionFuncMap_;
}
} // namespace OHOS::UpdateEngine