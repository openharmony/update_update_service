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

#include "check_result.h"
#include "parcel_common.h"

namespace OHOS::UpdateService {
bool CheckResult::ReadFromParcel(Parcel &parcel)
{
    isExistNewVersion = parcel.ReadBool();
    newVersionInfo.versionDigestInfo.versionDigest = Str16ToStr8(parcel.ReadString16());

    int32_t size = parcel.ReadInt32();
    if ((size < MIN_VECTOR_SIZE) || (size > MAX_VECTOR_SIZE)) {
        ENGINE_LOGE("Invalid versionComponent size=%{public}d", size);
        return false;
    }

    for (size_t i = 0; i < static_cast<size_t>(size); i++) {
        sptr<VersionComponent> unmarshallingVersionComponent = VersionComponent().Unmarshalling(parcel);
        if (unmarshallingVersionComponent != nullptr) {
            newVersionInfo.versionComponents.emplace_back(*unmarshallingVersionComponent);
        } else {
            ENGINE_LOGE("unmarshallingVersionComponent is null");
            return false;
        }
    }
    return true;
}

bool CheckResult::Marshalling(Parcel &parcel) const
{
    if (!parcel.WriteBool(isExistNewVersion)) {
        ENGINE_LOGE("Write isExistNewVersion failed");
        return false;
    }

    if (!parcel.WriteString16(Str8ToStr16(newVersionInfo.versionDigestInfo.versionDigest))) {
        ENGINE_LOGE("Write versionDigest failed");
        return false;
    }

    parcel.WriteInt32(static_cast<int32_t>(newVersionInfo.versionComponents.size()));
    for (size_t i = 0; i < newVersionInfo.versionComponents.size(); i++) {
        newVersionInfo.versionComponents[i].Marshalling(parcel);
    }
    return true;
}

CheckResult *CheckResult::Unmarshalling(Parcel &parcel)
{
    CheckResult *checkResult = new (std::nothrow) CheckResult();
    if (checkResult == nullptr) {
        ENGINE_LOGE("Create checkResult failed");
        return nullptr;
    }

    if (!checkResult->ReadFromParcel(parcel)) {
        ENGINE_LOGE("Read from parcel failed");
        delete checkResult;
        return nullptr;
    }
    return checkResult;
}
} // namespace OHOS::UpdateService
