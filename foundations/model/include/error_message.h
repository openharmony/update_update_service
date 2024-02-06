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

#ifndef UPDATE_SERVICE_ERROR_MESSAGE_H
#define UPDATE_SERVICE_ERROR_MESSAGE_H

#include <cstdint>
#include <string>

#include "nlohmann/json.hpp"

#include "dupdate_json_utils.h"
#include "json_builder.h"

namespace OHOS::UpdateEngine {
struct ErrorMessage {
    int32_t errorCode = 0;
    std::string errorMessage;

    friend void to_json(nlohmann::json &jsonObj, const ErrorMessage &message)
    {
        jsonObj["errorCode"] = message.errorCode;
        jsonObj["errorMessage"] = message.errorMessage;
    }

    friend void from_json(const nlohmann::json &jsonObj, ErrorMessage &message)
    {
        JsonUtils::GetValueAndSetTo(jsonObj, "errorCode", message.errorCode);
        JsonUtils::GetValueAndSetTo(jsonObj, "errorMessage", message.errorMessage);
    }

    JsonBuilder GetJsonBuilder()
    {
        return JsonBuilder()
            .Append("{")
            .Append("errorCode", errorCode)
            .Append("errorMessage", errorMessage)
            .Append("}");
    }
};
} // namespace OHOS::UpdateEngine
#endif // UPDATE_SERVICE_ERROR_MESSAGE_H
