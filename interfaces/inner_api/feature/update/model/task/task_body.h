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

#ifndef UPDATE_SERVICE_TASK_BODY_H
#define UPDATE_SERVICE_TASK_BODY_H

#include "event_id.h"
#include "error_message.h"
#include "install_mode.h"
#include "update_define.h"
#include "upgrade_status.h"
#include "version_digest_info.h"
#include "version_component.h"

namespace OHOS::UpdateEngine {
struct TaskBody {
    VersionDigestInfo versionDigestInfo;
    UpgradeStatus status = UpgradeStatus::INIT;
    int32_t subStatus = CAST_INT(UpgradeStatus::INIT);
    int32_t progress = 0;
    int32_t installMode = CAST_INT(InstallMode::NORMAL);
    std::vector<ErrorMessage> errorMessages;
    std::vector<VersionComponent> versionComponents;

    JsonBuilder GetJsonBuilder(EventId eventId);
};
} // namespace OHOS::UpdateEngine
#endif // UPDATE_SERVICE_TASK_BODY_H
