#ifndef WOLKABOUTCORE_ATTRIBUTE_H
#define WOLKABOUTCORE_ATTRIBUTE_H

#include <string>
#include "core/Types.h"

namespace wolkabout
{
class Attribute
{
public:
    Attribute(const std::string& name, DataType dataType, std::string value)
    : m_name(name), m_dataType(dataType), m_value(value)
    {
    }
    const std::string& getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }
    DataType getDataType() const { return m_dataType; }
    void setDataType(DataType dataType) { m_dataType = dataType; }
    std::string getValue() const { return m_value; }
    void setValue(std::string value) { m_value = value; }

private:
    std::string m_name;
    DataType m_dataType;
    std::string m_value;
};
} // !namespace wolkabout
#endif    // WOLKABOUTCORE_ATTRIBUTE_H
