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

#include "task_body.h"

#include "parcel_common.h"
#include "task_body_member_mask.h"
#include "update_log.h"

namespace OHOS::UpdateEngine {
JsonBuilder GetJsonBuilder(VersionComponent &versionComponent)
{
    return versionComponent.GetJsonBuilder();
}

JsonBuilder GetJsonBuilder(ErrorMessage &errorMessage)
{
    return errorMessage.GetJsonBuilder();
}

template <typename T>
std::vector<JsonBuilder> GetArrayJsonBuilderList(const std::vector<T> &valueList)
{
    std::vector<JsonBuilder> jsonBuilderList;
    for (T value : valueList) {
        jsonBuilderList.push_back(GetJsonBuilder(value));
    }
    return jsonBuilderList;
}

JsonBuilder TaskBody::GetJsonBuilder(EventId eventId)
{
    JsonBuilder jsonBuilder = JsonBuilder();
    auto iter = g_taskBodyTemplateMap.find(eventId);
    if (iter == g_taskBodyTemplateMap.end()) {
        ENGINE_LOGE("GetJsonBuilder eventId not find");
        return jsonBuilder;
    }
    jsonBuilder.Append("{");
    uint32_t taskBodyTemplate = iter->second;
    if (taskBodyTemplate & VERSION_DIGEST_INFO) {
        jsonBuilder.Append("versionDigestInfo", versionDigestInfo.GetJsonBuilder());
    }
    if (taskBodyTemplate & UPGRADE_STATUS) {
        jsonBuilder.Append("status", CAST_INT(status));
    }
    if (taskBodyTemplate & SUB_STATUS) {
        jsonBuilder.Append("subStatus", subStatus);
    }
    if (taskBodyTemplate & PROGRESS) {
        jsonBuilder.Append("progress", progress);
    }
    if (taskBodyTemplate & INSTALL_MODE) {
        jsonBuilder.Append("installMode", installMode);
    }
    if (taskBodyTemplate & ERROR_MESSAGE) {
        jsonBuilder.Append("errorMessages", GetArrayJsonBuilderList(errorMessages));
    }
    if (taskBodyTemplate & VERSION_COMPONENT) {
        jsonBuilder.Append("versionComponents", GetArrayJsonBuilderList(versionComponents));
    }
    return jsonBuilder.Append("}");
}

bool TaskBody::ReadFromParcel(Parcel &parcel)
{
    versionDigestInfo.versionDigest = Str16ToStr8(parcel.ReadString16());
    status = static_cast<UpgradeStatus>(parcel.ReadInt32());
    subStatus = parcel.ReadInt32();
    progress = parcel.ReadInt32();
    installMode = parcel.ReadInt32();

    int32_t errorMessageSize = parcel.ReadInt32();
    if (errorMessageSize > MAX_VECTOR_SIZE) {
        ENGINE_LOGE("ReadErrorMessages size is over MAX_VECTOR_SIZE, size=%{public}d", errorMessageSize);
        return false;
    }

    for (size_t i = 0; i < static_cast<size_t>(errorMessageSize); i++) {
        sptr<ErrorMessage> unmarshallingErrorMsg = ErrorMessage().Unmarshalling(parcel);
        if (unmarshallingErrorMsg != nullptr) {
            errorMessages.emplace_back(*unmarshallingErrorMsg);
        } else {
            ENGINE_LOGE("unmarshallingErrorMsg is null");
            return false;
        }
    }

    int32_t componentSize = parcel.ReadInt32();
    if (componentSize > MAX_VECTOR_SIZE) {
        ENGINE_LOGE("ReadVersionComponents size is over MAX_VECTOR_SIZE, size=%{public}d", componentSize);
        return false;
    }

    for (size_t i = 0; i < static_cast<size_t>(componentSize); i++) {
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


bool TaskBody::Marshalling(Parcel &parcel) const
{
    parcel.WriteString16(Str8ToStr16(versionDigestInfo.versionDigest));
    parcel.WriteInt32(static_cast<int32_t>(status));
    parcel.WriteInt32(subStatus);
    parcel.WriteInt32(progress);
    parcel.WriteInt32(installMode);

    parcel.WriteInt32(static_cast<int32_t>(errorMessages.size()));
    for (size_t i = 0; i < errorMessages.size(); i++) {
        errorMessages[i].Marshalling(parcel);
    }

    parcel.WriteInt32(static_cast<int32_t>(versionComponents.size()));
    for (size_t i = 0; i < versionComponents.size(); i++) {
        versionComponents[i].Marshalling(parcel);
    }
    return true;
}

TaskBody *TaskBody::Unmarshalling(Parcel &parcel)
{
    TaskBody *taskBody = new (std::nothrow) TaskBody();
    if (taskBody == nullptr) {
        ENGINE_LOGE("Create currentVersionInfo failed");
        return nullptr;
    }

    if (!taskBody->ReadFromParcel(parcel)) {
        ENGINE_LOGE("Read from parcel failed");
        delete taskBody;
        return nullptr;
    }
    return taskBody;
}
} // namespace OHOS::UpdateEngine