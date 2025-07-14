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

#include "ani_local_updater.h"

#include "taihe/runtime.hpp"

#include "ani_common_conveter.h"
#include "update_service_kits.h"
#include "upgrade_file.h"

namespace OHOS::UpdateService {
void AniLocalUpdater::VerifyUpgradePackageSync(const ohos::update::UpgradeFile &upgradeFile,
    taihe::string_view certsFile)
{
    BusinessError error;
    int32_t funcResult = 0;
    const int32_t ret = UpdateServiceKits::GetInstance().VerifyUpgradePackage(std::string(upgradeFile.filePath),
        std::string(certsFile), error, funcResult);
    SetError(ret, "verifyUpgradePackage", error);
}

void AniLocalUpdater::ApplyNewVersionSync(const taihe::array_view<ohos::update::UpgradeFile> &upgradeFiles)
{
    BusinessError error;
    UpgradeInfo upgradeInfo;
    upgradeInfo.upgradeApp = LOCAL_UPGRADE_INFO;
    const std::string miscFile = "/dev/block/by-name/misc";
    std::vector<std::string> packageNames;
    int32_t funcResult = 0;
    for (const auto &value : upgradeFiles) {
        packageNames.emplace_back(AniCommonConverter::Converter(value).filePath);
    }
    const int32_t ret = UpdateServiceKits::GetInstance().ApplyNewVersion(upgradeInfo, miscFile, packageNames, error,
        funcResult);
    SetError(ret, "applyNewVersion", error);
}
}
