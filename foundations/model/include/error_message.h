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

#include "cJSON.h"
#include "dupdate_json_utils.h"
#include "json_builder.h"

#include "parcel.h"

namespace OHOS::UpdateService {
struct ErrorMessage : public Parcelable {
    int32_t errorCode = 0;
    std::string errorMessage;
    friend void to_json(cJSON *jsonObject, const ErrorMessage &message)
    {
        if (jsonObject == nullptr)
        {
            return;
        }
        cJSON_AddNumberToObject(jsonObject, "errorCode", message.errorCode);
        cJSON_AddStringToObject(jsonObject, "errorCode", message.errorMessage.c_str());
    }

    friend void from_json(cJSON *jsonObject, ErrorMessage &message)
    {
        JsonUtils::GetValueAndSetTo(jsonObject, "errorCode", message.errorCode);
        JsonUtils::GetValueAndSetTo(jsonObject, "errorMessage", message.errorMessage);
    }

    JsonBuilder GetJsonBuilder()
    {
        return JsonBuilder()
            .Append("{")
            .Append("errorCode", errorCode)
            .Append("errorMessage", errorMessage)
            .Append("}");
    }

    bool ReadFromParcel(Parcel &parcel);
    bool Marshalling(Parcel &parcel) const override;
    static ErrorMessage *Unmarshalling(Parcel &parcel);
};
} // namespace OHOS::UpdateService
#endif // UPDATE_SERVICE_ERROR_MESSAGE_H
