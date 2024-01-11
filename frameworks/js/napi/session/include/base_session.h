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

#ifndef BASE_SESSION_H
#define BASE_SESSION_H

#include <functional>

#include "js_native_api_types.h"
#include "napi_structs_base.h"

namespace OHOS::UpdateEngine {
class BaseSession {
public:
    using DoWorkFunction = std::function<int(void *context)>;

    virtual uint32_t GetType() const = 0;
    virtual uint32_t GetSessionId() const = 0;
    virtual napi_value StartWork(napi_env env, const napi_value *args, DoWorkFunction worker, void *context) = 0;
    virtual bool IsAsyncCompleteWork() = 0;
    virtual void OnAsyncComplete(const BusinessError &businessError) = 0;
};
} // namespace OHOS::UpdateEngine
#endif // BASE_SESSION_H