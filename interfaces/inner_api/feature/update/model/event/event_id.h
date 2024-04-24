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

#ifndef UPDATE_SERVICE_EVENT_ID_H
#define UPDATE_SERVICE_EVENT_ID_H

#include "event_classify.h"
#include "update_define.h"

namespace OHOS::UpdateEngine {
enum class EventId {
    EVENT_TASK_BASE = CAST_UINT(EventClassify::TASK),
    EVENT_TASK_RECEIVE,
    EVENT_TASK_CANCEL,
    EVENT_DOWNLOAD_WAIT,
    EVENT_DOWNLOAD_START,
    EVENT_DOWNLOAD_UPDATE,
    EVENT_DOWNLOAD_PAUSE,
    EVENT_DOWNLOAD_RESUME,
    EVENT_DOWNLOAD_SUCCESS,
    EVENT_DOWNLOAD_FAIL,
    EVENT_UPGRADE_WAIT,
    EVENT_UPGRADE_START,
    EVENT_UPGRADE_UPDATE,
    EVENT_APPLY_WAIT,
    EVENT_APPLY_START,
    EVENT_UPGRADE_SUCCESS,
    EVENT_UPGRADE_FAIL,
    EVENT_AUTH_START,
    EVENT_AUTH_SUCCESS,
    EVENT_DOWNLOAD_CANCEL,
    EVENT_INITIALIZE,
    EVENT_TASK_CHANGE,
    EVENT_VERSION_INFO_CHANGE,
    EVENT_TRANSFER_WAIT,
    EVENT_TRANSFER_START,
    EVENT_TRANSFERING,
    EVENT_TRANSFER_SUCCESS,
    EVENT_TRANSFER_FAIL,
    SYSTEM_BASE = CAST_UINT(EventClassify::SYSTEM),
    SYSTEM_BOOT_COMPLETE,
};
} // namespace OHOS::UpdateEngine
#endif // UPDATE_SERVICE_EVENT_ID_H
