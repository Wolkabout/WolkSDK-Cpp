/*
 * Copyright 2020 WolkAbout Technology s.r.o.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SIMPLEURLDOWNLOADER_H
#define SIMPLEURLDOWNLOADER_H

#include "api/UrlFileDownloader.h"

#include <atomic>
#include <memory>
#include <thread>

namespace Poco
{
class URI;
}

namespace wolkabout
{
class SimpleUrlDownloader : public UrlFileDownloader
{
public:
    SimpleUrlDownloader();
    ~SimpleUrlDownloader();

    void download(const std::string& url, const std::string& downloadDirectory,
                  std::function<void(const std::string&, const std::string&, const std::string&)> onSuccessCallback,
                  std::function<void(const std::string&, FileTransferError)> onFailCallback) override;
    void abort(const std::string& url) override;

private:
    void initialize();
    std::string extractFileName(Poco::URI& uri) const;

    void run();
    void stop();

    std::string m_url;
    std::string m_downloadDirectory;
    std::function<void(const std::string&, const std::string&, const std::string&)> m_onSuccessCallback;
    std::function<void(const std::string&, FileTransferError)> m_onFailCallback;

    std::atomic_bool m_run;
    std::unique_ptr<std::thread> m_worker;
};
}    // namespace wolkabout

#endif    // SIMPLEURLDOWNLOADER_H
