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

#ifndef UPDATE_SERVICE_UPDATE_CALLBACK_INFO_H
#define UPDATE_SERVICE_UPDATE_CALLBACK_INFO_H

#include "event_info.h"

namespace OHOS::UpdateEngine {
using OnEvent = std::function<void(const EventInfo &eventInfo)>;

// 回调函数
struct UpdateCallbackInfo {
    OnEvent onEvent;
};
} // OHOS::UpdateEngine
#endif // UPDATE_SERVICE_UPDATE_CALLBACK_INFO_H
