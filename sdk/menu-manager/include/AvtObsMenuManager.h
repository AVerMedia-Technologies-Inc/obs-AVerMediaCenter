#ifndef AVERMEDIA_OBS_MENU_MANAGER_H
#define AVERMEDIA_OBS_MENU_MANAGER_H

#include "AvtObsMenuManager_global.h"

#include <memory>
#include <Qt>
#include <QObject>
#include <QDockWidget>
#include <QJsonObject>

class QMainWindow;
class QMenu;
class QAction;
class QWidget;

/* AVerMedia OBS Menu Manager */
namespace AVerMedia
{

/**
 * @brief AVerMedia OBS Dock Widget spec
 */
struct AVERMEDIA_OBS_MENU_EXPORT DockWidgetSpec
{
    /**
     * @brief true if the dock widget is visible
     */
    bool visible = true;

    /**
     * @brief true if the dock widget is floating
     */
    bool floating = true;

    /**
     * @brief the area of the dock widget
     */
    Qt::DockWidgetArea area = Qt::LeftDockWidgetArea;

    /**
     * @brief the minimal width of the dock widget
     */
    int minWidth = 0;

    /**
     * @brief the minimal height of the dock widget
     */
    int minHeight = 0;

    /**
     * @brief This property holds whether the dock widget is movable, closable, and floatable
     */
    QDockWidget::DockWidgetFeatures features = QDockWidget::DockWidgetClosable |
        QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable;

    /**
     * @brief Represents the areas a QDockWidget can be plugged to.
     */
    Qt::DockWidgetAreas allowedAreas = Qt::DockWidgetArea::AllDockWidgetAreas;
};

/**
 * @brief AVerMedia OBS Menu helper class
 */
class AVERMEDIA_OBS_MENU_EXPORT ObsMenuManager : public QObject
{
    struct Impl;
    std::unique_ptr<Impl> impl;

public:
    ObsMenuManager(QMainWindow* parent = nullptr);
    ~ObsMenuManager();

    /**
     * @brief Set the logger function
     * @param logger logger function for printing log message
     */
    void setLogger(std::function<void(const char* message)> logger);

    /**
     * @brief Make a QDockWidget and its corresponding QAction
     * @param name the objectName of the QAction and QDockWidget
     * @param text the text of the QAction and QDockWidget title
     * @param widget the widget to be added to the dock widget
     * @param defaultSpec default spec when no previous stored spec is found
     * @param spec new spec to override the stored spec
     * @return QDockWidget instance.
     */
    QDockWidget* makeDockWidget(const char* name, const char* text, QWidget* widget,
                                const DockWidgetSpec& defaultSpec,
                                const DockWidgetSpec& spec);
    QDockWidget* makeDockWidget(const char* name, const char* text, QWidget* widget,
                                const DockWidgetSpec& defaultSpec);

    /**
     * @brief Show DockWidget
     * @param name the objectName of the QDockWidget
     * @return true if the dock widget is found and shown
     */
    bool showDockWidget(const char* name);

    /**
     * @brief Hide DockWidget
     * @param name the objectName of the QDockWidget
     * @return true if the dock widget is found and hidden
     */
    bool hideDockWidget(const char* name);

    /**
     * @brief Destroy DockWidget
     * @param name the objectName of the QDockWidget
     */
    void destroyDockWidget(const char* name);

    /**
     * @brief Save QDockWidget config for next launch
     * @param name the objectName of the QDockWidget
     */
    void saveGeometry(const char* name);

    /**
     * @brief Log user event
     * @param event
     */
    void userEvent(const QJsonObject& event);

    // internal use
    void* fm(const char* name);
    void* fa(const char* name);
    void* da(const char* name);
    void* dw(const char* name);
    bool aa(const char* name);
    bool bb(const char* name);
    bool cc(const char* name);
    bool rd(const char* name);
};

}

/**
 * @brief Get the version of the library
 * @return version string
 */
AVERMEDIA_OBS_MENU_EXPORT QString AVerMedia_ObsMenuManager_VersionStr();

/**
 * @brief Get the version code of the library
 * @return version code
 */
AVERMEDIA_OBS_MENU_EXPORT int AVerMedia_ObsMenuManager_VersionCode();

/**
 * @brief Get the version of the library
 * @return version string
 */
AVERMEDIA_OBS_MENU_EXPORT QString AVerMedia_ObsMenuManager_BuiltTime();

#endif // AVERMEDIA_OBS_MENU_MANAGER_H
