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

#ifndef UPDATE_SERVICE_COMPONENT_TYPE_H
#define UPDATE_SERVICE_COMPONENT_TYPE_H

namespace OHOS::UpdateEngine {
enum class ComponentType {
    INVALID = 0,
    OTA = 1,
    PATCH = 2,
    COTA = 4,
    PARAM = 8,
    SA = 16
};
} // namespace OHOS::UpdateEngine
#endif // UPDATE_SERVICE_COMPONENT_TYPE_H
