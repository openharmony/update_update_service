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

#ifndef UPDATE_SERVICE_BUSINESS_ERROR_H
#define UPDATE_SERVICE_BUSINESS_ERROR_H

#include <string>
#include <string_ex.h>
#include <vector>

#include "call_result.h"
#include "cJSON.h"
#include "dupdate_json_utils.h"
#include "error_message.h"
#include "json_builder.h"
#include "parcel.h"

namespace OHOS::UpdateService {
struct BusinessError : public Parcelable {
    std::string message;
    CallResult errorNum = CallResult::SUCCESS;
    std::vector<ErrorMessage> data;

    BusinessError &Build(CallResult callResult, const std::string &msg)
    {
        errorNum = callResult;
        message = msg;
        return *this;
    }

    BusinessError &AddErrorMessage(int32_t errorCode, const std::string &errorMessage)
    {
        ErrorMessage errMsg;
        errMsg.errorCode = errorCode;
        errMsg.errorMessage = errorMessage;
        data.push_back(errMsg);
        return *this;
    }

    friend void to_json(cJSON *jsonObject, const BusinessError &businessError)
    {
        if (jsonObject == nullptr)
        {
            return;
        }
        cJSON_AddStringToObject(jsonObject, "message", businessError.message.c_str());
        cJSON_AddNumberToObject(jsonObject, "errorNum", CAST_INT(businessError.errorNum));
        cJSON *dataArray = cJSON_CreateArray();
        if (dataArray == nullptr)
        {
            return;
        }

        for (const auto &errorMessage : businessError.data)
        {
            cJSON *errorMessageObject = cJSON_CreateObject();
            if (errorMessageObject == nullptr)
            {
                cJSON_Delete(dataArray);
                return;
            }
            to_json(errorMessageObject, errorMessage);
            cJSON_AddItemToArray(dataArray, errorMessageObject);
        }
        cJSON_AddItemToObject(jsonObject, "data", dataArray);
    }

    friend void from_json(cJSON *jsonObject, BusinessError &businessError)
    {
        if (jsonObject == nullptr)
        {
            return;
        }

        JsonUtils::GetValueAndSetTo(jsonObject, "message", businessError.message);
        int32_t errorNumber = static_cast<int32_t>(CallResult::SUCCESS);
        JsonUtils::GetValueAndSetTo(jsonObject, "errorNum", errorNumber);
        businessError.errorNum = static_cast<CallResult>(errorNumber);
        cJSON *dataArray = cJSON_GetObjectItemCaseSensitive(jsonObject, "data");
        if (dataArray == nullptr || !cJSON_IsArray(dataArray))
        {
            return;
        }
        size_t arraySize = cJSON_GetArraySize(dataArray);
        businessError.data.reserve(arraySize);
        for (size_t i = 0; i < arraySize; i++)
        {
            cJSON *errorMessageObject = cJSON_GetArrayItem(dataArray, i);
            if (errorMessageObject == nullptr || !cJSON_IsObject(errorMessageObject))
            {
                return;
            }
            ErrorMessage errorMessage;
            from_json(errorMessageObject, errorMessage);
            businessError.data.emplace_back(errorMessage);
        }
    }
    bool ReadFromParcel(Parcel &parcel);
    bool Marshalling(Parcel &parcel) const override;
    static BusinessError *Unmarshalling(Parcel &parcel);
};
} // namespace OHOS::UpdateService
#endif // UPDATE_SERVICE_BUSINESS_ERROR_H
