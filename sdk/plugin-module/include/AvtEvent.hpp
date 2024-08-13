#ifndef AVT_PLUGIN_MODULE_EVENT_H
#define AVT_PLUGIN_MODULE_EVENT_H

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

namespace AVerMedia
{

enum EventType
{
    UNKNOWN = 0,

    LOG_DEBUG = 701,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,

    /* OBS events: https://docs.obsproject.com/reference-frontend-api */

    /* Triggered when streaming is starting. */
    OBS_EVENT_STREAMING_STARTING = 1000,
    /* Triggered when streaming has successfully started. */
    OBS_EVENT_STREAMING_STARTED,
    /* Triggered when streaming is stopping. */
    OBS_EVENT_STREAMING_STOPPING,
    /* Triggered when streaming has fully stopped. */
    OBS_EVENT_STREAMING_STOPPED,
    /* Triggered when recording is starting. */
    OBS_EVENT_RECORDING_STARTING,
    /* Triggered when recording has successfully started. */
    OBS_EVENT_RECORDING_STARTED,
    /* Triggered when recording is stopping. */
    OBS_EVENT_RECORDING_STOPPING,
    /* Triggered when recording has fully stopped. */
    OBS_EVENT_RECORDING_STOPPED,

    /* Triggered when the program is about to exit. This is the last chance to call any frontend
     * API functions for any saving / cleanup / etc. After returning from this event callback,
     * it is not permitted to make any further frontend API calls.*/
    OBS_EVENT_EXIT = 1017,

    /* Triggered when the program has finished loading. */
    OBS_EVENT_FINISHED_LOADING = 1026,

    /* Triggered when the theme is changed. */
    OBS_EVENT_THEME_CHANGED = 1039,
    /* Triggered when a screenshot is taken. */
    OBS_EVENT_SCREENSHOT_TAKEN,

    /* AVerMedia events: https://confluence.avermedia.com/display/OPD/Background+Service+Messages */

    AVT_EVENT_START_STREAMING = 2000,
    AVT_EVENT_STOP_STREAMING,
    AVT_EVENT_START_RECORDING,
    AVT_EVENT_STOP_RECORDING,
    AVT_EVENT_TAKE_SCREENSHOT,

    AVT_EVENT_QUERY_STREAMING,
    AVT_EVENT_QUERY_RECORDING,
	AVT_EVENT_QUERY_THEME,
    AVT_EVENT_QUERY_LAST_RECORDING,
    AVT_EVENT_QUERY_LAST_SCREENSHOT,

    AVT_EVENT_DOCK_VISIBILITY_CHANGED,
    AVT_EVENT_DOCK_TOP_LEVEL_CHANGED,
    AVT_EVENT_DOCK_LOCATION_CHANGED,

    AVT_EVENT_USB_START_CDC,
    AVT_EVENT_USB_STOP_CDC,
    AVT_EVENT_USB_DEVICE_ATTACHED,
    AVT_EVENT_USB_DEVICE_DETACHED,

    AVT_EVENT_QUERY_MODULE_DATA_PATH,
    AVT_EVENT_QUERY_VERSION_INI_PATH,
    AVT_EVENT_QUERY_USB_DEVICE_LIST,

    AVT_EVENT_SHOW_MESSAGE_BOX,
    AVT_EVENT_SHOW_STATUS_BAR_MESSAGE,
    AVT_EVENT_SHOW_CDC_MESSAGE,

    AVT_EVENT_GA,
    AVT_EVENT_SHUTDOWN,

    AVT_EVENT_DELETE_CDC_LOG
};

/**
 * @brief Event Info
 */
struct EventInfo
{
    EventType type;
    QJsonObject data;

    EventInfo() : type(UNKNOWN) {}
    EventInfo(EventType type) : type(type) {}
    EventInfo(EventType type, const QJsonObject &data) : type(type), data(data)
    {
    }
    EventInfo(int what, const char* data) : type(static_cast<EventType>(what))
    {
        if (data) {
            QJsonDocument doc = QJsonDocument::fromJson(data);
            if (!doc.isNull()) this->data = doc.object();
        }
    }

    /**
     * @brief Convert event message from json object
     * @param obj
     * @return
     */
    [[maybe_unused]] static EventInfo fromJson(const QJsonObject &obj)
    {
        EventInfo info;
        info.type = static_cast<EventType>(obj["type"].toInt());
        info.data = obj["data"].toObject();
        return info;
    }

    /**
     * @brief Convert event message to json object
     * @return
     */
    [[maybe_unused]] QJsonObject toJson() const
    {
        QJsonObject obj;
        obj["type"] = static_cast<int>(type);
        obj["data"] = data;
        return obj;
    }

    /**
     * @brief Convert event type to string
     * @return
    */
    [[maybe_unused]] QString typeToString() const
    {
        switch (type) {
        case EventType::LOG_DEBUG:
            return "LOG_DEBUG";
        case EventType::LOG_INFO:
            return "LOG_INFO";
        case EventType::LOG_WARNING:
            return "LOG_WARNING";
        case EventType::LOG_ERROR:
            return "LOG_ERROR";

        case EventType::OBS_EVENT_STREAMING_STARTING:
            return "OBS_EVENT_STREAMING_STARTING";
        case EventType::OBS_EVENT_STREAMING_STARTED:
            return "OBS_EVENT_STREAMING_STARTED";
        case EventType::OBS_EVENT_STREAMING_STOPPING:
            return "OBS_EVENT_STREAMING_STOPPING";
        case EventType::OBS_EVENT_STREAMING_STOPPED:
            return "OBS_EVENT_STREAMING_STOPPED";
        case EventType::OBS_EVENT_RECORDING_STARTING:
            return "OBS_EVENT_RECORDING_STARTING";
        case EventType::OBS_EVENT_RECORDING_STARTED:
            return "OBS_EVENT_RECORDING_STARTED";
        case EventType::OBS_EVENT_RECORDING_STOPPING:
            return "OBS_EVENT_RECORDING_STOPPING";
        case EventType::OBS_EVENT_RECORDING_STOPPED:
            return "OBS_EVENT_RECORDING_STOPPED";
        case EventType::OBS_EVENT_EXIT:
            return "OBS_EVENT_EXIT";
        case EventType::OBS_EVENT_FINISHED_LOADING:
            return "OBS_EVENT_FINISHED_LOADING";
        case EventType::OBS_EVENT_THEME_CHANGED:
            return "OBS_EVENT_THEME_CHANGED";
        case EventType::OBS_EVENT_SCREENSHOT_TAKEN:
            return "OBS_EVENT_SCREENSHOT_TAKEN";

        case EventType::AVT_EVENT_START_STREAMING:
            return "AVT_EVENT_START_STREAMING";
        case EventType::AVT_EVENT_STOP_STREAMING:
            return "AVT_EVENT_STOP_STREAMING";
        case EventType::AVT_EVENT_START_RECORDING:
            return "AVT_EVENT_START_RECORDING";
        case EventType::AVT_EVENT_STOP_RECORDING:
            return "AVT_EVENT_STOP_RECORDING";
        case EventType::AVT_EVENT_TAKE_SCREENSHOT:
            return "AVT_EVENT_TAKE_SCREENSHOT";

        case EventType::AVT_EVENT_QUERY_STREAMING:
            return "AVT_EVENT_QUERY_STREAMING";
        case EventType::AVT_EVENT_QUERY_RECORDING:
            return "AVT_EVENT_QUERY_RECORDING";
        case EventType::AVT_EVENT_QUERY_THEME:
            return "AVT_EVENT_QUERY_THEME";
        case EventType::AVT_EVENT_QUERY_LAST_RECORDING:
            return "AVT_EVENT_QUERY_LAST_RECORDING";
        case EventType::AVT_EVENT_QUERY_LAST_SCREENSHOT:
            return "AVT_EVENT_QUERY_LAST_SCREENSHOT";

        case EventType::AVT_EVENT_DOCK_VISIBILITY_CHANGED:
            return "AVT_EVENT_DOCK_VISIBILITY_CHANGED";
        case EventType::AVT_EVENT_DOCK_TOP_LEVEL_CHANGED:
            return "AVT_EVENT_DOCK_TOP_LEVEL_CHANGED";
        case EventType::AVT_EVENT_DOCK_LOCATION_CHANGED:
            return "AVT_EVENT_DOCK_LOCATION_CHANGED";

        case EventType::AVT_EVENT_USB_START_CDC:
            return "AVT_EVENT_USB_START_CDC";
        case EventType::AVT_EVENT_USB_STOP_CDC:
            return "AVT_EVENT_USB_STOP_CDC";
        case EventType::AVT_EVENT_USB_DEVICE_ATTACHED:
            return "AVT_EVENT_USB_DEVICE_ATTACHED";
        case EventType::AVT_EVENT_USB_DEVICE_DETACHED:
            return "AVT_EVENT_USB_DEVICE_DETACHED";

        case EventType::AVT_EVENT_QUERY_MODULE_DATA_PATH:
            return "AVT_EVENT_QUERY_MODULE_DATA_PATH";
        case EventType::AVT_EVENT_QUERY_VERSION_INI_PATH:
            return "AVT_EVENT_QUERY_VERSION_INI_PATH";
        case EventType::AVT_EVENT_QUERY_USB_DEVICE_LIST:
            return "AVT_EVENT_QUERY_USB_DEVICE_LIST";

        case EventType::AVT_EVENT_SHOW_MESSAGE_BOX:
            return "AVT_EVENT_SHOW_MESSAGE_BOX";
        case EventType::AVT_EVENT_SHOW_STATUS_BAR_MESSAGE:
            return "AVT_EVENT_SHOW_STATUS_BAR_MESSAGE";
        case EventType::AVT_EVENT_GA:
            return "AVT_EVENT_GA";
        case EventType::AVT_EVENT_SHUTDOWN:
            return "AVT_EVENT_SHUTDOWN";
        case EventType::AVT_EVENT_DELETE_CDC_LOG:
            return "AVT_EVENT_DELETE_CDC_LOG";

        default:
            return "UNKNOWN";
        }
    }

    /**
     * @brief Convert event data to byte array
     * @return
     */
    [[maybe_unused]] QByteArray dataToByteArray() const
    {
        if (data.isEmpty())
            return QByteArray();
        return QJsonDocument(data).toJson(QJsonDocument::Compact);
    }
};
}

#endif // AVT_PLUGIN_MODULE_EVENT_H
