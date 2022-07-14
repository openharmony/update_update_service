/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include "client_helper.h"
#include "napi_util.h"
#include "node_api.h"
#include "package/package.h"
#include "securec.h"
#include "update_helper.h"

using namespace std;

namespace OHOS {
namespace UpdateEngine {
const int32_t RESULT_ARGC = 2;

uint32_t g_sessionId = 0;
UpdateSession::UpdateSession(IUpdater *client, SessionParams &sessionParams, size_t argc, size_t callbackNumber)
    : sessionId(++g_sessionId), client_(client), sessionParams_(sessionParams), totalArgc_(argc),
    callbackNumber_(callbackNumber) {}

napi_value UpdateSession::CreateWorkerName(napi_env env) const
{
    napi_value workName;
    std::string name = "Async Work" + std::to_string(sessionId);
    napi_status status = napi_create_string_utf8(env, name.c_str(), NAPI_AUTO_LENGTH, &workName);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok, return nullptr, "Failed to worker name");
    return workName;
}

napi_value UpdateSession::StartWork(napi_env env, const napi_value *args, DoWorkFunction worker, void *context)
{
    static std::string sessName[static_cast<int32_t>(SessionType::SESSION_MAX)] = {
        "check version",
        "download",
        "pause download",
        "resume download",
        "upgrade",
        "set policy",
        "get policy",
        "clear error",
        "terminate upgrade",
        "get new version",
        "subscribe",
        "unsubscribe",
        "get updater",
        "apply new version",
        "reboot and clean",
        "verify package",
        "cancel Upgrade",
        "get ota status",
        "get current version",
        "get task info"
    };

    CLIENT_LOGI("StartWork type: %{public}s", sessName[static_cast<int32_t>(sessionParams_.type)].c_str());
    doWorker_ = worker;
    context_ = context;
    return StartWork(env, sessionParams_.callbackStartIndex, args);
}

void UpdateSession::ExecuteWork(napi_env env)
{
    if (doWorker_ != nullptr) {
#ifndef UPDATER_UT
        int32_t ret;
        if (sessionParams_.isNeedBusinessError) {
            ret = doWorker_(sessionParams_.type, &businessError_);
        } else {
            ret = doWorker_(sessionParams_.type, context_);
        }
        PARAM_CHECK(ret == 0, return, "execute work return fail ret:%{public}d", ret);
#else
        doWorker_(sessionParams_.type, context_);
#endif
    }
    return;
}

// JS thread, which is used to notify the JS page upon completion of the operation.
void UpdateSession::CompleteWork(napi_env env, napi_status status, void *data)
{
    auto sess = reinterpret_cast<UpdateSession*>(data);
    PARAM_CHECK(sess != nullptr && sess->GetUpdateClient() != nullptr, return, "Session is null pointer");
    sess->CompleteWork(env, status);
    // If the share ptr is used, you can directly remove the share ptr.
    IUpdater *client = sess->GetUpdateClient();
    if (client != nullptr && !sess->IsAsyncCompleteWork()) {
        client->RemoveSession(sess->GetSessionId());
    }
}

// The C++ thread executes the synchronization operation. After the synchronization is complete,
// the CompleteWork is called to notify the JS page of the completion of the operation.
void UpdateSession::ExecuteWork(napi_env env, void *data)
{
    auto sess = reinterpret_cast<UpdateSession*>(data);
    PARAM_CHECK(sess != nullptr, return, "sess is null");
    sess->ExecuteWork(env);
}

napi_value UpdateAsyncession::StartWork(napi_env env, size_t startIndex, const napi_value *args)
{
    CLIENT_LOGI("UpdateAsyncession::StartWork startIndex: %{public}zu", startIndex);
    CLIENT_LOGI("UpdateAsyncession::totalArgc_ %{public}zu callbackNumber_: %{public}zu", totalArgc_, callbackNumber_);
    PARAM_CHECK_NAPI_CALL(env, args != nullptr && totalArgc_ >= startIndex, return nullptr, "Invalid para");
    napi_value workName = CreateWorkerName(env);
    PARAM_CHECK_NAPI_CALL(env, workName != nullptr, return nullptr, "Failed to worker name");

    // Check whether a callback exists. Only one callback is allowed.
    for (size_t i = 0; (i < (totalArgc_ - startIndex)) && (i < callbackNumber_); i++) {
        CLIENT_LOGI("CreateReference index:%u", static_cast<unsigned int>(i + startIndex));
        ClientStatus ret = NapiUtil::IsTypeOf(env, args[i + startIndex], napi_function);
        PARAM_CHECK_NAPI_CALL(env, ret == ClientStatus::CLIENT_SUCCESS, return nullptr, "invalid type");

        ret = NapiUtil::CreateReference(env, args[i + startIndex], 1, callbackRef_[i]);
        PARAM_CHECK_NAPI_CALL(env, ret == ClientStatus::CLIENT_SUCCESS, return nullptr, "Failed to create reference");
    }

    napi_status status = napi_ok;
    // Create an asynchronous call.
    if (!IsAsyncCompleteWork()) {
        status = napi_create_async_work(
            env, nullptr, workName, UpdateSession::ExecuteWork, UpdateSession::CompleteWork, this, &(worker_));
    } else {
        status = napi_create_async_work(
            env,
            nullptr,
            workName,
            UpdateSession::ExecuteWork,
            [](napi_env env, napi_status status, void *data) {},
            this,
            &(worker_));
    }

    PARAM_CHECK_NAPI_CALL(env, status == napi_ok, return nullptr, "Failed to create worker");

    // Put the thread in the task execution queue.
    status = napi_queue_async_work(env, worker_);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok, return nullptr, "Failed to queue worker");
    napi_value result;
    napi_create_int32(env, 0, &result);
    return result;
}

void UpdateAsyncession::NotifyJS(napi_env env, napi_value thisVar, const UpdateResult &result)
{
    CLIENT_LOGI("UpdateAsyncession::NotifyJS callbackNumber_: %{public}d", static_cast<int32_t>(callbackNumber_));

    napi_value callback;
    napi_value undefined;
    napi_value callResult;
    napi_get_undefined(env, &undefined);
    napi_value retArgs[RESULT_ARGC] = { 0 };

    uint32_t ret = (uint32_t)ClientHelper::BuildBusinessError(env, retArgs[0], result.businessError);
    ret |= (uint32_t)result.buildJSObject(env, retArgs[1], result);
    PARAM_CHECK_NAPI_CALL(env, ret == napi_ok, return, "Failed to build json");

    napi_status retStatus = napi_get_reference_value(env, callbackRef_[0], &callback);
    PARAM_CHECK_NAPI_CALL(env, retStatus == napi_ok, return, "Failed to get reference");
    const int callBackNumber = 2;
    retStatus = napi_call_function(env, undefined, callback, callBackNumber, retArgs, &callResult);
    // Release resources.
    for (size_t i = 0; i < callbackNumber_; i++) {
        napi_delete_reference(env, callbackRef_[i]);
        callbackRef_[i] = nullptr;
    }
    napi_delete_async_work(env, worker_);
    worker_ = nullptr;
}

void UpdateAsyncession::CompleteWork(napi_env env, napi_status status)
{
    CLIENT_LOGI("UpdateAsyncession::CompleteWork callbackNumber_: %{public}d, %{public}d",
        static_cast<int32_t>(callbackNumber_),
        sessionParams_.type);
    if (IsAsyncCompleteWork()) {
        return;
    }
    UpdateResult result;
    GetUpdateResult(result);
    NotifyJS(env, NULL, result);
}

void UpdateAsyncessionNoCallback::CompleteWork(napi_env env, napi_status status)
{
    CLIENT_LOGI("UpdateAsyncessionNoCallback::CompleteWork callbackNumber_: %d",
        static_cast<int32_t>(callbackNumber_));
}

napi_value UpdatePromiseSession::StartWork(napi_env env, size_t startIndex, const napi_value *args)
{
    CLIENT_LOGI("UpdatePromiseSession::StartWork");
    PARAM_CHECK_NAPI_CALL(env, args != nullptr, return nullptr, "Invalid para");
    napi_value workName = CreateWorkerName(env);
    PARAM_CHECK_NAPI_CALL(env, workName != nullptr, return nullptr, "Failed to worker name");

    napi_value promise;
    napi_status status = napi_create_promise(env, &deferred_, &promise);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok, return nullptr, "Failed to napi_create_promise");

    // Create an asynchronous call.
    status = napi_create_async_work(env, nullptr, workName, UpdateSession::ExecuteWork,
        UpdateSession::CompleteWork, this, &(worker_));
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok, return nullptr, "Failed to napi_create_async_work");
    // Put the thread in the task execution queue.
    status = napi_queue_async_work(env, worker_);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok, return nullptr, "Failed to napi_queue_async_work");
    return promise;
}

void UpdatePromiseSession::NotifyJS(napi_env env, napi_value thisVar, const UpdateResult &result)
{
    int32_t errorNum = static_cast<int32_t>(result.businessError.errorNum);
    CLIENT_LOGI("UpdatePromiseSession NotifyJS errorNum:%{public}d", errorNum);

    // Get the return result.
    napi_value processResult;
    const BusinessError &businessError = result.businessError;
    if (!UpdateHelper::IsErrorExist(businessError)) {
        result.buildJSObject(env, processResult, result);
        napi_resolve_deferred(env, deferred_, processResult);
    } else {
        ClientHelper::BuildBusinessError(env, processResult, businessError);
        napi_reject_deferred(env, deferred_, processResult);
    }
    napi_delete_async_work(env, worker_);
    worker_ = nullptr;
}

void UpdatePromiseSession::CompleteWork(napi_env env, napi_status status)
{
    CLIENT_LOGI("UpdatePromiseSession::CompleteWork status: %d", static_cast<int32_t>(status));
    if (IsAsyncCompleteWork()) {
        return;
    }
    UpdateResult result;
    GetUpdateResult(result);
    NotifyJS(env, NULL, result);
}

napi_value UpdateListener::StartWork(napi_env env, size_t startIndex, const napi_value *args)
{
    CLIENT_LOGI("UpdateListener::StartWork");
    PARAM_CHECK_NAPI_CALL(env, args != nullptr && totalArgc_ > startIndex, return nullptr, "Invalid para");

    if (NapiUtil::IsTypeOf(env, args[0], napi_string) == ClientStatus::CLIENT_SUCCESS) {
        int ret = NapiUtil::GetString(env, args[0], eventType_);
        PARAM_CHECK_NAPI_CALL(env, ret == napi_ok, return nullptr, "Failed to get string event type");
    } else {
        ClientStatus ret = ClientHelper::GetEventClassifyInfoFromArg(env, args[0], eventClassifyInfo_);
        PARAM_CHECK_NAPI_CALL(env, ret == ClientStatus::CLIENT_SUCCESS, return nullptr, "Failed to get obj event type");
    }

    PARAM_CHECK_NAPI_CALL(env, NapiUtil::IsTypeOf(env, args[startIndex], napi_function) == ClientStatus::CLIENT_SUCCESS,
        return nullptr, "Invalid callback type");
    ClientStatus ret = NapiUtil::CreateReference(env, args[startIndex], 1, handlerRef_);
    PARAM_CHECK_NAPI_CALL(env, ret == ClientStatus::CLIENT_SUCCESS, return nullptr, "Failed to create reference");

    napi_value result;
    napi_create_int32(env, 0, &result);
    return result;
}

void UpdateListener::NotifyJS(napi_env env, napi_value thisVar, const UpdateResult &result)
{
    CLIENT_LOGI("NotifyJS");
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
    CLIENT_LOGI("NotifyJS, eventId:%{public}d", eventInfo.eventId);
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
        CLIENT_LOGE("NotifyJS error, napi_call_function fail");
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
        CLIENT_LOGI("not same listener, different event classify, 0x%{public}x, 0x%{public}x",
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
    CLIENT_LOGI("RemoveHandlerRef handlerRef_:%{public}p %{public}u", handlerRef_, GetSessionId());
    napi_delete_reference(env, handlerRef_);
    handlerRef_ = nullptr;
}
} // namespace UpdateEngine
} // namespace OHOS