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

#include "version_component.h"

#include <string_ex.h>
#include "update_log.h"

namespace OHOS::UpdateEngine {
JsonBuilder VersionComponent::GetJsonBuilder()
{
    return JsonBuilder()
        .Append("{")
        .Append("componentId", componentId)
        .Append("componentType", componentType)
        .Append("upgradeAction", upgradeAction)
        .Append("displayVersion", displayVersion)
        .Append("innerVersion", innerVersion)
        .Append("size", static_cast<int64_t>(size))
        .Append("effectiveMode", static_cast<int32_t>(effectiveMode))
        .Append("otaMode", static_cast<int32_t>(otaMode))
        .Append("descriptionInfo", descriptionInfo.GetJsonBuilder())
        .Append("componentExtra", componentExtra, true)
        .Append("}");
}

    bool VersionComponent::ReadFromParcel(Parcel &parcel)
    {
        componentId = Str16ToStr8(parcel.ReadString16());
        componentType = parcel.ReadInt32();
        upgradeAction = Str16ToStr8(parcel.ReadString16());
        displayVersion = Str16ToStr8(parcel.ReadString16());
        innerVersion = Str16ToStr8(parcel.ReadString16());
        size = static_cast<size_t>(parcel.ReadUint64());
        effectiveMode = static_cast<size_t>(parcel.ReadUint32());
        otaMode = static_cast<size_t>(parcel.ReadUint32());

        descriptionInfo.descriptionType = static_cast<DescriptionType>(parcel.ReadUint32());
        descriptionInfo.content = Str16ToStr8(parcel.ReadString16());
        componentExtra = Str16ToStr8(parcel.ReadString16());
        return true;
    }

    bool VersionComponent::Marshalling(Parcel &parcel) const
    {
        parcel.WriteString16(Str8ToStr16(componentId));
        parcel.WriteInt32(componentType);
        parcel.WriteString16(Str8ToStr16(upgradeAction));
        parcel.WriteString16(Str8ToStr16(displayVersion));
        parcel.WriteString16(Str8ToStr16(innerVersion));
        parcel.WriteUint64(static_cast<uint64_t>(size));
        parcel.WriteUint32(static_cast<uint32_t>(effectiveMode));
        parcel.WriteUint32(static_cast<uint32_t>(otaMode));

        parcel.WriteUint32(static_cast<uint32_t>(descriptionInfo.descriptionType));
        parcel.WriteString16(Str8ToStr16(descriptionInfo.content));
        parcel.WriteString16(Str8ToStr16(componentExtra));
        return true;
    }

    VersionComponent *VersionComponent::Unmarshalling(Parcel &parcel)
    {
        VersionComponent *versionComponent = new (std::nothrow) VersionComponent();
        if (versionComponent == nullptr) {
            ENGINE_LOGE("Create versionComponent failed");
            return nullptr;
        }

        if (!versionComponent->ReadFromParcel(parcel)) {
            ENGINE_LOGE("Read from parcel failed");
            delete versionComponent;
            return nullptr;
        }
        return versionComponent;
    }
} // namespace OHOS::UpdateEngine