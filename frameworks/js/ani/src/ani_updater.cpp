/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ani_updater.h"

#include <thread>

#include "taihe/runtime.hpp"

#include "ani_common_conveter.h"
#include "task_info.h"
#include "update_service_kits.h"

namespace OHOS::UpdateService{
AniUpdater::AniUpdater(const UpgradeInfo &info) : upgradeInfo_(info)
{
    ENGINE_LOGI("AniUpdater construct upgradeInfo = %{public}s", info.ToString().c_str());
}

ohos::update::CheckResult AniUpdater::CheckNewVersionSync()
{
    BusinessError error;
    CheckResult result;
    int32_t funcResult;
    const auto ret = UpdateServiceKits::GetInstance().CheckNewVersion(upgradeInfo_, error, result, funcResult);
    SetError(ret, "checkNewVersion", error);
    return AniCommonConverter::Converter(result);
}

ohos::update::NewVersionInfo AniUpdater::GetNewVersionInfoSync()
{
    BusinessError error;
    NewVersionInfo info;
    int32_t funcResult;
    const auto ret = UpdateServiceKits::GetInstance().GetNewVersionInfo(upgradeInfo_, info, error, funcResult);
    SetError(ret, "getNewVersionInfo", error);
    return AniCommonConverter::Converter(info);
}

taihe::array<ohos::update::ComponentDescription> AniUpdater::GetNewVersionDescriptionSync(
    const ohos::update::VersionDigestInfo &versionDigestInfo,
    const ohos::update::DescriptionOptions &descriptionOptions)
{
    BusinessError error;
    VersionDescriptionInfo result;
    int32_t funcResult;
    const auto ret = UpdateServiceKits::GetInstance().GetNewVersionDescription(upgradeInfo_,
        AniCommonConverter::Converter(versionDigestInfo), AniCommonConverter::Converter(descriptionOptions), result,
        error, funcResult);
    SetError(ret, "getNewVersionDescription", error);
    return AniCommonConverter::Converter(result.componentDescriptions);
}

ohos::update::CurrentVersionInfo AniUpdater::GetCurrentVersionInfoSync()
{
    BusinessError error;
    CurrentVersionInfo info;
    int32_t funcResult;
    const auto ret = UpdateServiceKits::GetInstance().GetCurrentVersionInfo(upgradeInfo_, info, error, funcResult);
    SetError(ret, "getCurrentVersionInfo", error);
    return AniCommonConverter::Converter(info);
}

taihe::array<ohos::update::ComponentDescription> AniUpdater::GetCurrentVersionDescriptionSync(
    const ohos::update::DescriptionOptions &descriptionOptions)
{
    VersionDescriptionInfo currentVersionDescriptionInfo;
    BusinessError error;
    int32_t funcResult;
    const auto ret = UpdateServiceKits::GetInstance().GetCurrentVersionDescription(upgradeInfo_,
        AniCommonConverter::Converter(descriptionOptions), currentVersionDescriptionInfo, error, funcResult);
    SetError(ret, "getCurrentVersionDescription", error);
    return AniCommonConverter::Converter(currentVersionDescriptionInfo.componentDescriptions);
}

ohos::update::TaskInfo AniUpdater::GetTaskInfoSync()
{
    TaskInfo taskInfo;
    BusinessError businessError;
    int32_t funcResult;
    const auto ret = UpdateServiceKits::GetInstance().GetTaskInfo(upgradeInfo_, taskInfo, businessError, funcResult);
    SetError(ret, "getTaskInfo", businessError);
    return AniCommonConverter::Converter(taskInfo);
}

void AniUpdater::DownloadSync(const ohos::update::VersionDigestInfo &versionDigestInfo,
    const ohos::update::DownloadOptions &downloadOptions)
{
    BusinessError businessError;
    int32_t funcResult;
    const auto ret =
        UpdateServiceKits::GetInstance().Download(upgradeInfo_, AniCommonConverter::Converter(versionDigestInfo),
        AniCommonConverter::Converter(downloadOptions), businessError, funcResult);
    SetError(ret, "download", businessError);
}

void AniUpdater::ResumeDownloadSync(const ohos::update::VersionDigestInfo &versionDigestInfo,
    const ohos::update::ResumeDownloadOptions &resumeDownloadOptions)
{
    BusinessError businessError;
    int32_t funcResult;
    const auto ret =
        UpdateServiceKits::GetInstance().ResumeDownload(upgradeInfo_, AniCommonConverter::Converter(versionDigestInfo),
        AniCommonConverter::Converter(resumeDownloadOptions), businessError, funcResult);
    SetError(ret, "resumeDownload", businessError);
}

void AniUpdater::PauseDownloadSync(const ohos::update::VersionDigestInfo &versionDigestInfo,
    const ohos::update::PauseDownloadOptions &pauseDownloadOptions)
{
    BusinessError businessError;
    int32_t funcResult;
    const auto ret =
        UpdateServiceKits::GetInstance().PauseDownload(upgradeInfo_, AniCommonConverter::Converter(versionDigestInfo),
        AniCommonConverter::Converter(pauseDownloadOptions), businessError, funcResult);
    SetError(ret, "pauseDownload", businessError);
}

void AniUpdater::UpgradeSync(const ohos::update::VersionDigestInfo &versionDigestInfo,
    const ohos::update::UpgradeOptions &upgradeOptions)
{
    BusinessError businessError;
    int32_t funcResult;
    const auto ret = UpdateServiceKits::GetInstance().Upgrade(upgradeInfo_,
        AniCommonConverter::Converter(versionDigestInfo), AniCommonConverter::Converter(upgradeOptions), businessError,
        funcResult);
    SetError(ret, "upgrade", businessError);
}

void AniUpdater::ClearErrorSync(const ohos::update::VersionDigestInfo &versionDigestInfo,
    const ohos::update::ClearOptions &clearOptions)
{
    BusinessError businessError;
    int32_t funcResult;
    const auto ret = UpdateServiceKits::GetInstance().ClearError(upgradeInfo_,
        AniCommonConverter::Converter(versionDigestInfo), AniCommonConverter::Converter(clearOptions), businessError,
        funcResult);
    SetError(ret, "clearError", businessError);
}

ohos::update::UpgradePolicy AniUpdater::GetUpgradePolicySync()
{
    BusinessError businessError;
    UpgradePolicy policy;
    int32_t funcResult;

    const auto ret = UpdateServiceKits::GetInstance().GetUpgradePolicy(upgradeInfo_, policy, businessError, funcResult);
    SetError(ret, "getUpgradePolicy", businessError);
    return AniCommonConverter::Converter(policy);
}

void AniUpdater::SetUpgradePolicySync(const ohos::update::UpgradePolicy &policy)
{
    BusinessError businessError;
    int32_t funcResult;
    const auto ret = UpdateServiceKits::GetInstance().SetUpgradePolicy(upgradeInfo_,
        AniCommonConverter::Converter(policy), businessError, funcResult);
    SetError(ret, "setUpgradePolicy", businessError);
}

void AniUpdater::TerminateUpgradeSync()
{
    BusinessError businessError;
    int32_t funcResult;
    const auto ret = UpdateServiceKits::GetInstance().TerminateUpgrade(upgradeInfo_, businessError, funcResult);
    if (!businessError.IsSuccess()) {
        SetError(ret, "terminateUpgrade", businessError);
    }
}

void AniUpdater::RegisterCallback()
{
    ENGINE_LOGI("AniUpdater::RegisterCallback");
    const UpdateCallbackInfo callback{ [=](const EventInfo &eventInfo) { CallbackEventInfo(eventInfo); } };
    constexpr int32_t sleepDuration = 10; // 回调注册失败后再次尝试之前的等待时间，单位：毫秒
    constexpr int32_t maxRetryTimes = 5;  // 回调注册失败最大尝试次数
    int32_t retryTimes = 0;
    int32_t funcResult;
    do {
        int32_t ret = UpdateServiceKits::GetInstance().RegisterUpdateCallback(upgradeInfo_, callback, funcResult);
        if (ret == INT_CALL_SUCCESS) {
            break;
        }

        if (retryTimes++ < maxRetryTimes) {
            ENGINE_LOGI("InitCallback fail, will retry after %{public}d milliseconds", sleepDuration);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
        } else {
            ENGINE_LOGE("InitCallback fail after retry %{public}d times", retryTimes);
        }
    } while (retryTimes < maxRetryTimes);
}

void AniUpdater::UnRegisterCallback()
{
    int32_t funcResult;
    UpdateServiceKits::GetInstance().UnregisterUpdateCallback(upgradeInfo_, funcResult);
}
}
