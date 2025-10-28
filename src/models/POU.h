#pragma once

#include <QString>
#include <QList>

namespace nascode {
namespace models {

/**
 * @brief POU类型
 */
enum class POUType {
    Program,
    Function,
    FunctionBlock
};

/**
 * @brief 变量声明
 */
struct Variable {
    QString name;
    QString dataType;
    QString initialValue;
    QString comment;
    
    enum Section {
        VAR,
        VAR_INPUT,
        VAR_OUTPUT,
        VAR_IN_OUT,
        VAR_TEMP,
        VAR_EXTERNAL,
        VAR_GLOBAL
    } section;
};

/**
 * @brief POU(程序组织单元)基类
 */
class POU
{
public:
    explicit POU(POUType type, const QString& name);
    virtual ~POU() = default;

    POUType type() const { return m_type; }
    QString name() const { return m_name; }
    void setName(const QString& name) { m_name = name; }

    QString code() const { return m_code; }
    void setCode(const QString& code) { m_code = code; }

    QList<Variable> variables() const { return m_variables; }
    void addVariable(const Variable& var) { m_variables.append(var); }
    void removeVariable(const QString& name);
    Variable* getVariable(const QString& name);

    QString comment() const { return m_comment; }
    void setComment(const QString& comment) { m_comment = comment; }

    // 序列化
    QVariantMap toVariant() const;
    void fromVariant(const QVariantMap& data);
    
    // 生成ST代码文本
    QString generateSTCode() const;

protected:
    POUType m_type;
    QString m_name;
    QString m_code;
    QString m_comment;
    QList<Variable> m_variables;
};

/**
 * @brief 程序(PROGRAM)
 */
class Program : public POU
{
public:
    explicit Program(const QString& name);
};

/**
 * @brief 函数(FUNCTION)
 */
class Function : public POU
{
public:
    explicit Function(const QString& name);
    
    QString returnType() const { return m_returnType; }
    void setReturnType(const QString& type) { m_returnType = type; }

private:
    QString m_returnType;
};

/**
 * @brief 功能块(FUNCTION_BLOCK)
 */
class FunctionBlock : public POU
{
public:
    explicit FunctionBlock(const QString& name);
};

} // namespace models
} // namespace nascode
