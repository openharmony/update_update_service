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

#include "update_session.h"

#include "node_api.h"

#include "client_helper.h"
#include "napi_common_utils.h"
#include "update_define.h"
#include "update_helper.h"

using namespace std;

namespace OHOS::UpdateEngine {
std::string GetFuncName(uint32_t sessionType)
{
    std::string funcName;
    for (auto &func: SESSION_FUNC_MAP) {
        if (func.first == sessionType) {
            funcName = func.second;
            break;
        }
    }
    return funcName;
}

void UpdateAsyncession::CompleteWork(napi_env env, napi_status status)
{
    ENGINE_LOGI("UpdateAsyncession::CompleteWork callbackNumber_: %{public}d, %{public}d",
        static_cast<int32_t>(callbackNumber_), sessionParams_.type);
    UpdateResult result;
    GetUpdateResult(result);
    NotifyJS(env, NULL, result);
}

std::string BaseUpdateSession::GetFunctionName()
{
    GetFuncName(sessionParams_.type);
}

void UpdatePromiseSession::CompleteWork(napi_env env, napi_status status)
{
    ENGINE_LOGI("UpdatePromiseSession::CompleteWork status: %d", static_cast<int32_t>(status));
    UpdateResult result;
    GetUpdateResult(result);
    NotifyJS(env, NULL, result);
}

std::string BaseMigratePromiseSession::GetFunctionName()
{
    GetFuncName(sessionParams_.type);
}

napi_value UpdateListener::StartWork(napi_env env, size_t startIndex, const napi_value *args)
{
    ENGINE_LOGI("UpdateListener::StartWork");
    PARAM_CHECK_NAPI_CALL(env, args != nullptr && totalArgc_ > startIndex, return nullptr, "Invalid para");

    if (NapiCommonUtils::IsTypeOf(env, args[0], napi_string) == ClientStatus::CLIENT_SUCCESS) {
        int ret = NapiCommonUtils::GetString(env, args[0], eventType_);
        PARAM_CHECK_NAPI_CALL(env, ret == napi_ok, return nullptr, "Failed to get string event type");
    } else {
        ClientStatus ret = ClientHelper::GetEventClassifyInfoFromArg(env, args[0], eventClassifyInfo_);
        PARAM_CHECK_NAPI_CALL(env, ret == ClientStatus::CLIENT_SUCCESS, return nullptr, "Failed to get obj event type");
    }

    PARAM_CHECK_NAPI_CALL(env,
        NapiCommonUtils::IsTypeOf(env, args[startIndex], napi_function) == ClientStatus::CLIENT_SUCCESS, return nullptr,
        "Invalid callback type");
    ClientStatus ret = NapiCommonUtils::CreateReference(env, args[startIndex], 1, handlerRef_);
    PARAM_CHECK_NAPI_CALL(env, ret == ClientStatus::CLIENT_SUCCESS, return nullptr, "Failed to create reference");
    int32_t res = doWorker_(context_);
    napi_value result;
    napi_create_int32(env, res, &result);
    return result;
}

void UpdateListener::NotifyJS(napi_env env, napi_value thisVar, const UpdateResult &result)
{
    ENGINE_LOGI("NotifyJS");
    napi_value jsEvent;
    napi_value handler = nullptr;
    napi_value callResult;
    int32_t ret = result.buildJSObject(env, jsEvent, result);
    PARAM_CHECK_NAPI_CALL(env, ret == napi_ok, return, "Failed to build json");
    {
        std::lock_guard<std::mutex> lock(mutex_);
        PARAM_CHECK_NAPI_CALL(env, handlerRef_ != nullptr, return, "handlerRef_ has beed freed");
        napi_status status = napi_get_reference_value(env, handlerRef_, &handler);
        PARAM_CHECK_NAPI_CALL(env, status == napi_ok && handler != nullptr, return, "Failed to get reference");
    }
    PARAM_CHECK_NAPI_CALL(env, handler != nullptr, return, "handlerRef_ has beed freed");
    napi_call_function(env, thisVar, handler, 1, &jsEvent, &callResult);
}

void UpdateListener::NotifyJS(napi_env env, napi_value thisVar, const EventInfo &eventInfo)
{
    ENGINE_LOGI("NotifyJS, eventId:0x%{public}08x", eventInfo.eventId);
    napi_value jsEvent = nullptr;
    ClientStatus ret = ClientHelper::BuildEventInfo(env, jsEvent, eventInfo);
    PARAM_CHECK_NAPI_CALL(env, ret == ClientStatus::CLIENT_SUCCESS, return, "Failed to build event info");

    std::lock_guard<std::mutex> lock(mutex_);
    PARAM_CHECK_NAPI_CALL(env, handlerRef_ != nullptr, return, "handlerRef_ has beed freed");
    napi_value handler = nullptr;
    napi_status status = napi_get_reference_value(env, handlerRef_, &handler);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok && handler != nullptr, return, "Failed to get reference");

    napi_value callResult = nullptr;
    status = napi_call_function(env, thisVar, handler, 1, &jsEvent, &callResult);
    if (status != napi_ok) {
        ENGINE_LOGE("NotifyJS error, napi_call_function fail");
    }
}

bool UpdateListener::CheckEqual(napi_env env, napi_value handler, const std::string &type)
{
    std::lock_guard<std::mutex> lock(mutex_);
    bool isEquals = false;
    napi_value handlerTemp = nullptr;
    napi_status status = napi_get_reference_value(env, handlerRef_, &handlerTemp);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok, return false, "Failed to get reference");
    napi_strict_equals(env, handler, handlerTemp, &isEquals);
    return isEquals && (type.compare(eventType_) == 0);
}

bool UpdateListener::IsSameListener(napi_env env, const EventClassifyInfo &eventClassifyInfo, napi_value handler)
{
    if (eventClassifyInfo_.eventClassify != eventClassifyInfo.eventClassify) {
        ENGINE_LOGI("not same listener, different event classify, 0x%{public}x, 0x%{public}x",
            eventClassifyInfo_.eventClassify, eventClassifyInfo.eventClassify);
        return false;
    }

    napi_value currentHandler = nullptr;
    napi_status status = napi_get_reference_value(env, handlerRef_, &currentHandler);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok, return false, "Failed to get current handle");

    bool isEquals = false;
    status = napi_strict_equals(env, handler, currentHandler, &isEquals);
    return status == napi_ok && isEquals;
}

void UpdateListener::RemoveHandlerRef(napi_env env)
{
    std::lock_guard<std::mutex> lock(mutex_);
    ENGINE_LOGI("RemoveHandlerRef handlerRef sessionId:%{public}u", GetSessionId());
    napi_delete_reference(env, handlerRef_);
    handlerRef_ = nullptr;
}
} // namespace OHOS::UpdateEngine