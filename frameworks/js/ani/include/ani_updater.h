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

#ifndef ANI_UPDATER_H
#define ANI_UPDATER_H

#include "ohos.update.proj.hpp"
#include "ohos.update.impl.hpp"

#include "ani_base_updater.h"
#include "upgrade_info.h"

namespace OHOS::UpdateEngine {
class AniUpdater final : public AniBaseUpdater {
public:
    explicit AniUpdater(const UpgradeInfo &info);

    ohos::update::CheckResult CheckNewVersionSync();
    ohos::update::NewVersionInfo GetNewVersionInfoSync();
    taihe::array<ohos::update::ComponentDescription> GetNewVersionDescriptionSync(
        const ohos::update::VersionDigestInfo &versionDigestInfo,
        const ohos::update::DescriptionOptions &descriptionOptions);
    ohos::update::CurrentVersionInfo GetCurrentVersionInfoSync();
    taihe::array<ohos::update::ComponentDescription> GetCurrentVersionDescriptionSync(
        const ohos::update::DescriptionOptions &descriptionOptions);
    ohos::update::TaskInfo GetTaskInfoSync();
    void DownloadSync(const ohos::update::VersionDigestInfo &versionDigestInfo,
        const ohos::update::DownloadOptions &downloadOptions);
    void ResumeDownloadSync(const ohos::update::VersionDigestInfo &versionDigestInfo,
        const ohos::update::ResumeDownloadOptions &resumeDownloadOptions);
    void PauseDownloadSync(const ohos::update::VersionDigestInfo &versionDigestInfo,
        const ohos::update::PauseDownloadOptions &pauseDownloadOptions);
    void UpgradeSync(const ohos::update::VersionDigestInfo &versionDigestInfo,
        const ohos::update::UpgradeOptions &upgradeOptions);
    void ClearErrorSync(const ohos::update::VersionDigestInfo &versionDigestInfo,
        const ohos::update::ClearOptions &clearOptions);
    ohos::update::UpgradePolicy getUpgradePolicySync();
    void SetUpgradePolicySync(const ohos::update::UpgradePolicy &policy);
    void TerminateUpgradeSync();

protected:
    void RegisterCallback() override;
    void UnRegisterCallback() override;

private:
    UpgradeInfo upgradeInfo_;
};
}

#endif // ANI_UPDATER_H
