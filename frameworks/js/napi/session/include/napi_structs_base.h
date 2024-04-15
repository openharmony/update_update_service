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

#ifndef NAPI_STRUCTS_BASE_H
#define NAPI_STRUCTS_BASE_H

#include <functional>

#include "business_error.h"
#include "napi_common_define.h"

namespace OHOS::UpdateEngine {
struct SessionParams {
    uint32_t type;
    size_t callbackStartIndex;
    bool isNeedBusinessError;
    bool isAsyncCompleteWork;

    SessionParams(uint32_t typeValue = UINT32_MAX, size_t callbackPosition = CALLBACK_POSITION_ONE,
        bool isNeedBusinessErrorValue = false, bool isAsyncCompleteWorkValue = false)
        {
            type = typeValue;
            isNeedBusinessError = isNeedBusinessErrorValue;
            isAsyncCompleteWork = isAsyncCompleteWorkValue;
            callbackStartIndex = (callbackPosition < 1) ? CALLBACK_POSITION_ONE : INDEX(callbackPosition);
        }
};

struct NapiResult {
    uint32_t type;
    BusinessError businessError;

    template <typename T>
    void AssignValue(T *const source, T *&target)
    {
        if (target == nullptr) {
            target = new (std::nothrow) T();
        }
        if ((target != nullptr) && (source != nullptr)) {
            *(target) = *(source);
        }
    }

    template <typename T>
    void ReleaseValue(T *&obj)
    {
        delete obj;
        obj = nullptr;
    }
};
} // namespace OHOS::UpdateEngine
#endif // NAPI_STRUCTS_BASE_H