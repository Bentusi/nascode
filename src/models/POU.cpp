#include "POU.h"
#include <QVariantMap>
#include <QVariantList>
#include <QJsonObject>
#include <QJsonArray>

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

QVariantMap POU::toVariant() const
{
    QVariantMap data;
    data["name"] = m_name;
    data["type"] = static_cast<int>(m_type);
    data["code"] = m_code;
    data["comment"] = m_comment;
    
    // 序列化变量
    QVariantList varList;
    for (const auto& var : m_variables) {
        QVariantMap varData;
        varData["name"] = var.name;
        varData["dataType"] = var.dataType;
        varData["initialValue"] = var.initialValue;
        varData["comment"] = var.comment;
        varData["section"] = static_cast<int>(var.section);
        varList.append(varData);
    }
    data["variables"] = varList;
    
    return data;
}

void POU::fromVariant(const QVariantMap& data)
{
    m_name = data.value("name").toString();
    m_type = static_cast<POUType>(data.value("type").toInt());
    m_code = data.value("code").toString();
    m_comment = data.value("comment").toString();
    
    // 反序列化变量
    m_variables.clear();
    QVariantList varList = data.value("variables").toList();
    for (const auto& varData : varList) {
        QVariantMap varMap = varData.toMap();
        Variable var;
        var.name = varMap.value("name").toString();
        var.dataType = varMap.value("dataType").toString();
        var.initialValue = varMap.value("initialValue").toString();
        var.comment = varMap.value("comment").toString();
        var.section = static_cast<Variable::Section>(varMap.value("section").toInt());
        m_variables.append(var);
    }
}

QString POU::generateSTCode() const
{
    QString result;
    
    // 根据类型生成声明
    switch (m_type) {
        case POUType::Program:
            result = QString("PROGRAM %1\n").arg(m_name);
            break;
        case POUType::Function:
            result = QString("FUNCTION %1\n").arg(m_name);
            break;
        case POUType::FunctionBlock:
            result = QString("FUNCTION_BLOCK %1\n").arg(m_name);
            break;
    }
    
    // 生成变量声明
    if (!m_variables.isEmpty()) {
        // 按section分组
        QMap<Variable::Section, QList<Variable>> grouped;
        for (const auto& var : m_variables) {
            grouped[var.section].append(var);
        }
        
        // 生成各个section
        for (auto it = grouped.begin(); it != grouped.end(); ++it) {
            QString sectionName;
            switch (it.key()) {
                case Variable::VAR: sectionName = "VAR"; break;
                case Variable::VAR_INPUT: sectionName = "VAR_INPUT"; break;
                case Variable::VAR_OUTPUT: sectionName = "VAR_OUTPUT"; break;
                case Variable::VAR_IN_OUT: sectionName = "VAR_IN_OUT"; break;
                case Variable::VAR_TEMP: sectionName = "VAR_TEMP"; break;
                case Variable::VAR_EXTERNAL: sectionName = "VAR_EXTERNAL"; break;
                case Variable::VAR_GLOBAL: sectionName = "VAR_GLOBAL"; break;
            }
            
            result += sectionName + "\n";
            for (const auto& var : it.value()) {
                result += QString("    %1 : %2").arg(var.name, var.dataType);
                if (!var.initialValue.isEmpty()) {
                    result += QString(" := %1").arg(var.initialValue);
                }
                result += ";";
                if (!var.comment.isEmpty()) {
                    result += QString(" (* %1 *)").arg(var.comment);
                }
                result += "\n";
            }
            result += "END_VAR\n";
        }
    }
    
    // 添加代码主体
    if (!m_code.isEmpty()) {
        result += "\n" + m_code + "\n";
    }
    
    // 添加结束标记
    switch (m_type) {
        case POUType::Program:
            result += "END_PROGRAM\n";
            break;
        case POUType::Function:
            result += "END_FUNCTION\n";
            break;
        case POUType::FunctionBlock:
            result += "END_FUNCTION_BLOCK\n";
            break;
    }
    
    return result;
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
