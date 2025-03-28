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

#include "version_digest_info.h"

namespace OHOS::UpdateEngine {
JsonBuilder VersionDigestInfo::GetJsonBuilder()
{
    return JsonBuilder()
        .Append("{")
        .Append("versionDigest", versionDigest)
        .Append("}");
}

bool VersionDigestInfo::ReadFromParcel(Parcel &parcel)
{
    versionDigest = Str16ToStr8(parcel.ReadString16());
    return true;
}

bool VersionDigestInfo::Marshalling(Parcel &parcel) const
{
    if (!parcel.WriteString16(Str8ToStr16(versionDigest))) {
        ENGINE_LOGE("write versionDigest failed");
        return false;
    }
    return true;
}

VersionDigestInfo *VersionDigestInfo::Unmarshalling(Parcel &parcel)
{
    VersionDigestInfo *versionDigestInfo = new (std::nothrow) VersionDigestInfo();
    if (versionDigestInfo == nullptr) {
        ENGINE_LOGE("Create versionDigestInfo failed");
        return nullptr;
    }

    if (!versionDigestInfo->ReadFromParcel(parcel)) {
        ENGINE_LOGE("Read from parcel failed");
        delete versionDigestInfo;
        return nullptr;
    }
    return versionDigestInfo;
}
} // namespace OHOS::UpdateEngine