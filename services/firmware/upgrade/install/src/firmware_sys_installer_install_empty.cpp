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

#include "firmware_sys_installer_install.h"

#include "dupdate_errno.h"
#include "firmware_log.h"
#include "firmware_update_helper.h"

namespace OHOS {
namespace UpdateEngine {

bool SysInstallerInstall::IsComponentLegal(const std::vector<FirmwareComponent> &componentList)
{
    return true;
}

bool SysInstallerInstall::PerformInstall(const std::vector<FirmwareComponent> &componentList)
{
    return true;
}

int32_t SysInstallerInstall::DoSysInstall(const FirmwareComponent &firmwareComponent)
{
    return OHOS_SUCCESS;
}

void SysInstallerInstall::InitInstallProgress()
{
    FIRMWARE_LOGI("SysInstallerInstall InitInstallProgress");
}

int32_t SysInstallerInstall::WaitInstallResult()
{
    return OHOS_SUCCESS;
}
} // namespace UpdateEngine
} // namespace OHOS
