#include "OutputPanel.h"
#include <QVBoxLayout>
#include <QDateTime>

namespace nascode {
namespace views {

OutputPanel::OutputPanel(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_tabWidget = new QTabWidget(this);
    
    m_buildOutput = new QTextEdit(this);
    m_buildOutput->setReadOnly(true);
    m_tabWidget->addTab(m_buildOutput, tr("Build"));

    m_debugOutput = new QTextEdit(this);
    m_debugOutput->setReadOnly(true);
    m_tabWidget->addTab(m_debugOutput, tr("Debug"));

    m_messageOutput = new QTextEdit(this);
    m_messageOutput->setReadOnly(true);
    m_tabWidget->addTab(m_messageOutput, tr("Messages"));

    layout->addWidget(m_tabWidget);
    setLayout(layout);
}

OutputPanel::~OutputPanel()
{
}

void OutputPanel::appendMessage(const QString& message, OutputType type)
{
    QString timestamp = QDateTime::currentDateTime().toString("[hh:mm:ss] ");
    QString fullMessage = timestamp + message;

    switch (type) {
        case BuildOutput:
            m_buildOutput->append(fullMessage);
            m_tabWidget->setCurrentWidget(m_buildOutput);
            break;
        case DebugOutput:
            m_debugOutput->append(fullMessage);
            m_tabWidget->setCurrentWidget(m_debugOutput);
            break;
        case MessageOutput:
        default:
            m_messageOutput->append(fullMessage);
            break;
    }
}

void OutputPanel::clear()
{
    m_buildOutput->clear();
    m_debugOutput->clear();
    m_messageOutput->clear();
}

void OutputPanel::clearBuildOutput()
{
    m_buildOutput->clear();
}

void OutputPanel::clearDebugOutput()
{
    m_debugOutput->clear();
}

} // namespace views
} // namespace nascode
