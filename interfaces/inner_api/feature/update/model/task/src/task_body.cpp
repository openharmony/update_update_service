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
} // namespace OHOS::UpdateEngine