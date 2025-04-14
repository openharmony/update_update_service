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

#include "new_version_info.h"

#include "parcel_common.h"
#include "update_log.h"

namespace OHOS::UpdateService {
bool NewVersionInfo::ReadFromParcel(Parcel &parcel)
{
    versionDigestInfo.versionDigest = Str16ToStr8(parcel.ReadString16());
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

bool NewVersionInfo::Marshalling(Parcel &parcel) const
{
    if (!parcel.WriteString16(Str8ToStr16(versionDigestInfo.versionDigest))) {
        return false;
    }

    parcel.WriteInt32(static_cast<int32_t>(versionComponents.size()));
    for (size_t i = 0; i < versionComponents.size(); i++) {
        versionComponents[i].Marshalling(parcel);
    }
    return true;
}

NewVersionInfo *NewVersionInfo::Unmarshalling(Parcel &parcel)
{
    NewVersionInfo *newVersionInfo = new (std::nothrow) NewVersionInfo();
    if (newVersionInfo == nullptr) {
        ENGINE_LOGE("Create newVersionInfo failed");
        return nullptr;
    }

    if (!newVersionInfo->ReadFromParcel(parcel)) {
        ENGINE_LOGE("Read from parcel failed");
        delete newVersionInfo;
        return nullptr;
    }
    return newVersionInfo;
}
} // namespace OHOS::UpdateService
