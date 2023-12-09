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

#include "hilog/log.h"

constexpr OHOS::HiviewDFX::HiLogLabel MODULE_MGR_LABEL = {LOG_CORE, 0xD002E00, "UPDATER_MODULE_MGR"};

#define UTILS_LOGF(...) (void)OHOS::HiviewDFX::HiLog::Fatal(MODULE_MGR_LABEL, __VA_ARGS__)
#define UTILS_LOGE(...) (void)OHOS::HiviewDFX::HiLog::Error(MODULE_MGR_LABEL, __VA_ARGS__)
#define UTILS_LOGW(...) (void)OHOS::HiviewDFX::HiLog::Warn(MODULE_MGR_LABEL, __VA_ARGS__)
#define UTILS_LOGI(...) (void)OHOS::HiviewDFX::HiLog::Info(MODULE_MGR_LABEL, __VA_ARGS__)
#define UTILS_LOGD(...) (void)OHOS::HiviewDFX::HiLog::Debug(MODULE_MGR_LABEL, __VA_ARGS__)

#endif // UPDATE_SERVICE__MODULE_LOG_H
