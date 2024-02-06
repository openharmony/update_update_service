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

#ifndef UPDATE_SERVICE_UPGRADE_INFO_H
#define UPDATE_SERVICE_UPGRADE_INFO_H

#include <string>

#include "business_type.h"
#include "update_device_type.h"

namespace OHOS::UpdateEngine {
const std::string LOCAL_UPGRADE_INFO = "LocalUpgradeInfo";
struct UpgradeInfo {
    std::string upgradeApp;
    BusinessType businessType = {};
    std::string upgradeDevId;
    std::string controlDevId;
    int32_t processId;
    DeviceType deviceType = DeviceType::UNKNOWN;

    bool operator<(const UpgradeInfo &other) const
    {
        if (upgradeApp != other.upgradeApp) {
            return upgradeApp < other.upgradeApp;
        }
        
        if (businessType != other.businessType) {
            return businessType < other.businessType;
        }

        if (upgradeDevId != other.upgradeDevId) {
            return upgradeDevId < other.upgradeDevId;
        }

        if (controlDevId != other.controlDevId) {
            return controlDevId < other.controlDevId;
        }

        if (processId != other.processId) {
            return processId < other.processId;
        }

        if (deviceType != other.deviceType) {
            return deviceType < other.deviceType;
        }

        return false;
    }

    std::string ToString() const;

    bool IsLocal() const
    {
        return upgradeApp == LOCAL_UPGRADE_INFO;
    }
};
} // namespace OHOS::UpdateEngine
#endif // UPDATE_SERVICE_UPGRADE_INFO_H
