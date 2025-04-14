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

#include "pause_download_options.h"
#include "update_log.h"

namespace OHOS::UpdateService {
bool PauseDownloadOptions::ReadFromParcel(Parcel &parcel)
{
    isAllowAutoResume = parcel.ReadBool();
    return true;
}

bool PauseDownloadOptions::Marshalling(Parcel &parcel) const
{
    if (!parcel.WriteBool(isAllowAutoResume)) {
        ENGINE_LOGE("Write isAllowAutoResume failed");
        return false;
    }
    return true;
}

PauseDownloadOptions *PauseDownloadOptions::Unmarshalling(Parcel &parcel)
{
    PauseDownloadOptions *pauseDownloadOptions = new (std::nothrow) PauseDownloadOptions();
    if (pauseDownloadOptions == nullptr) {
        ENGINE_LOGE("Create pauseDownloadOptions failed");
        return nullptr;
    }

    if (!pauseDownloadOptions->ReadFromParcel(parcel)) {
        ENGINE_LOGE("Read from parcel failed");
        delete pauseDownloadOptions;
        return nullptr;
    }
    return pauseDownloadOptions;
}
} // namespace OHOS::UpdateService