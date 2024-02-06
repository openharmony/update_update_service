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

#ifndef UPDATE_SERVICE_DEVICE_TYPE_H
#define UPDATE_SERVICE_DEVICE_TYPE_H

namespace OHOS::UpdateEngine {
enum class DeviceType {
    UNKNOWN = 0,
    SMART_PHONE = 1,    // 手机
    SMART_PAD = 2,      // 平板
    SMART_TV = 4,       // 智能电视
    TWS = 6,            // 真无线耳机
    KEYBOARD = 7,       // 键盘
    PEN  = 8            // 手写笔
};
} // namespace OHOS::UpdateEngine
#endif // UPDATE_SERVICE_DEVICE_TYPE_H
