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

#include "current_version_info.h"
#include "parcel_common.h"
#include "update_log.h"

namespace OHOS::UpdateEngine {

bool CurrentVersionInfo::ReadFromParcel(Parcel &parcel)
{
    osVersion = Str16ToStr8(parcel.ReadString16());
    deviceName = Str16ToStr8(parcel.ReadString16());

    int32_t size = parcel.ReadInt32();
    if (size > MAX_VECTOR_SIZE) {
        ENGINE_LOGE("ReadVersionComponents size is over MAX_VECTOR_SIZE, size=%{public}d", size);
        return false;
    }

    for (size_t i = 0; i < static_cast<size_t>(size); i++) {
        sptr<VersionComponent> unmarshallingVersionComponent = VersionComponent().Unmarshalling(parcel);
        if (unmarshallingVersionComponent != nullptr) {
            versionComponents.emplace_back(*unmarshallingVersionComponent);
        } else {
            ENGINE_LOGE("unmarshallingVersionComponent is null");
            return false;
        }
    }
    return true;
}

bool CurrentVersionInfo::Marshalling(Parcel &parcel) const
{
    if (!parcel.WriteString16(Str8ToStr16(osVersion))) {
        ENGINE_LOGE("write osVersion failed");
        return false;
    }

    if (!parcel.WriteString16(Str8ToStr16(deviceName))) {
        ENGINE_LOGE("write deviceName failed");
        return false;
    }

    parcel.WriteInt32(static_cast<int32_t>(versionComponents.size()));
    for (size_t i = 0; i < versionComponents.size(); i++) {
        versionComponents[i].Marshalling(parcel);
    }
    return true;
}

CurrentVersionInfo *CurrentVersionInfo::Unmarshalling(Parcel &parcel)
{
    CurrentVersionInfo *currentVersionInfo = new (std::nothrow) CurrentVersionInfo();
    if (currentVersionInfo == nullptr) {
        ENGINE_LOGE("Create currentVersionInfo failed");
        return nullptr;
    }

    if (!currentVersionInfo->ReadFromParcel(parcel)) {
        ENGINE_LOGE("Read from parcel failed");
        delete currentVersionInfo;
        return nullptr;
    }
    return currentVersionInfo;
}
} // namespace OHOS::UpdateEngine
