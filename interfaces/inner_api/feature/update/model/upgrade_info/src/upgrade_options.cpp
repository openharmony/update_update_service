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

#include "upgrade_options.h"
#include "update_log.h"

namespace OHOS::UpdateService {
bool UpgradeOptions::ReadFromParcel(Parcel &parcel)
{
    order = static_cast<Order>(parcel.ReadUint32());
    return true;
}

bool UpgradeOptions::Marshalling(Parcel &parcel) const
{
    if (!parcel.WriteUint32(static_cast<uint32_t>(order))) {
        ENGINE_LOGE("Write order failed");
        return false;
    }
    return true;
}

UpgradeOptions *UpgradeOptions::Unmarshalling(Parcel &parcel)
{
    UpgradeOptions *upgradeOptions = new (std::nothrow) UpgradeOptions();
    if (upgradeOptions == nullptr) {
        ENGINE_LOGE("Create UpgradeOptions failed");
        return nullptr;
    }

    if (!upgradeOptions->ReadFromParcel(parcel)) {
        ENGINE_LOGE("Read from parcel failed");
        delete upgradeOptions;
        return nullptr;
    }
    return upgradeOptions;
}
} // namespace OHOS::UpdateService
