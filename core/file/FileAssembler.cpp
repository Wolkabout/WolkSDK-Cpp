#include "FileAssembler.h"

namespace wolkabout
{
FileAssembler::FileAssembler(std::string path) : m_path{std::move(path)} {}

void FileAssembler::append(const ByteArray& bytes)
{
    m_bytes.insert(m_bytes.end(), bytes.begin(), bytes.end());
}

void FileAssembler::clear()
{
    // should remove parts from disk if any
    if (FileSystemUtils::isFilePresent(m_path))
    {
        FileSystemUtils::deleteFile(m_path);
    }
}

void FileAssembler::finish()
{
    if (!FileSystemUtils::createBinaryFileWithContent(m_path, m_bytes))
    {
        throw FileSystemException("Failed to save file");
    }
}

}    // namespace wolkabout
