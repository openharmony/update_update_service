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

#include <gtest/gtest.h>

#include "progress_thread.h"

using namespace testing::ext;
using namespace testing;

namespace OHOS::UpdateService {

Progress downloadProgress;
void TestProgressCallback(const Progress &progress)
{
    downloadProgress = progress;
}

class StreamProgressThreadTest : public testing::Test {
public:
    static void SetupTestCase(void) {};
    static void TearDownTestCase(void) {};
    void SetUp() const final {};
    void TearDown() const final {};
};

HWTEST_F(StreamProgressThreadTest, StartDownloadTest, TestSize.Level1)
{
    StreamProgressThread thread(TestProgressCallback);

    std::string url = "http://file-examples.com/wp-content/uploads/2017/02/file-example_txt-download.txt";
    int64_t fileSize = 2250;
    int64_t recordPoint = 0;

    // 调用 StartDownload
    int32_t result = thread.StartDownload(url, fileSize, recordPoint);
    ASSERT_EQ(result, 0); // 假设 StartDownload 返回 0 表示成功
    thread.StopDownload();
}

HWTEST_F(StreamProgressThreadTest, WriteFuncTest, TestSize.Level1)
{
    StreamProgressThread thread(TestProgressCallback);

    // 模拟写入数据
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
    size_t size = sizeof(data);
    size_t nmemb = 1;

    // 调用 WriteFunc
    size_t result = StreamProgressThread::WriteFunc(data, size, nmemb, &thread);
    ASSERT_EQ(result, size);
}

HWTEST_F(StreamProgressThreadTest, DownloadProgressTest, TestSize.Level1)
{
    StreamProgressThread thread(TestProgressCallback);

    // 模拟进度更新参数
    double dlTotal = 1024;
    double dlNow = 512;
    double ulTotal = 0;
    double ulNow = 0;

    // 调用 DownloadProgress
    int32_t result = StreamProgressThread::DownloadProgress(&thread, dlTotal, dlNow, ulTotal, ulNow);
    ASSERT_EQ(result, 0);
}
} // namespace OHOS::UpdateService