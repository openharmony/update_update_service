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

#include "update_client.h"
#include "client_helper.h"
#include "iupdate_service.h"
#include "napi_util.h"
#include "update_helper.h"
#include "update_service_kits.h"
#include "update_session.h"

using namespace std;

namespace OHOS {
namespace UpdateEngine {
napi_value UpdateClient::Napi::NapiOn(napi_env env, napi_callback_info info)
{
    CLIENT_LOGI("UpdateClient::Napi::NapiOn");
    UpdateClient *updater = UnwrapJsObject<UpdateClient>(env, info);
    PARAM_CHECK_NAPI_CALL(env, updater != nullptr, return nullptr, "Error get UpdateClient");
    return updater->On(env, info);
}

napi_value UpdateClient::Napi::NapiOff(napi_env env, napi_callback_info info)
{
    CLIENT_LOGI("UpdateClient::Napi::NapiOff");
    UpdateClient *updater = UnwrapJsObject<UpdateClient>(env, info);
    PARAM_CHECK_NAPI_CALL(env, updater != nullptr, return nullptr, "Error get UpdateClient");
    return updater->Off(env, info);
}

UpdateClient::UpdateClient(napi_env env, napi_value thisVar)
{
    napi_ref thisReference = nullptr;
    napi_create_reference(env, thisVar, 1, &thisReference);
    sessionsMgr_ = std::make_shared<SessionManager>(env, thisReference);
}

UpdateClient::~UpdateClient()
{
    if (sessionsMgr_ != nullptr) {
        sessionsMgr_->Clear();
    }
}

void UpdateClient::NapiThrowError(napi_env env, CallResult errCode, std::string paraName)
{
    BusinessError businessError;
    std::string errMsg = "BusinessError " + std::to_string(static_cast<int32_t>(errCode))
        + ": Parameter error. The type of ${" + paraName + "}"
        + "must be ${正确的类型} [or ${其他正确的输入}].";
    businessError.Build(errCode, errMsg);
    napi_value msg = ClientHelper::BuildThrowError(env, businessError);
    napi_status status = napi_throw(env, msg);
    PARAM_CHECK(status == napi_ok, return, "Failed to napi_throw %d", static_cast<int32_t>(status));
}

napi_value UpdateClient::GetOnlineUpdater(napi_env env, napi_callback_info info)
{
    napi_value result;
    napi_create_int32(env, 0, &result);
    size_t argc = MAX_ARGC;
    napi_value args[MAX_ARGC] = {0};
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok,
        NapiThrowError(env, CallResult::PARAM_ERR, "callback info"),
        "Error get cb info");
    PARAM_CHECK_NAPI_CALL(env, argc >= 1, NapiThrowError(env, CallResult::PARAM_ERR, "param"),
        "Invalid param");
    PARAM_CHECK_NAPI_CALL(env, !isInit_, return result, "Has been init");

    ClientStatus ret = ClientHelper::GetUpgradeInfoFromArg(env, args[0], upgradeInfo_);
    PARAM_CHECK(ret == ClientStatus::CLIENT_SUCCESS,
        NapiThrowError(env, CallResult::PARAM_ERR, "upgradeInfo param"),
        "Failed to get upgradeInfo param");

    UpdateCallbackInfo callback {
        [=](const BusinessError &businessError, const CheckResultEx &checkResultEx) {
            NotifyCheckVersionDone(businessError, checkResultEx);
        },
        [=](const EventInfo &eventInfo) {
            NotifyEventInfo(eventInfo);
        }
    };
    UpdateServiceKits::GetInstance().RegisterUpdateCallback(upgradeInfo_, callback);
    isInit_ = true;
    return result;
}

napi_value UpdateClient::CheckNewVersion(napi_env env, napi_callback_info info)
{
    versionInfo_.status = SYSTEM_ERROR;
    SessionParams sessionParams(SessionType::SESSION_CHECK_VERSION, CALLBACK_POSITION_ONE, false, true);
    napi_value ret = StartSession(env, info, sessionParams, [=](SessionType type, void *context) -> int {
        return UpdateServiceKits::GetInstance().CheckNewVersion(upgradeInfo_);
    });
    PARAM_CHECK(ret != nullptr, return nullptr, "Failed to start worker.");
    return ret;
}

napi_value UpdateClient::CancelUpgrade(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_ARGC;
    napi_value args[MAX_ARGC] = {0};
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok && argc == 0,
        NapiThrowError(env, CallResult::PARAM_ERR, "callback info"), "Error get cb info");
    CLIENT_LOGI("CancelUpgrade");
    SessionParams sessionParams(SessionType::SESSION_CANCEL_UPGRADE, CALLBACK_POSITION_ONE, true);
    std::shared_ptr<UpdateSession> sess = nullptr;
    sess = std::make_shared<UpdateAsyncessionNoCallback>(this, sessionParams, argc);
    PARAM_CHECK_NAPI_CALL(env, sess != nullptr, return nullptr, "Failed to create update session");
    sessionsMgr_->AddSession(sess);
    napi_value retValue = sess->StartWork(
        env,
        args,
        [=](SessionType type, void *context) -> int {
            BusinessError *businessError = reinterpret_cast<BusinessError *>(context);
            return UpdateServiceKits::GetInstance().Cancel(upgradeInfo_, IUpdateService::DOWNLOAD, *businessError);
        },
        nullptr);
    PARAM_CHECK(retValue != nullptr, sessionsMgr_->RemoveSession(sess->GetSessionId());
        return nullptr, "Failed to start worker.");
    return retValue;
}

template <typename T>
ClientStatus UpdateClient::ParseUpgOptions(napi_env env, napi_callback_info info, VersionDigestInfo &versionDigestInfo,
    T &options)
{
    size_t argc = MAX_ARGC;
    napi_value args[MAX_ARGC] = {0};
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok,
        NapiThrowError(env, CallResult::PARAM_ERR, "callback info"), "Error get cb info");

    ClientStatus ret = ClientHelper::GetVersionDigestInfoFromArg(env, args[0], versionDigestInfo);
    PARAM_CHECK(ret == ClientStatus::CLIENT_SUCCESS,
        NapiThrowError(env, CallResult::PARAM_ERR, "versionDigestInfo param"),
        "Failed to get versionDigestInfo param");

    ret = ClientHelper::GetOptionsFromArg(env, args[1], options);
    PARAM_CHECK(ret == ClientStatus::CLIENT_SUCCESS,
        NapiThrowError(env, CallResult::PARAM_ERR, "Options param"),
        "Failed to get Options param");
    return ClientStatus::CLIENT_SUCCESS;
}

napi_value UpdateClient::Download(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_ARGC;
    napi_value args[MAX_ARGC] = {0};
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok, return nullptr, "Error get cb info");
    CLIENT_LOGI("Download");

    ClientStatus ret = ParseUpgOptions(env, info, versionDigestInfo_, downloadOptions_);
    PARAM_CHECK(ret == ClientStatus::CLIENT_SUCCESS,
        NapiThrowError(env, CallResult::PARAM_ERR, "Download param"),
        "Failed to get Download param");

    SessionParams sessionParams(SessionType::SESSION_DOWNLOAD, CALLBACK_POSITION_THREE, true);
    napi_value retValue = StartSession(env, info, sessionParams, [=](SessionType type, void *context) -> int {
        BusinessError *businessError = reinterpret_cast<BusinessError *>(context);
        return UpdateServiceKits::GetInstance().Download(
            upgradeInfo_, versionDigestInfo_, downloadOptions_, *businessError);
    });
    PARAM_CHECK(retValue != nullptr, return nullptr, "Failed to start worker.");
    return retValue;
}

napi_value UpdateClient::PauseDownload(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_ARGC;
    napi_value args[MAX_ARGC] = {0};
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok,
        NapiThrowError(env, CallResult::PARAM_ERR, "callback info"),
        "Error get cb info");

    ClientStatus ret = ParseUpgOptions(env, info, versionDigestInfo_, pauseDownloadOptions_);
    PARAM_CHECK(ret == ClientStatus::CLIENT_SUCCESS,
        NapiThrowError(env, CallResult::PARAM_ERR, "pauseDownloadOptions param"),
        "Failed to get pauseDownloadOptions param");

    SessionParams sessionParams(SessionType::SESSION_PAUSE_DOWNLOAD, CALLBACK_POSITION_THREE, true);
    napi_value retValue = StartSession(env, info, sessionParams, [=](SessionType type, void *context) -> int {
        BusinessError *businessError = reinterpret_cast<BusinessError *>(context);
        return UpdateServiceKits::GetInstance().PauseDownload(
            upgradeInfo_, versionDigestInfo_, pauseDownloadOptions_, *businessError);
    });
    PARAM_CHECK(retValue != nullptr, return nullptr, "Failed to start worker.");
    return retValue;
}

napi_value UpdateClient::ResumeDownload(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_ARGC;
    napi_value args[MAX_ARGC] = {0};
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok,
        NapiThrowError(env, CallResult::PARAM_ERR, "callback info"),
        "Error get cb info");

    ClientStatus ret = ParseUpgOptions(env, info, versionDigestInfo_, resumeDownloadOptions_);
    PARAM_CHECK(ret == ClientStatus::CLIENT_SUCCESS,
        NapiThrowError(env, CallResult::PARAM_ERR, "resumeDownloadOptions param"),
        "Failed to get resumeDownloadOptions param");

    SessionParams sessionParams(SessionType::SESSION_RESUME_DOWNLOAD, CALLBACK_POSITION_THREE, true);
    napi_value retValue = StartSession(env, info, sessionParams, [=](SessionType type, void *context) -> int {
        BusinessError *businessError = reinterpret_cast<BusinessError *>(context);
        return UpdateServiceKits::GetInstance().ResumeDownload(
            upgradeInfo_, versionDigestInfo_, resumeDownloadOptions_, *businessError);
    });
    PARAM_CHECK(retValue != nullptr, return nullptr, "Failed to start worker.");
    return retValue;
}

napi_value UpdateClient::Upgrade(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_ARGC;
    napi_value args[MAX_ARGC] = {0};
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok,
        NapiThrowError(env, CallResult::PARAM_ERR, "callback info"),
        "Error get cb info");

    ClientStatus ret = ParseUpgOptions(env, info, versionDigestInfo_, upgradeOptions_);
    PARAM_CHECK(ret == ClientStatus::CLIENT_SUCCESS,
        NapiThrowError(env, CallResult::PARAM_ERR, "resumeDownloadOptions param"),
        "Failed to get resumeDownloadOptions param");

    SessionParams sessionParams(SessionType::SESSION_UPGRADE, CALLBACK_POSITION_THREE, true);
    napi_value retValue = StartSession(env, info, sessionParams, [=](SessionType type, void *context) -> int {
#ifndef UPDATER_API_TEST
        BusinessError *businessError = reinterpret_cast<BusinessError *>(context);
        return UpdateServiceKits::GetInstance().Upgrade(
            upgradeInfo_, versionDigestInfo_, upgradeOptions_, *businessError);
#else
        return INT_CALL_SUCCESS;
#endif
    });
    PARAM_CHECK(retValue != nullptr, return nullptr, "Failed to start worker.");
    return retValue;
}

napi_value UpdateClient::ClearError(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_ARGC;
    napi_value args[MAX_ARGC] = {0};
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok,
        NapiThrowError(env, CallResult::PARAM_ERR, "callback info"),
        "Error get cb info");

    ClientStatus ret = ParseUpgOptions(env, info, versionDigestInfo_, clearOptions_);
    PARAM_CHECK(ret == ClientStatus::CLIENT_SUCCESS,
        NapiThrowError(env, CallResult::PARAM_ERR, "clearOptions param"),
        "Failed to get clearOptions param");

    SessionParams sessionParams(SessionType::SESSION_CLEAR_ERROR, CALLBACK_POSITION_THREE, true);
    napi_value retValue = StartSession(env, info, sessionParams,
        [=](SessionType type, void *context) -> int {
            BusinessError *businessError = reinterpret_cast<BusinessError*>(context);
            return UpdateServiceKits::GetInstance().ClearError(
                upgradeInfo_, versionDigestInfo_, clearOptions_, *businessError);
    });
    PARAM_CHECK(retValue != nullptr, return nullptr, "Failed to start worker.");
    return retValue;
}

napi_value UpdateClient::TerminateUpgrade(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_ARGC;
    napi_value args[MAX_ARGC] = {0};
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok,
        NapiThrowError(env, CallResult::PARAM_ERR, "callback info"),
        "Error get cb info");

    SessionParams sessionParams(SessionType::SESSION_TERMINATE_UPGRADE, CALLBACK_POSITION_ONE, true);
    napi_value retValue = StartSession(env, info, sessionParams, [=](SessionType type, void *context) -> int {
        BusinessError *businessError = reinterpret_cast<BusinessError *>(context);
        return UpdateServiceKits::GetInstance().TerminateUpgrade(upgradeInfo_, *businessError);
    });
    PARAM_CHECK(retValue != nullptr, return nullptr, "Failed to start worker.");
    return retValue;
}

napi_value UpdateClient::SetUpgradePolicy(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_ARGC;
    napi_value args[MAX_ARGC] = {0};
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok,
        NapiThrowError(env, CallResult::PARAM_ERR, "callback info"),
        "Error get cb info");

    ClientStatus ret = ClientHelper::GetUpgradePolicyFromArg(env, args[0], upgradePolicy_);
    PARAM_CHECK(ret == ClientStatus::CLIENT_SUCCESS,
        NapiThrowError(env, CallResult::PARAM_ERR, "upgradePolicy param"),
        "Failed to get upgradePolicy param");

    SessionParams sessionParams(SessionType::SESSION_SET_POLICY, CALLBACK_POSITION_TWO, true);
    napi_value retValue = StartSession(env, info, sessionParams,
        [&](SessionType type, void *context) -> int {
            BusinessError *businessError = reinterpret_cast<BusinessError*>(context);
            return UpdateServiceKits::GetInstance().SetUpgradePolicy(upgradeInfo_, upgradePolicy_, *businessError);
    });
    PARAM_CHECK(retValue != nullptr, return nullptr, "Failed to SetUpgradePolicy.");
    return retValue;
}

napi_value UpdateClient::GetUpgradePolicy(napi_env env, napi_callback_info info)
{
    SessionParams sessionParams(SessionType::SESSION_GET_POLICY, CALLBACK_POSITION_ONE, true);
    napi_value retValue = StartSession(env, info, sessionParams,
        [=](SessionType type, void *context) -> int {
            BusinessError *businessError = reinterpret_cast<BusinessError*>(context);
            return UpdateServiceKits::GetInstance().GetUpgradePolicy(upgradeInfo_, upgradePolicy_, *businessError);
    });
    PARAM_CHECK(retValue != nullptr, return nullptr, "Failed to UpgradeVersion.");
    return retValue;
}

napi_value UpdateClient::GetNewVersionInfo(napi_env env, napi_callback_info info)
{
    SessionParams sessionParams(SessionType::SESSION_GET_NEW_VERSION, CALLBACK_POSITION_ONE, true);
    napi_value retValue = StartSession(env, info, sessionParams,
        [=](SessionType type, void *context) -> int {
            BusinessError *businessError = reinterpret_cast<BusinessError*>(context);
            return UpdateServiceKits::GetInstance().GetNewVersionInfo(upgradeInfo_, newVersionInfo_, *businessError);
    });
    PARAM_CHECK(retValue != nullptr, return nullptr, "Failed to GetNewVersionInfo.");
    return retValue;
}

napi_value UpdateClient::GetNewVersionDescription(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_ARGC;
    napi_value args[MAX_ARGC] = {0};
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok,
        NapiThrowError(env, CallResult::PARAM_ERR, "callback info"),
        "Error get cb info");
    CLIENT_LOGI("GetNewVersionDescription");

    ClientStatus ret = ParseUpgOptions(env, info, versionDigestInfo_, descriptionOptions_);
    PARAM_CHECK(ret == ClientStatus::CLIENT_SUCCESS,
        NapiThrowError(env, CallResult::PARAM_ERR, "GetNewVersionDescription param"),
        "Failed to get GetNewVersionDescription param");

    SessionParams sessionParams(SessionType::SESSION_GET_NEW_VERSION_DESCRIPTION, CALLBACK_POSITION_THREE, true);
    napi_value retValue = StartSession(env, info, sessionParams, [=](SessionType type, void *context) -> int {
        BusinessError *businessError = reinterpret_cast<BusinessError *>(context);
        return UpdateServiceKits::GetInstance().GetNewVersionDescription(
            upgradeInfo_, versionDigestInfo_, descriptionOptions_, newVersionDescriptionInfo_, *businessError);
    });
    PARAM_CHECK(retValue != nullptr, return nullptr, "Failed to start worker.");
    return retValue;
}

napi_value UpdateClient::GetCurrentVersionInfo(napi_env env, napi_callback_info info)
{
    SessionParams sessionParams(SessionType::SESSION_GET_CUR_VERSION, CALLBACK_POSITION_ONE, true);
    napi_value retValue = StartSession(env, info, sessionParams,
        [=](SessionType type, void *context) -> int {
            BusinessError *businessError = reinterpret_cast<BusinessError*>(context);
            return UpdateServiceKits::GetInstance().GetCurrentVersionInfo(upgradeInfo_, currentVersionInfo_,
                *businessError);
    });
    PARAM_CHECK(retValue != nullptr, return nullptr, "Failed to GetCurrentVersionInfo.");
    return retValue;
}

napi_value UpdateClient::GetCurrentVersionDescription(napi_env env, napi_callback_info info)
{
    size_t argc = MAX_ARGC;
    napi_value args[MAX_ARGC] = {0};
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok,
        NapiThrowError(env, CallResult::PARAM_ERR, "callback info"),
        "Error get cb info");
    CLIENT_LOGI("GetCurrentVersionDescription");

    ClientStatus ret = ParseUpgOptions(env, info, versionDigestInfo_, descriptionOptions_);
    PARAM_CHECK(ret == ClientStatus::CLIENT_SUCCESS,
        NapiThrowError(env, CallResult::PARAM_ERR, "GetCurrentVersionDescription param"),
        "Failed to get GetCurrentVersionDescription param");

    SessionParams sessionParams(SessionType::SESSION_GET_CUR_VERSION_DESCRIPTION, CALLBACK_POSITION_TWO, true);
    napi_value retValue = StartSession(env, info, sessionParams, [=](SessionType type, void *context) -> int {
        BusinessError *businessError = reinterpret_cast<BusinessError *>(context);
        return UpdateServiceKits::GetInstance().GetCurrentVersionDescription(
            upgradeInfo_, descriptionOptions_, currentVersionDescriptionInfo_, *businessError);
    });
    PARAM_CHECK(retValue != nullptr, return nullptr, "Failed to start worker.");
    return retValue;
}

napi_value UpdateClient::GetTaskInfo(napi_env env, napi_callback_info info)
{
    SessionParams sessionParams(SessionType::SESSION_GET_TASK_INFO, CALLBACK_POSITION_ONE, true);
    napi_value retValue = StartSession(env, info, sessionParams,
        [=](SessionType type, void *context) -> int {
            BusinessError *businessError = reinterpret_cast<BusinessError*>(context);
            return UpdateServiceKits::GetInstance().GetTaskInfo(upgradeInfo_, taskInfo_, *businessError);
    });
    PARAM_CHECK(retValue != nullptr, return nullptr, "Failed to GetTaskInfo.");
    return retValue;
}

void UpdateClient::NotifyCheckVersionDone(const BusinessError &businessError, const CheckResultEx &checkResultEx)
{
    CLIENT_LOGI("NotifyCheckVersionDone businessError %{public}d", static_cast<int32_t> (businessError.errorNum));
    CLIENT_LOGI("NotifyCheckVersionDone isExistNewVersion %{public}d", checkResultEx.isExistNewVersion);
    checkResultEx_ = checkResultEx;
}

void UpdateClient::GetUpdateResult(SessionType type, UpdateResult &result)
{
    CLIENT_LOGI("GetUpdateResult type %{public}d", type);
    result.type = type;
    switch (type) {
        case SessionType::SESSION_CHECK_VERSION:
            result.result.checkResultEx = &checkResultEx_;
            result.buildJSObject = ClientHelper::BuildCheckResultEx;
            break;
        case SessionType::SESSION_GET_NEW_VERSION:
            result.result.newVersionInfo = &newVersionInfo_;
            result.buildJSObject = ClientHelper::BuildNewVersionInfo;
            break;
        case SessionType::SESSION_GET_NEW_VERSION_DESCRIPTION:
            result.result.versionDescriptionInfo = &newVersionDescriptionInfo_;
            result.buildJSObject = ClientHelper::BuildVersionDescriptionInfo;
            break;
        case SessionType::SESSION_GET_TASK_INFO:
            result.result.taskInfo = &taskInfo_;
            result.buildJSObject = ClientHelper::BuildTaskInfo;
            break;
        case SessionType::SESSION_GET_CUR_VERSION:
            result.result.currentVersionInfo = &currentVersionInfo_;
            result.buildJSObject = ClientHelper::BuildCurrentVersionInfo;
            break;
        case SessionType::SESSION_GET_CUR_VERSION_DESCRIPTION:
            result.result.versionDescriptionInfo = &currentVersionDescriptionInfo_;
            result.buildJSObject = ClientHelper::BuildVersionDescriptionInfo;
            break;
        case SessionType::SESSION_GET_POLICY:
            result.result.upgradePolicy = &upgradePolicy_;
            result.buildJSObject = ClientHelper::BuildUpgradePolicy;
            break;
        default:
            result.buildJSObject = ClientHelper::BuildUndefinedStatus;
            break;
    }
}
} // namespace UpdateEngine
} // namespace OHOS