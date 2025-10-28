#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QTabWidget>

namespace nascode {
namespace views {

/**
 * @brief 输出面板 - 显示编译、调试等输出信息
 */
class OutputPanel : public QWidget
{
    Q_OBJECT

public:
    enum OutputType {
        BuildOutput,
        DebugOutput,
        MessageOutput
    };

    explicit OutputPanel(QWidget* parent = nullptr);
    ~OutputPanel();

public slots:
    void appendMessage(const QString& message, OutputType type = MessageOutput);
    void clear();
    void clearBuildOutput();
    void clearDebugOutput();

private:
    QTabWidget* m_tabWidget;
    QTextEdit* m_buildOutput;
    QTextEdit* m_debugOutput;
    QTextEdit* m_messageOutput;
};

} // namespace views
} // namespace nascode
