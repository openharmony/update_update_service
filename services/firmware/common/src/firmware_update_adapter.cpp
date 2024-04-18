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

#include "firmware_update_adapter.h"

#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <securec.h>
#include <sstream>

#include "securec.h"

#include "constant.h"
#include "config_parse.h"
#include "device_adapter.h"
#include "firmware_constant.h"
#include "firmware_file_utils.h"
#include "firmware_log.h"
#include "firmware_preferences_utils.h"
#include "firmware_task.h"
#include "time_utils.h"

namespace OHOS {
namespace UpdateEngine {
std::string FirmwareUpdateAdapter::GetBusinessDomain()
{
    return DelayedSingleton<ConfigParse>::GetInstance()->GetBusinessDomain();
}

std::string FirmwareUpdateAdapter::GetUpdateAction()
{
    std::string action =
        DelayedSingleton<FirmwarePreferencesUtil>::GetInstance()->ObtainString(Firmware::UPDATE_ACTION, "");
    if (action.compare("recovery") == 0) {
        return "recovery";
    }
    return "upgrade";
}
} // namespace UpdateEngine
} // namespace OHOS
