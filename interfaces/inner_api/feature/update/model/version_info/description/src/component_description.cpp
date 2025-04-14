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

#include "component_description.h"

#include <string_ex.h>
#include "update_log.h"

namespace OHOS::UpdateService {
bool ComponentDescription::ReadFromParcel(Parcel &parcel)
{
    componentId = Str16ToStr8(parcel.ReadString16());
    descriptionInfo.descriptionType = static_cast<DescriptionType>(parcel.ReadUint32());
    descriptionInfo.content = Str16ToStr8(parcel.ReadString16());
    notifyDescriptionInfo.descriptionType = static_cast<DescriptionType>(parcel.ReadUint32());
    notifyDescriptionInfo.content = Str16ToStr8(parcel.ReadString16());
    return true;
}

bool ComponentDescription::Marshalling(Parcel &parcel) const
{
    parcel.WriteString16(Str8ToStr16(componentId));
    parcel.WriteUint32(static_cast<uint32_t>(descriptionInfo.descriptionType));
    parcel.WriteString16(Str8ToStr16(descriptionInfo.content));
    parcel.WriteUint32(static_cast<uint32_t>(notifyDescriptionInfo.descriptionType));
    parcel.WriteString16(Str8ToStr16(notifyDescriptionInfo.content));
    return true;
}

ComponentDescription *ComponentDescription::Unmarshalling(Parcel &parcel)
{
    ComponentDescription *componentDescription = new (std::nothrow) ComponentDescription();
    if (componentDescription == nullptr) {
        ENGINE_LOGE("Create componentDescription failed");
        return nullptr;
    }

    if (!componentDescription->ReadFromParcel(parcel)) {
        ENGINE_LOGE("Read from parcel failed");
        delete componentDescription;
        return nullptr;
    }
    return componentDescription;
}
} // namespace OHOS::UpdateService