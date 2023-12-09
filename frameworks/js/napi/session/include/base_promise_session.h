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

#ifndef BASE_PROMISE_SESSION_H
#define BASE_PROMISE_SESSION_H

#include "napi/native_api.h"
#include "node_api.h"

#include "napi_session.h"

namespace OHOS::UpdateEngine {
template <typename RESULT> class BasePromiseSession : public NapiSession {
public:
    BasePromiseSession(BaseClient *client, SessionParams &sessionParams, size_t argc, size_t callbackNumber = 0)
        : NapiSession(client, sessionParams, argc, callbackNumber) {}

    ~BasePromiseSession() override = default;

    napi_value StartWork(napi_env env, size_t startIndex, const napi_value *args) override
    {
        ENGINE_LOGI("BasePromiseSession::StartWork");
        PARAM_CHECK_NAPI_CALL(env, args != nullptr, return nullptr, "Invalid para");
        napi_value workName = CreateWorkerName(env);
        PARAM_CHECK_NAPI_CALL(env, workName != nullptr, return nullptr, "Failed to worker name");

        napi_value promise;
        napi_status status = napi_create_promise(env, &deferred_, &promise);
        PARAM_CHECK_NAPI_CALL(env, status == napi_ok, return nullptr, "Failed to napi_create_promise");

        // Create an asynchronous call.
        status = napi_create_async_work(env, nullptr, workName, NapiSession::ExecuteWork, NapiSession::CompleteWork,
            this, &(worker_));
        PARAM_CHECK_NAPI_CALL(env, status == napi_ok, return nullptr, "Failed to napi_create_async_work");
        // Put the thread in the task execution queue.
        status = napi_queue_async_work_with_qos(env, worker_, napi_qos_default);
        PARAM_CHECK_NAPI_CALL(env, status == napi_ok, return nullptr, "Failed to napi_queue_async_work");
        return promise;
    }

    void NotifyJS(napi_env env, napi_value thisVar, const RESULT &result)
    {
        int32_t errorNum = static_cast<int32_t>(result.businessError.errorNum);
        ENGINE_LOGI("BasePromiseSession NotifyJS errorNum:%{public}d", errorNum);

        // Get the return result.
        napi_value processResult = nullptr;
        BusinessError businessError;
        GetBusinessError(businessError, result);
        if (businessError.errorNum == CallResult::SUCCESS) {
            result.buildJSObject(env, processResult, result);
            napi_resolve_deferred(env, deferred_, processResult);
        } else {
            NapiCommonUtils::BuildBusinessError(env, processResult, businessError);
            napi_reject_deferred(env, deferred_, processResult);
        }
        napi_delete_async_work(env, worker_);
        worker_ = nullptr;
    }

    virtual void CompleteWork(napi_env env, napi_status status) override {}

protected:
    napi_async_work worker_ = nullptr;
    napi_deferred deferred_ = nullptr;
};
} // namespace OHOS::UpdateEngine
#endif // BASE_PROMISE_SESSION_H