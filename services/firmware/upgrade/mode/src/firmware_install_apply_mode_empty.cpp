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

#include "firmware_install_apply_mode.h"

#include "firmware_constant.h"
#include "firmware_log.h"

namespace OHOS {
namespace UpdateEngine {
FirmwareStep FirmwareInstallApplyMode::GetNextStep(FirmwareStep step)
{
    FIRMWARE_LOGI("GetNextStep %{public}d", static_cast<uint32_t>(step));
    return step;
}

FirmwareStep FirmwareInstallApplyMode::GetStepAfterInit()
{
    return FirmwareStep::COMPLETE;
}

FirmwareStep FirmwareInstallApplyMode::GetStepForInstallAndApplyOrder()
{
    return FirmwareStep::INSTALL_STEP;
}

bool FirmwareInstallApplyMode::IsAllowInstall()
{
    return true;
}

bool FirmwareInstallApplyMode::IsUpgradeFilesReady()
{
    return true;
}

FirmwareStep FirmwareInstallApplyMode::GetStepAfterInstall()
{
    return FirmwareStep::COMPLETE;
}

FirmwareStep FirmwareInstallApplyMode::GetStepAfterApply()
{
    return FirmwareStep::COMPLETE;
}

void FirmwareInstallApplyMode::GetTask()
{
    FIRMWARE_LOGI("FirmwareInstallApplyMode::GetTask");
    businessError_.Build(CallResult::FAIL, "no task!");
}

void FirmwareInstallApplyMode::HandleComplete()
{
    onExecuteFinishCallback_();
}

UpgradeOptions FirmwareInstallApplyMode::GetUpgradeOptions()
{
    return upgradeOptions_;
}

InstallType FirmwareInstallApplyMode::GetInstallType()
{
    return installType_;
}

void FirmwareInstallApplyMode::SetInstallResult(const InstallCallbackInfo &installCallbackInfo)
{
    installStepDataProcessor_.SetInstallResult(installCallbackInfo);
}

void FirmwareInstallApplyMode::SetApplyResult(bool isSuccess)
{
    FIRMWARE_LOGI("SetApplyResult isSuccess: %{public}s", isSuccess ? "success" : "fail");
}
} // namespace UpdateEngine
} // namespace OHOS