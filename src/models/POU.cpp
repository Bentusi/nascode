#include "POU.h"

namespace nascode {
namespace models {

POU::POU(POUType type, const QString& name)
    : m_type(type)
    , m_name(name)
{
}

void POU::removeVariable(const QString& name)
{
    for (int i = 0; i < m_variables.size(); ++i) {
        if (m_variables[i].name == name) {
            m_variables.removeAt(i);
            break;
        }
    }
}

Variable* POU::getVariable(const QString& name)
{
    for (auto& var : m_variables) {
        if (var.name == name) {
            return &var;
        }
    }
    return nullptr;
}

Program::Program(const QString& name)
    : POU(POUType::Program, name)
{
}

Function::Function(const QString& name)
    : POU(POUType::Function, name)
    , m_returnType("VOID")
{
}

FunctionBlock::FunctionBlock(const QString& name)
    : POU(POUType::FunctionBlock, name)
{
}

} // namespace models
} // namespace nascode
