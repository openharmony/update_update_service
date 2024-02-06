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

#ifndef UPDATE_SERVICE_KITS_IMPL_H
#define UPDATE_SERVICE_KITS_IMPL_H

#include "singleton.h"

#include "base_service_kits_impl.h"
#include "iupdate_callback.h"
#include "update_define.h"
#include "update_service_kits.h"
#include "update_callback.h"
#include "update_service_proxy.h"

namespace OHOS::UpdateEngine {
class UpdateServiceKitsImpl final : public UpdateServiceKits,
    public DelayedRefSingleton<UpdateServiceKitsImpl>,
    public BaseServiceKitsImpl<IUpdateService> {
    DECLARE_DELAYED_REF_SINGLETON(UpdateServiceKitsImpl);

public:
    DISALLOW_COPY_AND_MOVE(UpdateServiceKitsImpl);

    int32_t RegisterUpdateCallback(const UpgradeInfo &info, const UpdateCallbackInfo &cb) final;

    int32_t UnregisterUpdateCallback(const UpgradeInfo &info) final;

    int32_t CheckNewVersion(const UpgradeInfo &info, BusinessError &businessError, CheckResult &checkResult) final;

    int32_t Download(const UpgradeInfo &info, const VersionDigestInfo &versionDigestInfo,
        const DownloadOptions &downloadOptions, BusinessError &businessError) final;

    int32_t PauseDownload(const UpgradeInfo &info, const VersionDigestInfo &versionDigestInfo,
        const PauseDownloadOptions &pauseDownloadOptions, BusinessError &businessError) final;

    int32_t ResumeDownload(const UpgradeInfo &info, const VersionDigestInfo &versionDigestInfo,
        const ResumeDownloadOptions &resumeDownloadOptions, BusinessError &businessError) final;

    int32_t Upgrade(const UpgradeInfo &info, const VersionDigestInfo &versionDigest,
        const UpgradeOptions &upgradeOptions, BusinessError &businessError) final;

    int32_t ClearError(const UpgradeInfo &info, const VersionDigestInfo &versionDigest,
        const ClearOptions &clearOptions, BusinessError &businessError) final;

    int32_t TerminateUpgrade(const UpgradeInfo &info, BusinessError &businessError) final;

    int32_t GetNewVersionInfo(const UpgradeInfo &info, NewVersionInfo &newVersionInfo,
        BusinessError &businessError) final;

    int32_t GetNewVersionDescription(const UpgradeInfo &info, const VersionDigestInfo &versionDigestInfo,
        const DescriptionOptions &descriptionOptions, VersionDescriptionInfo &newVersionDescriptionInfo,
        BusinessError &businessError) final;

    int32_t GetCurrentVersionInfo(const UpgradeInfo &info, CurrentVersionInfo &currentVersionInfo,
        BusinessError &businessError) final;

    int32_t GetCurrentVersionDescription(const UpgradeInfo &info, const DescriptionOptions &descriptionOptions,
        VersionDescriptionInfo &currentVersionDescriptionInfo, BusinessError &businessError) final;

    int32_t GetTaskInfo(const UpgradeInfo &info, TaskInfo &taskInfo, BusinessError &businessError) final;

    int32_t SetUpgradePolicy(const UpgradeInfo &info, const UpgradePolicy &policy, BusinessError &businessError) final;

    int32_t GetUpgradePolicy(const UpgradeInfo &info, UpgradePolicy &policy, BusinessError &businessError) final;

    int32_t Cancel(const UpgradeInfo &info, int32_t service, BusinessError &businessError) final;

    int32_t FactoryReset(BusinessError &businessError) final;

    int32_t ApplyNewVersion(const UpgradeInfo &info, const std::string &miscFile, const std::string &packageName,
        BusinessError &businessError) final;

    int32_t VerifyUpgradePackage(const std::string &packagePath, const std::string &keyPath,
        BusinessError &businessError) final;

protected:
    void RegisterCallback() override;

#ifndef UPDATER_UT
private:
#endif
    std::map<UpgradeInfo, sptr<IUpdateCallback>> remoteUpdateCallbackMap_;
    UpgradeInfo upgradeInfo_{};
};
} // namespace OHOS::UpdateEngine
#endif // UPDATE_SERVICE_KITS_IMPL_H
