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

#include "version_description_info.h"

#include "error_message.h"
#include "parcel_common.h"
#include "update_log.h"

namespace OHOS::UpdateService {
bool VersionDescriptionInfo::ReadFromParcel(Parcel &parcel)
{
    int32_t componentDescriptionsSize = parcel.ReadInt32();
    if (componentDescriptionsSize > MAX_VECTOR_SIZE) {
        ENGINE_LOGE("size is over MAX_VECTOR_SIZE, size=%{public}d", componentDescriptionsSize);
        return false;
    }

    for (size_t i = 0; i < static_cast<size_t>(componentDescriptionsSize); i++) {
        sptr<ComponentDescription> unmarshallingComponentDescription = ComponentDescription().Unmarshalling(parcel);
        if (unmarshallingComponentDescription != nullptr) {
            componentDescriptions.emplace_back(*unmarshallingComponentDescription);
        } else {
            ENGINE_LOGE("unmarshallingComponentDescription is null");
            return false;
        }
    }
    return true;
}

bool VersionDescriptionInfo::Marshalling(Parcel &parcel) const
{
    parcel.WriteInt32(static_cast<int32_t>(componentDescriptions.size()));
    for (size_t i = 0; i < componentDescriptions.size(); i++) {
        const ComponentDescription *componentDescription = &componentDescriptions[i];
        componentDescription->Marshalling(parcel);
    }
    return true;
}

VersionDescriptionInfo *VersionDescriptionInfo::Unmarshalling(Parcel &parcel)
{
    VersionDescriptionInfo *versionDescriptionInfo = new (std::nothrow) VersionDescriptionInfo();
    if (versionDescriptionInfo == nullptr) {
        ENGINE_LOGE("Create versionDescriptionInfo failed");
        return nullptr;
    }

    if (!versionDescriptionInfo->ReadFromParcel(parcel)) {
        ENGINE_LOGE("Read from parcel failed");
        delete versionDescriptionInfo;
        return nullptr;
    }
    return versionDescriptionInfo;
}
} // namespace OHOS::UpdateService
