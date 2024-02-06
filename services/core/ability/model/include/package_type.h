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

#ifndef UPDATE_SERVICE_PACKAGE_TYPE_H
#define UPDATE_SERVICE_PACKAGE_TYPE_H

namespace OHOS::UpdateEngine {
enum class PackageType {
    DYNAMIC = 0,
    NORMAL = 1,
    BASE = 2,
    CUST = 3,
    PRELOAD = 4,
    COTA = 5,
    VERSION = 6,
    PATCH = 8,
    SA = 9
};
} // namespace OHOS::UpdateEngine
#endif // UPDATE_SERVICE_PACKAGE_TYPE_H
