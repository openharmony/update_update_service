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

#include "firmware_manual_check_mode.h"

#include "firmware_constant.h"
#include "firmware_log.h"

namespace OHOS {
namespace UpdateEngine {
FirmwareStep FirmwareManualCheckMode::GetNextStep(FirmwareStep step)
{
    FIRMWARE_LOGI("GetNextStep %{public}d", static_cast<uint32_t>(step));
    return step;
}

FirmwareStep FirmwareManualCheckMode::GetStepAfterInit()
{
    return FirmwareStep::CHECK_STEP;
}

FirmwareStep FirmwareManualCheckMode::GetStepAfterCheck()
{
    return FirmwareStep::COMPLETE;
}

void FirmwareManualCheckMode::SetCheckResult(
    CheckStatus status, const Duration &duration, const std::vector<FirmwareComponent> &componentList,
    const CheckAndAuthInfo &checkAndAuthInfo)
{
    FIRMWARE_LOGI("FirmwareManualCheckMode::SetCheckResult %{public}d", static_cast<uint32_t>(status));
}

void FirmwareManualCheckMode::HandleComplete()
{
    FIRMWARE_LOGI("FirmwareManualCheckMode::HandleComplete");
}
} // namespace UpdateEngine
} // namespace OHOS