/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#include "upgrade_policy.h"

#include "parcel_common.h"
#include "update_define.h"

namespace OHOS::UpdateService {
bool UpgradePolicy::ReadFromParcel(Parcel &parcel)
{
    downloadStrategy = static_cast<bool>(parcel.ReadBool());
    autoUpgradeStrategy = static_cast<bool>(parcel.ReadBool());
    customPolicyType = static_cast<PolicyType>(parcel.ReadInt32());
    int32_t size = parcel.ReadInt32();
    size_t arraySize = COUNT_OF(autoUpgradePeriods);
    if ((size < MIN_VECTOR_SIZE) || (size > MAX_VECTOR_SIZE)) {
        ENGINE_LOGE("Invalid autoUpgradePeriods size=%{public}zu", size);
        return -1;
    }
    for (size_t i = 0; (i < static_cast<size_t>(size)) && (i < arraySize); i++) {
        autoUpgradePeriods[i].start = parcel.ReadUint32();
        autoUpgradePeriods[i].end = parcel.ReadUint32();
    }
    return true;
}

bool UpgradePolicy::Marshalling(Parcel &parcel) const
{
    if (!parcel.WriteBool(downloadStrategy)) {
        ENGINE_LOGE("Write downloadStrategy failed");
        return false;
    }

    if (!parcel.WriteBool(autoUpgradeStrategy)) {
        ENGINE_LOGE("Write downloadStrategy failed");
        return false;
    }

    if (!parcel.WriteInt32(CAST_INT(customPolicyType))) {
        ENGINE_LOGE("Write customPolicyType failed");
        return false;
    }

    int32_t size = static_cast<int32_t>(COUNT_OF(autoUpgradePeriods));
    if (!parcel.WriteInt32(size)) {
        ENGINE_LOGE("Write size failed");
        return false;
    }

    for (int32_t i = 0; i < size; i++) {
        parcel.WriteUint32(autoUpgradePeriods[i].start);
        parcel.WriteUint32(autoUpgradePeriods[i].end);
    }
    return true;
}

UpgradePolicy *UpgradePolicy::Unmarshalling(Parcel &parcel)
{
    UpgradePolicy *upgradePolicy = new (std::nothrow) UpgradePolicy();
    if (upgradePolicy == nullptr) {
        ENGINE_LOGE("Create upgradePolicy failed");
        return nullptr;
    }

    if (!upgradePolicy->ReadFromParcel(parcel)) {
        ENGINE_LOGE("Read from parcel failed");
        delete upgradePolicy;
        return nullptr;
    }
    return upgradePolicy;
}
} // namespace OHOS::UpdateService
