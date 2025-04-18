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

#ifndef ISERVICE_ONLINE_UPDATER_H
#define ISERVICE_ONLINE_UPDATER_H

#include "refbase.h"

#include "business_error.h"
#include "check_result.h"
#include "clear_options.h"
#include "current_version_info.h"
#include "description_options.h"
#include "download_options.h"
#include "iupdate_callback.h"
#include "pause_download_options.h"
#include "resume_download_options.h"
#include "new_version_info.h"
#include "upgrade_info.h"
#include "upgrade_options.h"
#include "upgrade_policy.h"
#include "task_info.h"
#include "version_description_info.h"
#include "version_digest_info.h"

namespace OHOS::UpdateService {
class IServiceOnlineUpdater : public virtual RefBase {
public:
    virtual ~IServiceOnlineUpdater() = default;

    virtual int32_t CheckNewVersion(const UpgradeInfo &info, BusinessError &businessError,
        CheckResult &checkResult) = 0;

    virtual int32_t Download(const UpgradeInfo &info, const VersionDigestInfo &versionDigestInfo,
        const DownloadOptions &downloadOptions, BusinessError &businessError) = 0;

    virtual int32_t PauseDownload(const UpgradeInfo &info, const VersionDigestInfo &versionDigestInfo,
        const PauseDownloadOptions &pauseDownloadOptions, BusinessError &businessError) = 0;

    virtual int32_t ResumeDownload(const UpgradeInfo &info, const VersionDigestInfo &versionDigestInfo,
        const ResumeDownloadOptions &resumeDownloadOptions, BusinessError &businessError) = 0;

    virtual int32_t Upgrade(const UpgradeInfo &info, const VersionDigestInfo &versionDigest,
        const UpgradeOptions &upgradeOptions, BusinessError &businessError) = 0;

    virtual int32_t ClearError(const UpgradeInfo &info, const VersionDigestInfo &versionDigest,
        const ClearOptions &clearOptions, BusinessError &businessError) = 0;

    virtual int32_t TerminateUpgrade(const UpgradeInfo &info, BusinessError &businessError) = 0;

    virtual int32_t GetNewVersionInfo(const UpgradeInfo &info, NewVersionInfo &newVersionInfo,
        BusinessError &businessError) = 0;

    virtual int32_t GetNewVersionDescription(const UpgradeInfo &info, const VersionDigestInfo &versionDigestInfo,
        const DescriptionOptions &descriptionOptions, VersionDescriptionInfo &newVersionDescriptionInfo,
        BusinessError &businessError) = 0;

    virtual int32_t GetCurrentVersionInfo(const UpgradeInfo &info, CurrentVersionInfo &currentVersionInfo,
        BusinessError &businessError) = 0;

    virtual int32_t GetCurrentVersionDescription(const UpgradeInfo &info, const DescriptionOptions &descriptionOptions,
        VersionDescriptionInfo &currentVersionDescriptionInfo, BusinessError &businessError) = 0;

    virtual int32_t GetTaskInfo(const UpgradeInfo &info, TaskInfo &taskInfo, BusinessError &businessError) = 0;

    virtual int32_t SetUpgradePolicy(const UpgradeInfo &info, const UpgradePolicy &policy,
        BusinessError &businessError) = 0;

    virtual int32_t GetUpgradePolicy(const UpgradeInfo &info, UpgradePolicy &policy, BusinessError &businessError) = 0;

    virtual int32_t Cancel(const UpgradeInfo &info, int32_t service, BusinessError &businessError) = 0;
};
} // namespace OHOS::UpdateService
#endif // ISERVICE_ONLINE_UPDATER_H
