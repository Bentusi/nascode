#pragma once

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QList>
#include <functional>

namespace nascode {
namespace core {

/**
 * @brief 事件总线 - 用于组件间解耦通信
 */
class EventBus : public QObject
{
    Q_OBJECT

public:
    using EventHandler = std::function<void(const QVariant&)>;

    explicit EventBus(QObject* parent = nullptr);
    ~EventBus();

    /**
     * @brief 订阅事件
     * @param eventName 事件名称
     * @param handler 事件处理函数
     * @return 订阅ID(用于取消订阅)
     */
    int subscribe(const QString& eventName, EventHandler handler);

    /**
     * @brief 取消订阅
     * @param subscriptionId 订阅ID
     */
    void unsubscribe(int subscriptionId);

    /**
     * @brief 发布事件
     * @param eventName 事件名称
     * @param data 事件数据
     */
    void publish(const QString& eventName, const QVariant& data = QVariant());

signals:
    void eventPublished(const QString& eventName, const QVariant& data);

private:
    struct Subscription {
        int id;
        QString eventName;
        EventHandler handler;
    };

    QMap<QString, QList<Subscription>> m_subscriptions;
    int m_nextSubscriptionId;
};

} // namespace core
} // namespace nascode
