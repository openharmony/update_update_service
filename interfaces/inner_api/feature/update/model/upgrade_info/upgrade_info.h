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
#include "device_type.h"

namespace OHOS::UpdateEngine {
const std::string LOCAL_UPGRADE_INFO = "LocalUpgradeInfo";
struct UpgradeInfo {
    std::string upgradeApp;
    BusinessType businessType;
    std::string upgradeDevId;
    std::string controlDevId;
    int32_t processId;
    DeviceType deviceType;

    bool operator<(const UpgradeInfo &r) const
    {
        if (upgradeApp < r.upgradeApp) return true;
        if (upgradeApp > r.upgradeApp) return false;

        if (businessType < r.businessType) return true;
        if (r.businessType < businessType) return false;

        if (upgradeDevId < r.upgradeDevId) return true;
        if (upgradeDevId > r.upgradeDevId) return false;

        if (controlDevId < r.controlDevId) return true;
        if (controlDevId > r.controlDevId) return false;

        if (processId < r.processId) return true;
        if (processId > r.processId) return false;

        if (deviceType < r.deviceType) return true;
        if (deviceType > r.deviceType) return false;

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
