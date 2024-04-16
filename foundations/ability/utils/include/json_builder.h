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

#ifndef JSON_BUILDER_H
#define JSON_BUILDER_H

#include <string>
#include <vector>

namespace OHOS::UpdateEngine {
class JsonBuilder {
public:
    JsonBuilder &Append(const std::string &qualifier)
    {
        builder_.append(qualifier);
        return *this;
    }

    JsonBuilder &Append(const std::string &key, const uint32_t value)
    {
        return AppendNum(key, std::to_string(value));
    }

    JsonBuilder &Append(const std::string &key, const int32_t value)
    {
        return AppendNum(key, std::to_string(value));
    }

    JsonBuilder &Append(const std::string &key, const bool value)
    {
        const std::string valueString = value ? "true" : "false";
        return AppendNum(key, valueString);
    }

    JsonBuilder &Append(const std::string &key, const int64_t value)
    {
        return AppendNum(key, std::to_string(value));
    }

    JsonBuilder &Append(const std::string &key, const std::string &value, const bool isJsonString = false)
    {
        AppendComma();
        if (isJsonString) {
            builder_.append("\"").append(key).append("\"").append(":").append(value);
        } else {
            builder_.append("\"").append(key).append("\"").append(":").append("\"").append(value).append("\"");
        }
        return *this;
    }

    JsonBuilder &Append(const std::string &key, const JsonBuilder &jsonBuilder)
    {
        AppendComma();
        builder_.append("\"").append(key).append("\"").append(":").append(jsonBuilder.ToJson());
        return *this;
    }

    JsonBuilder &Append(const std::string &key, const std::vector<JsonBuilder> &valueList)
    {
        std::vector<JsonBuilder>::size_type valueListSize = valueList.size();
        if (!valueList.empty()) {
            JsonBuilder jsonArray;
            jsonArray.Append("[");
            for (size_t i = 0; i < valueListSize; i++) {
                jsonArray.Append(valueList[i].ToJson());
                if (i != valueListSize - 1) {
                    jsonArray.Append(",");
                }
            }
            jsonArray.Append("]");

            Append(key, jsonArray);
        }
        return *this;
    }

    std::string ToJson() const
    {
        return builder_;
    }

private:
    JsonBuilder &AppendNum(const std::string &key, const std::string &value)
    {
        AppendComma();
        builder_.append("\"").append(key).append("\"").append(":").append(value);
        return *this;
    }

    void AppendComma()
    {
        if (isFirstItem_) {
            isFirstItem_ = false;
        } else {
            builder_.append(",");
        }
    }

private:
    std::string builder_;
    bool isFirstItem_ = true;
};
} // namespace OHOS::UpdateEngine
#endif // JSON_BUILDER_H