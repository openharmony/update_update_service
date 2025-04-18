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

#include "iupdater.h"

#include "napi_common_define.h"
#include "update_session.h"

namespace OHOS::UpdateService {
napi_value IUpdater::On(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_ARGC;
    napi_value args[MAX_ARGC] = { 0 };
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok && argc >= ARG_NUM_TWO, return nullptr, "Error get cb info");

    bool isCallerValid = NapiCommonUtils::IsCallerValid();
    PARAM_CHECK_NAPI_CALL(env, isCallerValid, NapiCommonUtils::NapiThrowNotSystemAppError(env);
        return nullptr, "Caller not system app.");

    EventClassifyInfo eventClassifyInfo;
    ClientStatus ret = ClientHelper::GetEventClassifyInfoFromArg(env, args[0], eventClassifyInfo);
    std::vector<std::pair<std::string, std::string>> paramInfos;
    paramInfos.push_back({ "eventClassifyInfo", "EventClassifyInfo" });
    PARAM_CHECK_NAPI_CALL(env, ret == ClientStatus::CLIENT_SUCCESS,
        NapiCommonUtils::NapiThrowParamError(env, paramInfos);
        return nullptr, "Error get eventClassifyInfo");
    PARAM_CHECK(sessionsMgr_->FindSessionByHandle(env, eventClassifyInfo, args[1]) == nullptr, return nullptr,
        "Handle has been sub");

    SessionParams sessionParams(SessionType::SESSION_SUBSCRIBE, CALLBACK_POSITION_TWO);
    std::shared_ptr<BaseSession> sess = std::make_shared<UpdateListener>(this, sessionParams, argc, false);
    PARAM_CHECK_NAPI_CALL(env, sess != nullptr, return nullptr, "Failed to create listener");

    sessionsMgr_->AddSession(sess);
    napi_value retValue = sess->StartWork(
        env, args,
        [&](void *context) -> int {
            RegisterCallback();
            return 0;
        },
        nullptr);
    PARAM_CHECK(retValue != nullptr, sessionsMgr_->RemoveSession(sess->GetSessionId()); return nullptr,
        "Failed to SubscribeEvent.");
    return retValue;
}

napi_value IUpdater::Off(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_ARGC;
    napi_value args[MAX_ARGC] = { 0 };
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok, return nullptr, "Error get cb info");

    bool isCallerValid = NapiCommonUtils::IsCallerValid();
    PARAM_CHECK_NAPI_CALL(env, isCallerValid, NapiCommonUtils::NapiThrowNotSystemAppError(env);
        return nullptr, "Caller not system app.");

    EventClassifyInfo eventClassifyInfo;
    ClientStatus ret = ClientHelper::GetEventClassifyInfoFromArg(env, args[0], eventClassifyInfo);
    std::vector<std::pair<std::string, std::string>> paramInfos;
    paramInfos.push_back({ "eventClassifyInfo", "EventClassifyInfo" });
    PARAM_CHECK_NAPI_CALL(env, ret == ClientStatus::CLIENT_SUCCESS,
        NapiCommonUtils::NapiThrowParamError(env, paramInfos);
        return nullptr, "Error get eventClassifyInfo");

    napi_value handle = nullptr;
    if (argc >= ARG_NUM_TWO) {
        ret = NapiCommonUtils::IsTypeOf(env, args[1], napi_function);
        std::vector<std::pair<std::string, std::string>> paramErrors;
        paramErrors.push_back({ "callback", "napi_function" });
        PARAM_CHECK_NAPI_CALL(env, ret == ClientStatus::CLIENT_SUCCESS,
            NapiCommonUtils::NapiThrowParamError(env, paramErrors);
            return nullptr, "invalid type");
        handle = args[1];
    }
    sessionsMgr_->Unsubscribe(eventClassifyInfo, handle);
    UnRegisterCallback();
    napi_value result;
    napi_create_int32(env, 0, &result);
    return result;
}

void IUpdater::GetUpdateResult(uint32_t type, UpdateResult &result)
{
    result.buildJSObject = ClientHelper::BuildUndefinedStatus;
}

napi_value IUpdater::StartSession(napi_env env, napi_callback_info info, SessionParams &sessionParams,
    BaseSession::DoWorkFunction function)
{
    size_t argc = MAX_ARGC;
    napi_value args[MAX_ARGC] = { 0 };
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok, return nullptr, "Error get cb info");

    ENGINE_LOGI("StartSession type %{public}d argc %{public}zu callbackStartIndex %{public}d",
        static_cast<int32_t>(sessionParams.type), argc, static_cast<int>(sessionParams.callbackStartIndex));
    std::shared_ptr<BaseSession> sess = nullptr;
    if (argc > sessionParams.callbackStartIndex) {
        sess = std::make_shared<UpdateAsyncession>(this, sessionParams, argc);
    } else {
        sess = std::make_shared<UpdatePromiseSession>(this, sessionParams, argc);
    }
    PARAM_CHECK_NAPI_CALL(env, sess != nullptr, return nullptr, "Failed to create update session");
    sessionsMgr_->AddSession(sess);
    napi_value retValue = sess->StartWork(env, args, function, nullptr);
    PARAM_CHECK(retValue != nullptr, sessionsMgr_->RemoveSession(sess->GetSessionId()); return nullptr,
        "Failed to start worker.");
    return retValue;
}

napi_value IUpdater::StartParamErrorSession(napi_env env, napi_callback_info info, CALLBACK_POSITION callbackPosition)
{
    SessionParams sessionParams(SessionType::SESSION_REPLY_PARAM_ERROR, callbackPosition, true);
    return StartSession(env, info, sessionParams, [](void *context) -> int {
            return INT_PARAM_ERR;
        });
}

void IUpdater::NotifyEventInfo(const EventInfo &eventInfo)
{
    ENGINE_LOGI("NotifyEventInfo 0x%{public}08x", eventInfo.eventId);
    auto classify = EventClassify::TASK;
    for (const auto item : g_eventClassifyList) {
        if (CAST_UINT(eventInfo.eventId) & CAST_UINT(item)) {
            classify = item;
            break;
        }
    }
    EventClassifyInfo eventClassifyInfo(classify);
    sessionsMgr_->Emit(eventClassifyInfo, eventInfo);
}
} // namespace OHOS::UpdateService