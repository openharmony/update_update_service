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

#ifndef CLIENT_CONTEXT_H
#define CLIENT_CONTEXT_H

#include <utility>

#include "napi/native_api.h"
#include "napi/native_node_api.h"

#include "common_error_define.h"

namespace OHOS::UpdateEngine {
template <typename T>
struct ClientContext {
    typedef napi_value (*GetNapiParam)(napi_env env, napi_callback_info info, std::unique_ptr<T> &clientContext);
    typedef void (*GetIpcBusinessError)(const std::string &funcName, int32_t ipcRequestCode,
        BusinessError &businessError);
    typedef napi_value (*CreateNapiValue)(napi_env env, const T &context);

    ClientContext(std::string method, GetNapiParam getNapiParam, napi_async_execute_callback executeFunc,
        std::vector<std::pair<std::string, std::string>> paramInfos, GetIpcBusinessError getIpcBusinessError)
        : method_(std::move(method)),
          getNapiParam_(getNapiParam),
          executeFunc_(executeFunc),
          paramInfos_(std::move(paramInfos)),
          getIpcBusinessError_(getIpcBusinessError)
    {
        ENGINE_LOGD("ClientContext construct");
    }

    ~ClientContext()
    {
        ENGINE_LOGD("~ClientContext destruct");
    }

    std::string method_; // 执行的接口名
    BusinessError businessError_;
    int32_t ipcRequestCode_ = 0;

    GetNapiParam getNapiParam_ = nullptr; // napi获取参数
    CreateNapiValue createValueFunc_ = nullptr; // 通过ipc返回结果, 构建napi结果对象函数

    napi_async_execute_callback executeFunc_; // 异步执行函数
    napi_ref callbackRef_ = nullptr; // callback 回调
    napi_deferred deferred_ = nullptr; // promise deferred对象
    napi_async_work work_ = nullptr;

    std::vector<std::pair<std::string, std::string>> paramInfos_; // 入参校验异常返回结果
    GetIpcBusinessError getIpcBusinessError_; // 通过ipc返回异常，构造BusinessError
};
} // namespace OHOS::UpdateEngine
#endif // CLIENT_CONTEXT_H