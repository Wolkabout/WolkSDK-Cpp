/*
 * Copyright 2019 WolkAbout Technology s.r.o.
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
#ifndef FILEDOWNLOADPROTOCOL_H
#define FILEDOWNLOADPROTOCOL_H

#include "protocol/Protocol.h"

#include "model/FileUploadAbort.h"
#include "model/FileUploadInitiate.h"
#include "model/Message.h"

#include <memory>
#include <string>

namespace wolkabout
{
class BinaryData;
class FilePacketRequest;
class FileUploadStatus;

class FileDownloadProtocol : public Protocol
{
public:
    inline Type getType() const override final { return Protocol::Type::FILE_DOWNLOAD; }

    virtual bool isBinary(const Message& message) const = 0;

    virtual std::unique_ptr<BinaryData> makeBinaryData(const Message& message) const = 0;

    virtual std::unique_ptr<Message> makeMessage(const std::string& deviceKey,
                                                 const FilePacketRequest& filePacketRequest) const = 0;

    // define empty for backward compatibility with json single
    virtual bool isUploadInitiate(const Message& /*message*/) const { return false; }

    virtual bool isUploadAbort(const Message& /*message*/) const { return false; }

    virtual std::unique_ptr<FileUploadInitiate> makeFileUploadInitiate(const Message& /*message*/) const
    {
        return nullptr;
    }

    virtual std::unique_ptr<FileUploadAbort> makeFileUploadAbort(const Message& /*message*/) const { return nullptr; }

    virtual std::unique_ptr<Message> makeMessage(const std::string& /*deviceKey*/,
                                                 const FileUploadStatus& /*fileUploadStatus*/) const
    {
        return nullptr;
    }
};
}    // namespace wolkabout

#endif    // FILEDOWNLOADPROTOCOL_H
