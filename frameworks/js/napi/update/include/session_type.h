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

#include <map>
#include <cstdint>
#include <string>

namespace OHOS::UpdateEngine {
class SessionType {
public:
    static constexpr const uint32_t SESSION_CHECK_VERSION = 0;
    static constexpr const uint32_t SESSION_DOWNLOAD = 1;
    static constexpr const uint32_t SESSION_PAUSE_DOWNLOAD = 2;
    static constexpr const uint32_t SESSION_RESUME_DOWNLOAD = 3;
    static constexpr const uint32_t SESSION_UPGRADE = 4;
    static constexpr const uint32_t SESSION_SET_POLICY = 5;
    static constexpr const uint32_t SESSION_GET_POLICY = 6;
    static constexpr const uint32_t SESSION_CLEAR_ERROR = 7;
    static constexpr const uint32_t SESSION_TERMINATE_UPGRADE = 8;
    static constexpr const uint32_t SESSION_GET_NEW_VERSION = 9;
    static constexpr const uint32_t SESSION_GET_NEW_VERSION_DESCRIPTION = 10;
    static constexpr const uint32_t SESSION_SUBSCRIBE = 11;
    static constexpr const uint32_t SESSION_UNSUBSCRIBE = 12;
    static constexpr const uint32_t SESSION_GET_UPDATER = 13;
    static constexpr const uint32_t SESSION_APPLY_NEW_VERSION = 14;
    static constexpr const uint32_t SESSION_FACTORY_RESET = 15;
    static constexpr const uint32_t SESSION_VERIFY_PACKAGE = 16;
    static constexpr const uint32_t SESSION_CANCEL_UPGRADE = 17;
    static constexpr const uint32_t SESSION_GET_CUR_VERSION = 18;
    static constexpr const uint32_t SESSION_GET_CUR_VERSION_DESCRIPTION = 19;
    static constexpr const uint32_t SESSION_GET_TASK_INFO = 20;
    static constexpr const uint32_t SESSION_REPLY_PARAM_ERROR = 21;
    static constexpr const uint32_t SESSION_MAX = 22;

public:
    static std::map<uint32_t, std::string> GetSessionFuncs();

private:
    static std::map<uint32_t, std::string> sessionFuncMap_;
};
} // namespace OHOS::UpdateEngine
#endif // UPDATE_SESSION_TYPE_H
