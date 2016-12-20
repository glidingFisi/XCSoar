/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2016 The XCSoar Project
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

#include "Dialogs/Dialogs.h"
#include "Dialogs/WidgetDialog.hpp"
#include "Blackboard/DeviceBlackboard.hpp"
#include "Computer/Settings.hpp"
#include "Units/Units.hpp"
#include "Units/Group.hpp"
#include "Formatter/UserUnits.hpp"
#include "Atmosphere/Temperature.hpp"
#include "Form/DataField/Float.hpp"
#include "Form/DataField/Listener.hpp"
#include "UIGlobals.hpp"
#include "Interface.hpp"
#include "Components.hpp"
#include "GlideSolvers/GlidePolar.hpp"
#include "Task/ProtectedTaskManager.hpp"
#include "Widget/RowFormWidget.hpp"
#include "Form/Button.hpp"
#include "Language/Language.hpp"
#include "Operation/MessageOperationEnvironment.hpp"
#include "Event/Timer.hpp"

#include "LogFile.hpp"
#include <math.h>

#include "Dialogs/MultiReplay/MultiReplayDialog.hpp"

enum ControlIndex {
  ReplaySpeed
};

class MultiReplaySettingsPanel final
  : public RowFormWidget, DataFieldListener,
    private Timer,
    public ActionListener {

public:
  MultiReplaySettingsPanel()
    :RowFormWidget(UIGlobals::GetDialogLook())
  {}

  void SetReplaySpeed(double speed);

  /* virtual methods from Widget */
  virtual void Prepare(ContainerWindow &parent,
                       const PixelRect &rc) override;

  virtual void Show(const PixelRect &rc) override {
  }

  /* virtual methods from ActionListener */
  virtual void OnAction(int id) override;

private:
  /* virtual methods from DataFieldListener */
  virtual void OnModified(DataField &df) override;

  /* virtual methods from Timer */
  virtual void OnTimer() override;
};

void MultiReplaySettingsPanel::SetReplaySpeed(double speed) {
    LogFormat("Replay AddTrafficPath %f",speed);
  }

void
MultiReplaySettingsPanel::OnModified(DataField &df)
{
  if (IsDataField(ReplaySpeed, df)) {
    const DataFieldFloat &dff = (const DataFieldFloat &)df;
    SetReplaySpeed(dff.GetAsFixed());
  } 
}

void
MultiReplaySettingsPanel::Prepare(ContainerWindow &parent, const PixelRect &rc)
{
  RowFormWidget::Prepare(parent, rc);

  WndProperty *wp;

  wp = AddFloat(_("Replay Speed"),
                _("Set to forecast ground temperature.  Used by convection estimator (temperature trace page of Analysis dialog)"),
                _T("%.0f %s"), _T("%.0f"),
                1,
                100,
                5, false,
                1);
  {
    wp->RefreshDisplay();
  }
}

void
MultiReplaySettingsPanel::OnTimer()
{
  
}

void
MultiReplaySettingsPanel::OnAction(int id)
{
  
}

void
dlgMultiReplaySettingsShowModal()
{
  MultiReplaySettingsPanel *instance = new MultiReplaySettingsPanel();

  StaticString<128> caption(_("Multi Replay Settings"));

  WidgetDialog dialog(UIGlobals::GetDialogLook());
  dialog.CreateAuto(UIGlobals::GetMainWindow(), caption, instance);
  dialog.AddButton(_("OK"), mrOK);

  dialog.ShowModal();
}
