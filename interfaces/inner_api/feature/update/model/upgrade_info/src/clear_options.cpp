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

#include "clear_options.h"

#include "update_log.h"

namespace OHOS::UpdateEngine {
bool ClearOptions::ReadFromParcel(Parcel &parcel)
{
    status = static_cast<UpgradeStatus>(parcel.ReadUint32());
    return true;
}

bool ClearOptions::Marshalling(Parcel &parcel) const
{
    if (!parcel.WriteUint32(static_cast<uint32_t>(status))) {
        ENGINE_LOGE("Write status failed");
        return false;
    }
    return true;
}

ClearOptions *ClearOptions::Unmarshalling(Parcel &parcel)
{
    ClearOptions *clearOptions = new (std::nothrow) ClearOptions();
    if (clearOptions == nullptr) {
        ENGINE_LOGE("Create clearOptions failed");
        return nullptr;
    }

    if (!clearOptions->ReadFromParcel(parcel)) {
        ENGINE_LOGE("Read from parcel failed");
        delete clearOptions;
        return nullptr;
    }
    return clearOptions;
}
} // namespace OHOS::UpdateEngine
