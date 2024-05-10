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


#ifndef UPDATE_SERVICE__MODULE_LOG_H
#define UPDATE_SERVICE__MODULE_LOG_H

#include "update_log.h"

#define UTILS_LOGE(fmt, ...) PRINT_LOGE(OHOS::UpdateEngine::UPDATE_MODULEMGR_TAG, fmt, ##__VA_ARGS__)
#define UTILS_LOGI(fmt, ...) PRINT_LOGI(OHOS::UpdateEngine::UPDATE_MODULEMGR_TAG, fmt, ##__VA_ARGS__)
#define UTILS_LOGD(fmt, ...) PRINT_LOGD(OHOS::UpdateEngine::UPDATE_MODULEMGR_TAG, fmt, ##__VA_ARGS__)

#endif // UPDATE_SERVICE__MODULE_LOG_H
