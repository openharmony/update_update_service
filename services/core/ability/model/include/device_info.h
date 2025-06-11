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

#ifndef DEVICE_INFO_H
#define DEVICE_INFO_H

#include <string>

#include "anonymous_utils.h"

namespace OHOS {
namespace UpdateService {
struct DeviceInfo {
public:
    std::string udid;
    std::string deviceId;

    friend void ToJson(cJSON *jsonObject, const DeviceInfo &deviceInfo, bool isPrint)
    {
        if (jsonObject == nullptr) {
            return;
        }

        if (isPrint) {
            cJSON_AddStringToObject(jsonObject, "udid", AnonymousUtils::AnonymousString(deviceInfo.udid).c_str());
            cJSON_AddStringToObject(jsonObject, "deviceId", AnonymousUtils::AnonymousString(deviceInfo.udid).c_str());
        } else {
            cJSON_AddStringToObject(jsonObject, "udid", deviceInfo.udid.c_str());
            cJSON_AddStringToObject(jsonObject, "deviceId", deviceInfo.deviceId.c_str());
        }
    }
};
} // namespace UpdateService
} // namespace OHOS
#endif // DEVICE_INFO_H