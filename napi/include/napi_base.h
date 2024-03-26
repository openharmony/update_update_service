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

#ifndef NAPI_BASE_H
#define NAPI_BASE_H

#include <memory>

#include "napi_common_utils.h"
#include "napi/native_api.h"
#include "napi/native_common.h"

namespace OHOS::UpdateEngine {
template <typename T> class NapiBase {
#define GET_PARAMS(env, info, num)    \
    size_t argc = num;                \
    napi_value args[num] = {nullptr}; \
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr)

public:
    NapiBase() = default;
    ~NapiBase() = default;

    static napi_value HandleFunc(napi_env env, napi_callback_info info,
        std::unique_ptr<T> &clientContext)
    {
        if (clientContext == nullptr) {
            ENGINE_LOGI("HandleFunc clientContext is null");
            return nullptr;
        }
        std::string method = clientContext->method_;
        ENGINE_LOGI("HandleFunc method: %{public}s", method.c_str());
        napi_value result = clientContext->getNapiParam_(env, info, clientContext);
        if (result == nullptr) {
            ENGINE_LOGE("HandleFunc GetMigrateStatusParam fail");
            return nullptr;
        }
        if (!Execute(env, clientContext)) {
            ENGINE_LOGE("HandleFunc Execute error");
            return result;
        }
        ENGINE_LOGI("HandleFunc method: %{public}s complete", method.c_str());
        return result;
    }

    static napi_value GetCallbackParam(napi_env env, uint32_t argNum, size_t argc, napi_value args[],
        std::unique_ptr<T> &clientContext)
    {
        // 接口调用返回值,非返回内容
        napi_value result = nullptr;
        if (argc >= argNum) {
            uint32_t callbackPosition = argNum - 1;
            napi_valuetype callbackValueType;
            napi_typeof(env, args[callbackPosition], &callbackValueType);
            std::vector<std::pair<std::string, std::string>> paramInfos;
            paramInfos.emplace_back("callback", "napi_function");
            PARAM_CHECK(callbackValueType == napi_function, NapiCommonUtils::NapiThrowParamError(env, paramInfos);
               return nullptr, "Failed to GetCallbackParam");
            napi_create_reference(env, args[callbackPosition], 1, &clientContext->callbackRef_);
            napi_get_undefined(env, &result); // 创建接口返回值对象
        } else {
            napi_create_promise(env, &clientContext->deferred_, &result);
        }
        return result;
    }

    static bool Execute(napi_env env, std::unique_ptr<T> &clientContext)
    {
        napi_value workName;
        napi_create_string_utf8(env, clientContext->method_.c_str(), NAPI_AUTO_LENGTH, &workName);
        if (napi_create_async_work(env, nullptr, workName, clientContext->executeFunc_, NapiBase::Complete,
                                   static_cast<void *>(clientContext.get()), &clientContext->work_) != napi_ok) {
            ENGINE_LOGE("Failed to create async work for %{public}s", clientContext->method_.c_str());
            return false;
        }
        if (napi_queue_async_work_with_qos(env, clientContext->work_, napi_qos_default) != napi_ok) {
            ENGINE_LOGE("Failed to queue async work for %{public}s", clientContext->method_.c_str());
            return false;
        }
        ENGINE_LOGI("Execute finish");
        clientContext.release(); // unique_ptr release之后，释放指针的控制权，后续由complete里面释放指针内容
        return true;
    }

    static void Complete(napi_env env, napi_status status, void *data)
    {
        constexpr size_t resultLen = 2;
        T *clientContext = static_cast<T *>(data);

        napi_value finalResult = nullptr;
        if (clientContext->createValueFunc_ != nullptr) {
            // 执行结果转换函数
            finalResult = clientContext->createValueFunc_(env, *clientContext);
        }

        if (clientContext->ipcRequestCode_ != 0) {
            // ipc失败，获取失败原因
            clientContext->getIpcBusinessError_(clientContext->method_, clientContext->ipcRequestCode_,
                clientContext->businessError_);
        }

        napi_value result[resultLen] = { nullptr, nullptr };
        bool isSuccess = BuildResult(env, clientContext, finalResult, result);
        if (clientContext->deferred_) { // promise调用
            ExecutePromiseFunc(env, clientContext, result, resultLen, isSuccess);
        } else {
            ExecuteCallbackFunc(env, clientContext, result, resultLen);
        }
        napi_delete_async_work(env, clientContext->work_);
        delete clientContext; // Execute 中释放控制权的指针，在此处释放
        clientContext = nullptr;
    }

    static void ExecutePromiseFunc(napi_env env, T *clientContext, napi_value const * result, size_t len,
        bool isSuccess)
    {
        constexpr size_t resultLength = 2;
        if (len < resultLength) {
            ENGINE_LOGE("length error:%{public}zu", len);
            return;
        }
        napi_status callbackStatus = isSuccess ? napi_resolve_deferred(env, clientContext->deferred_, result[1]) :
                                                 napi_reject_deferred(env, clientContext->deferred_, result[0]);
        if (callbackStatus != napi_ok) {
            ENGINE_LOGE("ExecutePromiseFunc error: %{public}d", callbackStatus);
        }
    }

    static void ExecuteCallbackFunc(napi_env env, T *clientContext, napi_value *result, size_t len)
    {
        napi_value callback = nullptr;
        napi_status resultStatus = napi_get_reference_value(env, clientContext->callbackRef_, &callback);
        if (resultStatus != napi_ok) {
            ENGINE_LOGE("napi_get_reference_value failed result=%{public}d", resultStatus);
            return;
        }
        napi_value userRet = nullptr;
        resultStatus = napi_call_function(env, nullptr, callback, len, result, &userRet);
        if (resultStatus != napi_ok) {
            ENGINE_LOGE("napi_call_function failed result=%{public}d", resultStatus);
            return;
        }
        resultStatus = napi_delete_reference(env, clientContext->callbackRef_);
        if (resultStatus != napi_ok) {
            ENGINE_LOGE("napi_delete_reference failed result=%{public}d", resultStatus);
        }
    }

    static bool BuildResult(napi_env env, const T *clientContext, napi_value finalResult, napi_value *result)
    {
        bool isSuccess = clientContext->businessError_.errorNum == CallResult::SUCCESS;
        if (isSuccess) {
            napi_get_undefined(env, &result[0]);
            result[1] = finalResult;
        } else {
            NapiCommonUtils::BuildBusinessError(env, result[0], clientContext->businessError_);
            napi_get_undefined(env, &result[1]);
        }
        return isSuccess;
    }
};
} // namespace OHOS::UpdateEngine
#endif // NAPI_BASE_H