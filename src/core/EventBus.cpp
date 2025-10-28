#include "EventBus.h"
#include <spdlog/spdlog.h>

namespace nascode {
namespace core {

EventBus::EventBus(QObject* parent)
    : QObject(parent)
    , m_nextSubscriptionId(1)
{
}

EventBus::~EventBus()
{
    m_subscriptions.clear();
}

int EventBus::subscribe(const QString& eventName, EventHandler handler)
{
    Subscription sub;
    sub.id = m_nextSubscriptionId++;
    sub.eventName = eventName;
    sub.handler = handler;

    m_subscriptions[eventName].append(sub);
    
    spdlog::debug("Subscribed to event: {} (ID: {})", eventName.toStdString(), sub.id);
    return sub.id;
}

void EventBus::unsubscribe(int subscriptionId)
{
    for (auto& list : m_subscriptions) {
        for (int i = 0; i < list.size(); ++i) {
            if (list[i].id == subscriptionId) {
                spdlog::debug("Unsubscribed from event (ID: {})", subscriptionId);
                list.removeAt(i);
                return;
            }
        }
    }
}

void EventBus::publish(const QString& eventName, const QVariant& data)
{
    spdlog::debug("Publishing event: {}", eventName.toStdString());
    
    emit eventPublished(eventName, data);

    if (m_subscriptions.contains(eventName)) {
        const auto& subscriptions = m_subscriptions[eventName];
        for (const auto& sub : subscriptions) {
            try {
                sub.handler(data);
            } catch (const std::exception& e) {
                spdlog::error("Error in event handler: {}", e.what());
            }
        }
    }
}

} // namespace core
} // namespace nascode
