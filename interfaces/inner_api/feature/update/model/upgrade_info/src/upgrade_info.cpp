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

#include "anonymous_utils.h"
#include "upgrade_info.h"
#include "update_define.h"

namespace OHOS::UpdateService {
std::string UpgradeInfo::ToString() const
{
    std::string output = "upgradeApp:" + upgradeApp;
    output += ",businessType(vender:" + businessType.vendor;
    output += ",subType:" + std::to_string(CAST_INT(businessType.subType));
    output += "),upgradeDevId:" + AnonymousUtils::AnonymousString(upgradeDevId);
    output += ",controlDevId:" + AnonymousUtils::AnonymousString(controlDevId);
    return output;
}

bool UpgradeInfo::ReadFromParcel(Parcel &parcel)
{
    upgradeApp = Str16ToStr8(parcel.ReadString16());
    businessType.vendor = Str16ToStr8(parcel.ReadString16());
    businessType.subType = static_cast<BusinessSubType>(parcel.ReadInt32());
    upgradeDevId = Str16ToStr8(parcel.ReadString16());
    controlDevId = Str16ToStr8(parcel.ReadString16());
    processId = parcel.ReadInt32();
    deviceType = static_cast<DeviceType>(parcel.ReadInt32());
    return true;
}

bool UpgradeInfo::Marshalling(Parcel &parcel) const
{
    if (!parcel.WriteString16(Str8ToStr16(upgradeApp))) {
        ENGINE_LOGE("Write upgradeApp failed");
        return false;
    }

    if (!parcel.WriteString16(Str8ToStr16(businessType.vendor))) {
        ENGINE_LOGE("Write businessType vendor failed");
        return false;
    }

    if (!parcel.WriteInt32(static_cast<int32_t>(businessType.subType))) {
        ENGINE_LOGE("Write businessType subType failed");
        return false;
    }

    if (!parcel.WriteString16(Str8ToStr16(upgradeDevId))) {
        ENGINE_LOGE("Write upgradeDevId failed");
        return false;
    }

    if (!parcel.WriteString16(Str8ToStr16(controlDevId))) {
        ENGINE_LOGE("Write controlDevId failed");
        return false;
    }

    if (!parcel.WriteInt32(processId)) {
        ENGINE_LOGE("Write processId failed");
        return false;
    }

    if (!parcel.WriteInt32(static_cast<int32_t>(deviceType))) {
        ENGINE_LOGE("Write deviceType failed");
        return false;
    }
    return true;
}

UpgradeInfo *UpgradeInfo::Unmarshalling(Parcel &parcel)
{
    UpgradeInfo *upgradeInfo = new (std::nothrow) UpgradeInfo();
    if (upgradeInfo == nullptr) {
        ENGINE_LOGE("Create UpgradeInfo failed");
        return nullptr;
    }

    if (!upgradeInfo->ReadFromParcel(parcel)) {
        ENGINE_LOGE("Read from parcel failed");
        delete upgradeInfo;
        return nullptr;
    }
    return upgradeInfo;
}
} // namespace OHOS::UpdateService