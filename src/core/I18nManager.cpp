#include "I18nManager.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QDir>
#include <QSettings>
#include <spdlog/spdlog.h>

namespace nascode {
namespace core {

I18nManager::I18nManager(QObject* parent)
    : QObject(parent)
    , m_currentLanguage(SystemDefault)
    , m_appTranslator(std::make_unique<QTranslator>())
    , m_qtTranslator(std::make_unique<QTranslator>())
{
}

I18nManager::~I18nManager()
{
    removeTranslation();
}

I18nManager& I18nManager::getInstance()
{
    static I18nManager instance;
    return instance;
}

bool I18nManager::initialize()
{
    spdlog::info("Initializing I18n Manager...");

    // 从设置中读取上次选择的语言
    QSettings settings("NasCode", "NasCode");
    QString langStr = settings.value("language", "System").toString();
    Language lang = languageFromString(langStr);

    if (lang == SystemDefault) {
        // 根据系统语言自动选择
        QLocale locale;
        if (locale.language() == QLocale::Chinese) {
            lang = Chinese;
        } else {
            lang = English;
        }
        spdlog::info("Using system locale: {}", 
            (lang == Chinese ? "Chinese" : "English"));
    }

    return switchLanguage(lang);
}

bool I18nManager::switchLanguage(Language lang)
{
    if (lang == m_currentLanguage) {
        return true;
    }

    spdlog::info("Switching language to: {}", languageToString(lang).toStdString());

    // 移除旧的翻译
    removeTranslation();

    // 加载新的翻译
    QString translationFile;
    QString qtTranslationFile;

    switch (lang) {
        case Chinese:
            translationFile = "nascode_zh_CN";
            qtTranslationFile = "qt_zh_CN";
            break;
        case English:
        default:
            translationFile = "nascode_en_US";
            qtTranslationFile = "qt_en";
            break;
    }

    // 加载应用程序翻译
    // 尝试多个可能的路径
    QStringList searchPaths;
    searchPaths << QApplication::applicationDirPath() + "/translations"
                << QApplication::applicationDirPath() + "/../translations"
                << QApplication::applicationDirPath();

    bool loaded = false;
    for (const QString& path : searchPaths) {
        if (m_appTranslator->load(translationFile, path)) {
            QApplication::installTranslator(m_appTranslator.get());
            spdlog::debug("Loaded application translation: {} from {}", 
                translationFile.toStdString(), path.toStdString());
            loaded = true;
            break;
        }
    }
    
    if (!loaded) {
        spdlog::warn("Failed to load application translation: {} (searched {} paths)", 
            translationFile.toStdString(), searchPaths.size());
    }

    // 加载Qt标准翻译
    QString qtTransPath = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
    if (m_qtTranslator->load(qtTranslationFile, qtTransPath)) {
        QApplication::installTranslator(m_qtTranslator.get());
        spdlog::debug("Loaded Qt translation: {}", qtTranslationFile.toStdString());
    } else {
        spdlog::debug("Qt translation not found: {}", qtTranslationFile.toStdString());
    }

    m_currentLanguage = lang;

    // 保存语言设置
    QSettings settings("NasCode", "NasCode");
    settings.setValue("language", languageToString(lang));

    emit languageChanged(lang);

    spdlog::info("Language switched successfully");
    return true;
}

void I18nManager::removeTranslation()
{
    if (m_appTranslator) {
        QApplication::removeTranslator(m_appTranslator.get());
    }
    if (m_qtTranslator) {
        QApplication::removeTranslator(m_qtTranslator.get());
    }
}

QStringList I18nManager::supportedLanguages() const
{
    return QStringList() << "English" << "Chinese";
}

I18nManager::Language I18nManager::languageFromString(const QString& langName)
{
    if (langName == "English" || langName == "en" || langName == "en_US") {
        return English;
    } else if (langName == "Chinese" || langName == "zh" || langName == "zh_CN") {
        return Chinese;
    } else {
        return SystemDefault;
    }
}

QString I18nManager::languageToString(Language lang)
{
    switch (lang) {
        case English:
            return "English";
        case Chinese:
            return "Chinese";
        case SystemDefault:
        default:
            return "System";
    }
}

} // namespace core
} // namespace nascode
