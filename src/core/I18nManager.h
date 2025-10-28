#pragma once

#include <QObject>
#include <QTranslator>
#include <QLocale>
#include <memory>

namespace nascode {
namespace core {

/**
 * @brief 国际化管理器
 * 管理应用程序的多语言支持
 */
class I18nManager : public QObject
{
    Q_OBJECT

public:
    enum Language {
        English,
        Chinese,
        SystemDefault
    };

    explicit I18nManager(QObject* parent = nullptr);
    ~I18nManager();

    /**
     * @brief 获取单例实例
     */
    static I18nManager& getInstance();

    /**
     * @brief 初始化国际化系统
     */
    bool initialize();

    /**
     * @brief 切换语言
     * @param lang 目标语言
     */
    bool switchLanguage(Language lang);

    /**
     * @brief 获取当前语言
     */
    Language currentLanguage() const { return m_currentLanguage; }

    /**
     * @brief 获取支持的语言列表
     */
    QStringList supportedLanguages() const;

    /**
     * @brief 语言名称到枚举的转换
     */
    static Language languageFromString(const QString& langName);
    static QString languageToString(Language lang);

signals:
    /**
     * @brief 语言已切换信号
     */
    void languageChanged(Language newLang);

private:
    void loadTranslation(const QString& filename);
    void removeTranslation();

private:
    Language m_currentLanguage;
    std::unique_ptr<QTranslator> m_appTranslator;
    std::unique_ptr<QTranslator> m_qtTranslator;
};

} // namespace core
} // namespace nascode
