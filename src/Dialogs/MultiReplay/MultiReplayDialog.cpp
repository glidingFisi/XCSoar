/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2015 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "Dialogs/Message.hpp"
#include "Dialogs/WidgetDialog.hpp"
#include "Widget/ListWidget.hpp"
#include "Form/Button.hpp"
#include "Renderer/TwoTextRowsRenderer.hpp"
#include "OS/FileUtil.hpp"
#include "OS/PathName.hpp"
#include "Compatibility/path.h"
#include "LocalPath.hpp"
#include "Components.hpp"
#include "Profile/Profile.hpp"
#include "Task/ProtectedTaskManager.hpp"
#include "UIGlobals.hpp"
#include "Look/DialogLook.hpp"
#include "Interface.hpp"
#include "Language/Language.hpp"
#include "Util/StringAPI.hxx"
#include "Dialogs/MultiReplay/MultiReplayDialog.hpp"
#include "Replay/Replay.hpp"
#include "IGC/IGCParser.hpp" 
#include "Dialogs/FilePicker.hpp"

#include <map>
#include <vector>
#include <assert.h>
#include <windef.h> /* for MAX_PATH */

#include "LogFile.hpp"

/* this macro exists in the WIN32 API */
#ifdef DELETE
#undef DELETE
#endif

class MultiReplayItem
{
public:
  AllocatedPath path;
  StaticString<32> name;

  MultiReplayItem(Path _path)
    :path(_path) {
  }
};

std::vector<MultiReplayItem*> list;

class MultiReplayWidget final
  : public ListWidget, private ActionListener{

  enum Buttons {
    ADD,
    DELETE,
    START,
    SETTINGS
  };

  WndForm *form;
  Button *add_button, *delete_button, *start_button, *settings_button;
  
  

  TwoTextRowsRenderer row_renderer;

public:
  void CreateButtons(WidgetDialog &dialog);
  void AddListItem(const TCHAR* name, const Path path);
private:
  void UpdateList();  

  void AddClicked();
  void DeleteClicked();
  void StartClicked();
  void SettingsClicked();
public:
  /* virtual methods from class Widget */
  void Prepare(ContainerWindow &parent, const PixelRect &rc) override;
  void Unprepare() override;

protected:
  /* virtual methods from ListItemRenderer */
  void OnPaintItem(Canvas &canvas, const PixelRect rc, unsigned idx) override;

  /* virtual methods from ListCursorHandler */
  bool CanActivateItem(gcc_unused unsigned index) const override {
    return true;
  }

  void OnActivateItem(unsigned index) override;

private:
  /* virtual methods from class ActionListener */
  void OnAction(int id) override;
};


void
MultiReplayWidget::UpdateList()
{

  unsigned len = list.size();

  if (len > 0)
    std::sort(list.begin(), list.end());

  ListControl &list_control = GetList();
  list_control.SetLength(len);
  list_control.Invalidate();

  const bool empty = list.empty();
  delete_button->SetEnabled(!empty);
  start_button->SetEnabled(!empty);
}

void
MultiReplayWidget::CreateButtons(WidgetDialog &dialog)
{
  form = &dialog;

  add_button = dialog.AddButton(_("Add"), *this, ADD);
  delete_button = dialog.AddButton(_("Delete"), *this, DELETE);
  start_button = dialog.AddButton(_("Start"), *this, START);
  settings_button = dialog.AddButton(_("Settings"), *this, SETTINGS);
}

void
MultiReplayWidget::Prepare(ContainerWindow &parent, const PixelRect &rc)
{
  const DialogLook &look = UIGlobals::GetDialogLook();
  CreateList(parent, look, rc,
             row_renderer.CalculateLayout(*look.list.font_bold,
                                          look.small_font));

  UpdateList();
}

void
MultiReplayWidget::Unprepare()
{
  DeleteWindow();
}

void
MultiReplayWidget::OnPaintItem(Canvas &canvas, const PixelRect rc, unsigned i)
{
  

  if(i > list.size())
    assert(i > list.size());
 
  row_renderer.DrawFirstRow(canvas, rc, list[i]->name);

  row_renderer.DrawSecondRow(canvas, rc, list[i]->path.c_str());
}

inline void
MultiReplayWidget::AddClicked()
{
  for(unsigned int i =0; i<list.size();i++){
    /* TODO */
    }
    const auto path = FilePicker(_("Load IGC From File"), _T("*.igc\0"));
    if (path == nullptr)
      return;

    struct igcMetaData data;
    IGCParseFromFilePath( path,  &data);

    /* Todo duplicate */
    for(unsigned int i =0; i<list.size();i++){
      if(list[i]->name == data.registration)
        return; /* duplicate */    
    }
    
    AddListItem(data.registration,path);
    
    UpdateList();
}

inline void
MultiReplayWidget::DeleteClicked()
{
  assert(GetList().GetCursorIndex() < list.size());

  std::vector<MultiReplayItem*>::iterator it = list.begin();
  for(unsigned int i =0; i<list.size();i++, it++){

    if(GetList().GetCursorIndex() == i)
      list.erase(it);
  }

  UpdateList();
}

inline void
MultiReplayWidget::SettingsClicked()
{
  LogFormat("SettingsClicked");
  dlgMultiReplaySettingsShowModal();
  replay->SetTimeScale(20);
}

inline void
MultiReplayWidget::StartClicked()
{
  unsigned int i=0;

  if(0 == list.size()){
    return;
  }
  assert(GetList().GetCursorIndex() < list.size());

  replay->ClearTraffic();

  for(std::vector<MultiReplayItem*>::iterator it = list.begin(); it != list.end(); it++){
    LogFormat("Set Traffic list");
    if(GetList().GetCursorIndex() == i){
       /* Selective is master file */
	     //continue;	
    }
    replay->AddTrafficPath((*it)->path);
    i++;
  }
  //replay->SetTimeScale(20);
  ShowMessageBox(list[GetList().GetCursorIndex()]->name, _("Start"), MB_YESNO) ;

  try {
    replay->Start(list[GetList().GetCursorIndex()]->path);
  } catch (const std::runtime_error &e) {
    ShowMessageBox(list[GetList().GetCursorIndex()]->name, _("Start ERROR"), MB_YESNO) ;
  }

}

void 
MultiReplayWidget::OnActivateItem(unsigned i)
{
  assert(i < list.size());

  StaticString<256> tmp;
  tmp.Format(_("Activate IGC \"%s\"?"),
             list[i]->name.c_str());

  if (ShowMessageBox(tmp, _T(" "), MB_YESNO) == IDYES){
  }

   
}

void 
MultiReplayWidget::AddListItem(const TCHAR* name, const Path _path) {
  
  MultiReplayItem* item = new MultiReplayItem(_path);
  item->name = name;

  LogFormat(_T("AddListItem  %s"), item->path.GetExtension() );
  LogFormat(_T("AddListItem  %s"), item->path.c_str() );

  list.emplace_back(item);
}

void
MultiReplayWidget::OnAction(int id)
{
  switch ((Buttons)id) {
  case ADD:
    AddClicked();
    break;

  case DELETE:
    DeleteClicked();
    break;
  
  case START:
    StartClicked();
    break;

  case SETTINGS:
    SettingsClicked();
    break;
  }
}

void
ShowMultiReplayDialog()
{
  MultiReplayWidget widget;
  WidgetDialog dialog(UIGlobals::GetDialogLook());
  dialog.CreateFull(UIGlobals::GetMainWindow(), _("Replay"), &widget);
  dialog.AddButton(_("Close"), mrOK);
  widget.CreateButtons(dialog);
  dialog.EnableCursorSelection();

  dialog.ShowModal();
  dialog.StealWidget();
}