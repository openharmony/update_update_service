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

#ifndef NAPI_CLIENT_INTERFACE_H
#define NAPI_CLIENT_INTERFACE_H

#include <memory>

#include "napi/native_api.h"
#include "napi/native_node_api.h"

namespace OHOS::UpdateEngine {
class NapiClientInterface {
public:
    virtual ~NapiClientInterface() = default;
    virtual napi_value HandleFunc(napi_env env, napi_callback_info info) = 0;
};
} // OHOS::UpdateEngine
#endif // NAPI_CLIENT_INTERFACE_H