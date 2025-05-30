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

#ifndef UPDATE_SERVICE_SUBSCRIBE_INFO_H
#define UPDATE_SERVICE_SUBSCRIBE_INFO_H

#include <string>

#include "base_json_struct.h"
#include "business_type.h"
#include "update_device_type.h"

namespace OHOS::UpdateService {
const std::string UPDATE_APP_PACKAGE_NAME = "com.ohos.updateapp";
const std::string UPDATE_APP_SERVICE_EXT_ABILITY_NAME = "ServiceExtAbility";
constexpr int32_t UPDATE_APP_TIMEOUT = 15;
constexpr int32_t UPDATE_APP_CONNECT_TIMEOUT = 5;

struct SubscribeInfo : public BaseJsonStruct {
    std::string upgradeApp = UPDATE_APP_PACKAGE_NAME;
    BusinessType businessType = {};
    std::string abilityName;
    std::string subscriberDevId;
    std::string upgradeDevId;
    DeviceType deviceType = DeviceType::UNKNOWN;
    std::string deviceName;

    explicit SubscribeInfo(BusinessSubType subType)
    {
        businessType.subType = subType;
    }

    SubscribeInfo() = default;

    JsonBuilder GetJsonBuilder() final;
};
} // namespace OHOS::UpdateService
#endif // UPDATE_SERVICE_SUBSCRIBE_INFO_H
