/*
* Copyright (c) 2025 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <algorithm>
#include <functional>
#include <string>
#include <unistd.h>

#include "cJSON.h"
#include "update_define.h"

namespace OHOS::UpdateService {
enum class JsonParseError {
    ERR_OK = 0,
    COMMOM_ERROR,
    MISSING_PROP,
    TYPE_ERROR
};

// 开发人员请使用该工具类封装的安全函数来获取json对象/数组，以及从json对象获取Value
// 当前获取json对象/数组的封装使用的parse方法参数：string输入、无解析回调、解析失败不报异常
class JsonUtils {
public:
    template <typename T> static int32_t GetValueAndSetTo(cJSON *jsonObject, const std::string &key, T &value)
    {
        if (!jsonObject || key.empty()) {
            return CAST_INT(JsonParseError::MISSING_PROP);
        }

        cJSON *item = cJSON_GetObjectItemCaseSensitive(jsonObject, key.c_str());
        if (!item) {
            return CAST_INT(JsonParseError::MISSING_PROP);
        }

        if (!CheckType(item, value)) {
            return CAST_INT(JsonParseError::TYPE_ERROR);
        }
        GetValue(item, value);
        return CAST_INT(JsonParseError::ERR_OK);
    }

    static cJSON *ParseAndGetJsonObject(const std::string &jsonStr)
    {
        cJSON *object = cJSON_Parse(jsonStr.c_str());
        if (!object) {
            return nullptr;
        }

        if (!cJSON_IsObject(object))
        {
            cJSON_Delete(object);
            return nullptr;
        }

        return object;
    }

    static cJSON *ParseAndGetJsonArray(const std::string &jsonStr)
    {
        cJSON *root = cJSON_Parse(jsonStr.c_str());
        if (!root) {
            return nullptr;
        }

        if (!cJSON_IsArray(root))
        {
            cJSON_Delete(root);
            return nullptr;
        }

        return root;
    }

    static int32_t GetValueAndSetToArray(cJSON *jsonObject, const std::string &key, cJSON *&value)
    {
        cJSON *item = cJSON_GetObjectItemCaseSensitive(jsonObject, key.c_str());
        if (!item) {
            return CAST_INT(JsonParseError::MISSING_PROP);
        }

        if (!cJSON_IsArray(item)) {
            return CAST_INT(JsonParseError::TYPE_ERROR);
        }

        value = cJSON_Duplicate(item, 1);
        return CAST_INT(JsonParseError::ERR_OK);
    }

    static void SetJsonToVector(cJSON *jsonObject, std::vector<std::string> &vector)
    {
        if (cJSON_IsArray(jsonObject))
        {
            cJSON *child = jsonObject->child;
            while (child != nullptr)
            {
                if (cJSON_IsString(child) && child->valuestring != nullptr)
                {
                    vector.emplace_back(std::string(child->valuestring));
                }
                child = child->next;
            }
        }
    }

    static bool IsArray(cJSON *jsonObject, const std::string &key)
    {
        cJSON *item = cJSON_GetObjectItemCaseSensitive(jsonObject, key.c_str());
        return item && cJSON_IsArray(item);
    }

    static void SetJsonToVector(cJSON *jsonObject, const std::string &key, std::vector<std::string> &vector)
    {
        if (IsArray(jsonObject, key)) {
            cJSON *jsonArray = cJSON_GetObjectItemCaseSensitive(jsonObject, key.c_str());
            SetJsonToVector(jsonArray, vector);
        }
    }

private:
    static bool CheckType(cJSON *jsonObject, std::string &value)
    {
        return jsonObject && cJSON_IsString(jsonObject);
    }

    static bool CheckType(cJSON *jsonObject, int32_t &value)
    {
        return jsonObject && cJSON_IsNumber(jsonObject);
    }

    static bool CheckType(cJSON *jsonObject, int64_t &value)
    {
        return jsonObject && cJSON_IsNumber(jsonObject);
    }

    static bool CheckType(cJSON *jsonObject, uint32_t &value)
    {
        return jsonObject && cJSON_IsNumber(jsonObject) && jsonObject->valuedouble >=0;
    }

    static bool CheckType(cJSON *jsonObject, uint64_t &value)
    {
        return jsonObject && cJSON_IsNumber(jsonObject) && jsonObject->valuedouble >=0;
    }

    static bool CheckType(cJSON *jsonObject, bool value)
    {
        return jsonObject && (jsonObject->type == cJSON_True || jsonObject->type == cJSON_False);
    }

    template <typename T>
    static bool CheckType(cJSON *jsonObject, T &value)
    {
        return cJSON_IsObject(jsonObject);
    }

    template <typename T>
    static bool CheckType(cJSON *jsonObject, std::vector<T> &value)
    {
        return cJSON_IsArray(jsonObject);
    }

    static void GetValue(const cJSON *jsonArray, std::vector<std::string> &value)
    {
        if (!jsonArray || !cJSON_IsArray(jsonArray)) {
            return;
        }

        const cJSON *element = nullptr;
        cJSON_ArrayForEach(element, jsonArray) {
            if (cJSON_IsString(element) && element->valuestring != nullptr) {
                value.push_back(element->valuestring);
            }
        }
    }

    template <typename T>
    static void GetValue(cJSON *item, T &value)
    {
        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, int32_t>) {
            if (cJSON_IsNumber(item)) {
                value = item->valueint;
            }
        } else if constexpr (std::is_same_v<T, double> || std::is_same_v<T, int64_t>) {
            if (cJSON_IsNumber(item)) {
                value = item->valuedouble;
            }
        } else if constexpr (std::is_same_v<T, uint32_t> || std::is_same_v<T, uint64_t>) {
            if (cJSON_IsNumber(item)) {
                value = item->valuedouble;
            }
        } else if constexpr (std::is_same_v<T, bool>) {
            if (cJSON_IsTrue(item)) {
                value = true;
            } else {
                value = false;
            }
        } else if constexpr (std::is_same_v<T, std::string>) {
            if (cJSON_IsString(item)) {
                value = item->valuestring;
            }
        } else {
            return;
        }
        return;
    }
};
} // namespace OHOS::UpdateService
#endif // JSON_UTILS_H
