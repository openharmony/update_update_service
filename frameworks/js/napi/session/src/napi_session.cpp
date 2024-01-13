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

#include "napi_session.h"

#include "node_api.h"

#include "base_client.h"
#include "napi_common_define.h"
#include "napi_common_utils.h"

using namespace std;

namespace OHOS::UpdateEngine {
uint32_t g_sessionId = 0;

NapiSession::NapiSession(BaseClient *client, SessionParams &sessionParams, size_t argc, size_t callbackNumber)
    : sessionId(++g_sessionId), client_(client), sessionParams_(sessionParams), totalArgc_(argc),
    callbackNumber_(callbackNumber) {}

napi_value NapiSession::CreateWorkerName(napi_env env) const
{
    napi_value workName;
    std::string name = "Async Work" + std::to_string(sessionId);
    napi_status status = napi_create_string_utf8(env, name.c_str(), NAPI_AUTO_LENGTH, &workName);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok, return nullptr, "Failed to worker name");
    return workName;
}

napi_value NapiSession::StartWork(napi_env env, const napi_value *args, DoWorkFunction worker, void *context)
{
    ENGINE_LOGI("StartWork type: %{public}d", CAST_INT(sessionParams_.type));
    doWorker_ = worker;
    context_ = context;
    return StartWork(env, sessionParams_.callbackStartIndex, args);
}

void NapiSession::ExecuteWork(napi_env env)
{
    if (doWorker_ != nullptr) {
#ifndef UPDATER_UT
        if (sessionParams_.isNeedBusinessError) {
            workResult_ = doWorker_(&businessError_);
        } else {
            workResult_ = doWorker_(context_);
        }
        ENGINE_LOGI("UpdateSession::ExecuteWork workResult : %{public}d", workResult_);
        if (sessionParams_.isAsyncCompleteWork && IsWorkExecuteSuccess()) {
            // 异步搜包完成，需要把businessError设置进来或者超时，才能结束等待
            std::unique_lock<std::mutex> lock(conditionVariableMutex_);
            auto now = std::chrono::system_clock::now();
            conditionVariable_.wait_until(lock, now + 40000ms, [this] { return asyncExecuteComplete_; });
            ENGINE_LOGI("UpdateSession::ExecuteWork asyncExcuteComplete : %{public}s",
                asyncExecuteComplete_ ? "true" : "false");
            if (!asyncExecuteComplete_) {
                businessError_.errorNum = CallResult::TIME_OUT;
            }
        }
#else
        doWorker_(context_);
#endif
    }
}

// JS thread, which is used to notify the JS page upon completion of the operation.
void NapiSession::CompleteWork(napi_env env, napi_status status, void *data)
{
    auto sess = reinterpret_cast<NapiSession *>(data);
    PARAM_CHECK(sess != nullptr && sess->GetNapiClient() != nullptr, return, "Session is null pointer");
    sess->CompleteWork(env, status);
    // If the share ptr is used, you can directly remove the share ptr.
    BaseClient *client = sess->GetNapiClient();
    if (client != nullptr) {
        client->RemoveSession(sess->GetSessionId());
    }
}

// The C++ thread executes the synchronization operation. After the synchronization is complete,
// the CompleteWork is called to notify the JS page of the completion of the operation.
void NapiSession::ExecuteWork(napi_env env, void *data)
{
    auto sess = reinterpret_cast<NapiSession *>(data);
    PARAM_CHECK(sess != nullptr, return, "sess is null");
    sess->ExecuteWork(env);
}

void NapiSession::GetSessionFuncParameter(std::string &funcName, std::string &permissionName)
{
    funcName = GetFunctionName();
    permissionName = GetFunctionPermissionName();
}
} // namespace OHOS::UpdateEngine