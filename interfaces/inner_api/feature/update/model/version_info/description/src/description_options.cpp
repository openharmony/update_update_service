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

#include "description_options.h"

#include "description_info.h"
#include "update_define.h"

namespace OHOS::UpdateEngine {
bool DescriptionOptions::ReadFromParcel(Parcel &parcel)
{
    format = static_cast<DescriptionFormat>(parcel.ReadUint32());
    language = Str16ToStr8(parcel.ReadString16());
    return true;
}

bool DescriptionOptions::Marshalling(Parcel &parcel) const
{
    if (!parcel.WriteUint32(static_cast<uint32_t>(format))) {
        ENGINE_LOGE("write format failed");
        return false;
    }
    if (!parcel.WriteString16(Str8ToStr16(language))) {
        ENGINE_LOGE("write language failed");
        return false;
    }
    return true;
}

DescriptionOptions *DescriptionOptions::Unmarshalling(Parcel &parcel)
{
    DescriptionOptions *descriptionOptions = new (std::nothrow) DescriptionOptions();
    if (descriptionOptions == nullptr) {
        ENGINE_LOGE("Create descriptionOptions failed");
        return nullptr;
    }

    if (!descriptionOptions->ReadFromParcel(parcel)) {
        ENGINE_LOGE("Read from parcel failed");
        delete descriptionOptions;
        return nullptr;
    }
    return descriptionOptions;
}
} // namespace OHOS::UpdateEngine