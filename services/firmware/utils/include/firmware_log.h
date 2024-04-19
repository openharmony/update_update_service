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

#ifndef FIRMWARE_LOG_H
#define FIRMWARE_LOG_H

#include "update_log.h"

namespace OHOS {
namespace UpdateEngine {
static constexpr OHOS::HiviewDFX::HiLogLabel FIRMWARE_UPDATE_LABEL = {LOG_CORE, 0xD002E00, "UPDATE_FIRMWARE"};

#define FIRMWARE_LOGD(fmt, ...) PRINT_LOGD(UPDATE_FIRMWARE_TAG, fmt, ##__VA_ARGS__)
#define FIRMWARE_LOGI(fmt, ...) PRINT_LOGI(UPDATE_FIRMWARE_TAG, fmt, ##__VA_ARGS__)
#define FIRMWARE_LOGE(fmt, ...) PRINT_LOGE(UPDATE_FIRMWARE_TAG, fmt, ##__VA_ARGS__)

#define FIRMWARE_LONG_LOGD(fmt, args) PRINT_LONG_LOGD(UPDATE_FIRMWARE_TAG, UPDATE_LABEL[UPDATE_FIRMWARE_TAG], \
    fmt, args)
#define FIRMWARE_LONG_LOGI(fmt, args) PRINT_LONG_LOGI(UPDATE_FIRMWARE_TAG, UPDATE_LABEL[UPDATE_FIRMWARE_TAG], \
    fmt, args)
#define FIRMWARE_LONG_LOGE(fmt, args) PRINT_LONG_LOGE(UPDATE_FIRMWARE_TAG, UPDATE_LABEL[UPDATE_FIRMWARE_TAG], \
    fmt, args)
} // namespace UpdateEngine
} // namespace OHOS
#endif // FIRMWARE_LOG_H