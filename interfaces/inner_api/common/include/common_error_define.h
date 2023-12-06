/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef COMMON_ERROR_DEFINE_H
#define COMMON_ERROR_DEFINE_H

#include <string>
#include <vector>

#include "json_builder.h"
#include "json_utils.h"

namespace OHOS::UpdateEngine {
#define CAST_INT(enumClass) (static_cast<int32_t>(enumClass))
#define CALL_RESULT_TO_IPC_RESULT(callResult) ((callResult) + CALL_RESULT_OFFSET)
constexpr int32_t COMPONENT_ERR = 11500000;
constexpr int CALL_RESULT_OFFSET = 2000;
enum class CallResult {
    // 通用错误码
    APP_NOT_GRANTED = 201,
    NOT_SYSTEM_APP = 202,
    PARAM_ERR = 401,
    UN_SUPPORT = 801,

    // 模块内错误码
    SUCCESS = 0,
    FAIL = 100,
    DEV_UPG_INFO_ERR = 102,
    FORBIDDEN = 103,
    IPC_ERR = 104,
    TIME_OUT = 402,
    DB_ERROR = 501,
    IO_ERROR = 502,
    NET_ERROR = 503
};

constexpr int32_t INT_CALL_SUCCESS = CAST_INT(CallResult::SUCCESS);
constexpr int32_t INT_CALL_FAIL = CAST_INT(CallResult::FAIL);
constexpr int32_t INT_UN_SUPPORT = CAST_INT(CallResult::UN_SUPPORT);
constexpr int32_t INT_FORBIDDEN = CAST_INT(CallResult::FORBIDDEN);
constexpr int32_t INT_CALL_IPC_ERR = CAST_INT(CallResult::IPC_ERR);
constexpr int32_t INT_APP_NOT_GRANTED = CAST_INT(CallResult::APP_NOT_GRANTED);
constexpr int32_t INT_NOT_SYSTEM_APP = CAST_INT(CallResult::NOT_SYSTEM_APP);
constexpr int32_t INT_PARAM_ERR = CAST_INT(CallResult::PARAM_ERR);
constexpr int32_t INT_DEV_UPG_INFO_ERR = CAST_INT(CallResult::DEV_UPG_INFO_ERR);
constexpr int32_t INT_TIME_OUT = CAST_INT(CallResult::TIME_OUT);
constexpr int32_t INT_DB_ERROR = CAST_INT(CallResult::DB_ERROR);
constexpr int32_t INT_IO_ERROR = CAST_INT(CallResult::IO_ERROR);
constexpr int32_t INT_NET_ERROR = CAST_INT(CallResult::NET_ERROR);

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

struct BusinessError {
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
};
} // namespace OHOS::UpdateEngine
#endif // COMMON_ERROR_DEFINE_H