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

#ifndef UPDATE_SERVICE_NETWORK_TYPE_H
#define UPDATE_SERVICE_NETWORK_TYPE_H

namespace OHOS::UpdateEngine {
enum class NetType {
    NO_NET = 0,
    CELLULAR = 1,
    METERED_WIFI = 2,
    NOT_METERED_WIFI = 4,
    CELLULAR_AND_METERED_WIFI = CELLULAR | METERED_WIFI,
    CELLULAR_AND_NOT_METERED_WIFI = CELLULAR | NOT_METERED_WIFI,
    WIFI = METERED_WIFI | NOT_METERED_WIFI,
    CELLULAR_AND_WIFI = CELLULAR | WIFI
};
} // namespace OHOS::UpdateEngine
#endif // UPDATE_SERVICE_NETWORK_TYPE_H
