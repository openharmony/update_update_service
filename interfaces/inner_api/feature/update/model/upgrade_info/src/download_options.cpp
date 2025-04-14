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

#include "download_options.h"
#include "update_log.h"

namespace OHOS::UpdateService {
bool DownloadOptions::ReadFromParcel(Parcel &parcel)
{
    allowNetwork = static_cast<NetType>(parcel.ReadUint32());
    order = static_cast<Order>(parcel.ReadUint32());
    return true;
}

bool DownloadOptions::Marshalling(Parcel &parcel) const
{
    if (!parcel.WriteUint32(static_cast<uint32_t>(allowNetwork))) {
        ENGINE_LOGE("Write allowNetwork failed");
        return false;
    }

    if (!parcel.WriteUint32(static_cast<uint32_t>(order))) {
        ENGINE_LOGE("Write order failed");
        return false;
    }
    return true;
}

DownloadOptions *DownloadOptions::Unmarshalling(Parcel &parcel)
{
    DownloadOptions *downloadOptions = new (std::nothrow) DownloadOptions();
    if (downloadOptions == nullptr) {
        ENGINE_LOGE("Create downloadOptions failed");
        return nullptr;
    }

    if (!downloadOptions->ReadFromParcel(parcel)) {
        ENGINE_LOGE("Read from parcel failed");
        delete downloadOptions;
        return nullptr;
    }
    return downloadOptions;
}
} // namespace OHOS::UpdateService