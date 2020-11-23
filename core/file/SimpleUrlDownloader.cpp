#include "SimpleUrlDownloader.h"

#include "FileAssembler.h"
#include "utilities/ByteUtils.h"
#include "utilities/Logger.h"

#include "Poco/Exception.h"
#include "Poco/Net/HTTPSStreamFactory.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/NetException.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/URIStreamOpener.h"

#include <iostream>
#include <memory>

namespace wolkabout
{
SimpleUrlDownloader::SimpleUrlDownloader()
{
    initialize();
}

SimpleUrlDownloader::~SimpleUrlDownloader()
{
    stop();
}

void SimpleUrlDownloader::download(
  const std::string& url, const std::string& downloadDirectory,
  std::function<void(const std::string&, const std::string&, const std::string&)> onSuccessCallback,
  std::function<void(const std::string&, FileTransferError)> onFailCallback)
{
    if (m_run)
    {
        LOG(WARN) << "Already downloading: " << m_url;
        return;
    }

    LOG(INFO) << "Starting download from " << url;
    m_url = url;
    m_downloadDirectory = downloadDirectory;
    m_onSuccessCallback = onSuccessCallback;
    m_onFailCallback = onFailCallback;

    m_worker = std::unique_ptr<std::thread>(new std::thread(&SimpleUrlDownloader::run, this));
}

void SimpleUrlDownloader::abort(const std::string& url)
{
    LOG(INFO) << "Aborting download from " << url;
    stop();
}

void SimpleUrlDownloader::initialize()
{
    static bool initialized = false;
    if (!initialized)
    {
        Poco::Net::HTTPStreamFactory::registerFactory();
        Poco::Net::HTTPSStreamFactory::registerFactory();
        initialized = true;
    }
}

std::string SimpleUrlDownloader::extractFileName(Poco::URI& uri) const
{
    std::vector<std::string> pathSegments;
    uri.getPathSegments(pathSegments);

    return pathSegments.size() > 0 ? pathSegments.back() : "";
}

void SimpleUrlDownloader::run()
{
    m_run = true;

    Poco::URI uri;

    try
    {
        uri = Poco::URI(m_url);
    }
    catch (Poco::SyntaxException& e)
    {
        LOG(ERROR) << "Failed to parse url: " << e.what();
        m_onFailCallback(m_url, FileTransferError::MALFORMED_URL);
        m_run = false;
        return;
    }

    std::shared_ptr<std::istream> responseStream;

    try
    {
        auto& opener = Poco::URIStreamOpener::defaultOpener();
        responseStream = std::shared_ptr<std::istream>{opener.open(uri)};
    }
    catch (Poco::Exception& e)
    {
        LOG(ERROR) << "Failed to open url: " << e.what();
        m_onFailCallback(m_url, FileTransferError::UNSPECIFIED_ERROR);
        m_run = false;
        return;
    }

    auto fileName = extractFileName(uri);

    if (fileName.empty())
    {
        m_onFailCallback(m_url, FileTransferError::MALFORMED_URL);
        m_run = false;
        return;
    }

    auto path = FileSystemUtils::composePath(fileName, m_downloadDirectory);

    FileAssembler assembler(path);
    Byte buffer[1024];

    LOG(INFO) << "Downloading file: " << m_url;

    while (m_run && responseStream->good())
    {
        responseStream->read(reinterpret_cast<char*>(buffer), sizeof(buffer));
        std::streamsize read = responseStream->gcount();

        assembler.append({std::begin(buffer), std::begin(buffer) + read});
    }

    // not aborted
    if (m_run)
    {
        try
        {
            assembler.finish();
            m_onSuccessCallback(m_url, fileName, path);
        }
        catch (FileSystemException& e)
        {
            LOG(ERROR) << "Failed to save file: " << e.what();
            m_onFailCallback(m_url, FileTransferError::FILE_SYSTEM_ERROR);
        }
    }
    else
    {
        assembler.clear();
    }

    m_run = false;
}

void SimpleUrlDownloader::stop()
{
    m_run = false;

    if (m_worker && m_worker->joinable())
    {
        m_worker->join();
    }
}

}    // namespace wolkabout
