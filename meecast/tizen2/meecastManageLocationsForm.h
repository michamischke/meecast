#ifndef _MEECAT_MANAGELOCATIONS_FORM_H_
#define _MEECAST_MANAGELOCATIONS_FORM_H_

#include <FBase.h>
#include <FUi.h>

#include "configtizen.h"

#include <libintl.h>
#include <locale.h>
#define _(String) gettext(String)

class meecastLocationManager;
class meecastManageLocationsForm
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::Controls::IFormBackEventListener
	, public Tizen::Ui::Controls::IListViewItemEventListener
	, public Tizen::Ui::Controls::IListViewItemProvider
 	, public Tizen::Ui::Scenes::ISceneEventListener
 	, public Tizen::App::IAppControlResponseListener
{
public:
	static const RequestId LOC_MGR_DRAW_SYNC_LOC_UPDATE = 101;
	static const RequestId LOC_MGR_NOTIFY_ERROR = 102;
    static const RequestId UPDATE_LIST = 111;

public:
    meecastManageLocationsForm(void);
    virtual ~meecastManageLocationsForm(void);
    bool Initialize(void);
public:
    virtual result OnInitializing(void);
    virtual result OnTerminating(void);
    virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);
    virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);
    virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
    virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
									const Tizen::Ui::Scenes::SceneId& nextSceneId);

    virtual void OnListViewItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListItemStatus status);
    virtual void OnListViewItemSwept(Tizen::Ui::Controls::ListView& listView, int index, Tizen::Ui::Controls::SweepDirection direction);
    virtual void OnListViewContextItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListContextItemStatus state);
    virtual void OnItemReordered(Tizen::Ui::Controls::ListView& view, int oldIndex, int newIndex);
    virtual Tizen::Ui::Controls::ListItemBase* CreateItem(int index, int itemWidth);
    virtual bool DeleteItem(int index, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth);
    virtual int GetItemCount(void);
    void GetStationsList(void);

    void DeleteMessageBox(const Tizen::Base::String& Station, int index);
    bool CheckLocationSetting(void);
    void LaunchLocationSettings(void);

    /* Events */
    virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);
    virtual void OnAppControlCompleteResponseReceived(const Tizen::App::AppId &appId, const Tizen::Base::String &operationId, Tizen::App::AppCtrlResult appControlResult, const Tizen::Base::Collection::IMap *pExtraData){}


protected:
private:
    Tizen::Ui::Controls::ListView* __pListView;
    Tizen::Base::Collection::ArrayList __stationsList;
    ConfigTizen *_config;
	meecastLocationManager* __pLocationManagerThread;
};

#endif	//_MEECAST_MANAGELOCATIONS_FORM_H_