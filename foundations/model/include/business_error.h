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

#include "nlohmann/json.hpp"

#include "call_result.h"
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

    friend void to_json(nlohmann::json &jsonObj, const BusinessError &businessError)
    {
        jsonObj["message"] = businessError.message;
        jsonObj["errorNum"] = businessError.errorNum;
        jsonObj["data"] = businessError.data;
    }

    friend void from_json(const nlohmann::json &jsonObj, BusinessError &businessError)
    {
        JsonUtils::GetValueAndSetTo(jsonObj, "message", businessError.message);
        JsonUtils::GetValueAndSetTo(jsonObj, "data", businessError.data);
        int32_t errorNumber = static_cast<int32_t>(CallResult::SUCCESS);
        JsonUtils::GetValueAndSetTo(jsonObj, "errorNum", errorNumber);
        businessError.errorNum = static_cast<CallResult>(errorNumber);
    }

    bool ReadFromParcel(Parcel &parcel);
    bool Marshalling(Parcel &parcel) const override;
    static BusinessError *Unmarshalling(Parcel &parcel);
};
} // namespace OHOS::UpdateService
#endif // UPDATE_SERVICE_BUSINESS_ERROR_H
