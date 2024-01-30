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

#ifndef NAPI_COMMON_DEFINE_H
#define NAPI_COMMON_DEFINE_H

#include <algorithm>
#include <list>

#include "hilog/log.h"
#include "node_api.h"
#include "update_log.h"

namespace OHOS::UpdateEngine {
constexpr int32_t COMPONENT_ERR = 11500000;
#define PARAM_CHECK(validCheck, exper, ...)  \
if (!(validCheck)) {                     \
    ENGINE_LOGE(__VA_ARGS__);            \
    exper;                               \
}

#define PARAM_CHECK_NAPI_CALL(env, assertion, exper, message) \
if (!(assertion)) {                     \
    ENGINE_LOGE(message);               \
    exper;                              \
}

#define INDEX(x) ((x) - 1)

enum class ClientStatus {
    CLIENT_SUCCESS = 0,
    CLIENT_INVALID_PARAM = 1000,
    CLIENT_INVALID_TYPE,
    CLIENT_REPEAT_REQ,
    CLIENT_FAIL,
    CLIENT_CHECK_NEW_FIRST
};

enum ARG_NUM {
    ARG_NUM_ONE = 1,
    ARG_NUM_TWO,
    ARG_NUM_THREE,
    ARG_NUM_FOUR,
    MAX_ARGC
};

enum CALLBACK_POSITION {
    CALLBACK_POSITION_ONE = 1,
    CALLBACK_POSITION_TWO,
    CALLBACK_POSITION_THREE,
    CALLBACK_POSITION_FOUR,
    CALLBACK_MAX_POSITION
};

struct NativeClass {
    std::string className;
    napi_callback constructor;
    napi_ref *constructorRef;
    napi_property_descriptor *desc;
    int descSize;
};

template<typename T>
T *CreateJsObject(napi_env env, napi_callback_info info, napi_ref constructorRef, napi_value &jsObject)
{
    napi_value constructor = nullptr;
    napi_status status = napi_get_reference_value(env, constructorRef, &constructor);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok, return nullptr,
        "CreateJsObject error, napi_get_reference_value fail");

    size_t argc = MAX_ARGC;
    napi_value args[MAX_ARGC] = {0};
    status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    if (status != napi_ok) {
        ENGINE_LOGI("CreateJsObject, napi_get_cb_info error");
    }
    status = napi_new_instance(env, constructor, argc, args, &jsObject);
    PARAM_CHECK_NAPI_CALL(env, status == napi_ok, return nullptr, "CreateJsObject error, napi_new_instance fail");

    T *nativeObject = nullptr;
    status = napi_unwrap(env, jsObject, (void **) &nativeObject);
    if (status != napi_ok) {
        ENGINE_LOGE("CreateJsObject error, napi_unwrap fail");
        napi_remove_wrap(env, jsObject, (void **) &nativeObject);
        jsObject = nullptr;
        return nullptr;
    }
    return nativeObject;
}

template <typename T> T *UnwrapJsObject(napi_env env, napi_callback_info info)
{
    size_t argc = ARG_NUM_ONE;
    napi_value argv[ARG_NUM_ONE] = { 0 };
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);

    T *nativeObject = nullptr;
    napi_unwrap(env, thisVar, (void **)&nativeObject);
    return nativeObject;
}

template <typename T>
bool IsValidEnum(const std::list<T> &enumList, int32_t number)
{
    return std::any_of(enumList.begin(), enumList.end(), [=](T key) { return number == static_cast<int32_t>(key); });
}
} // namespace OHOS::UpdateEngine
#endif // NAPI_COMMON_DEFINE_H