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

#ifndef REPLAY_HPP
#define REPLAY_HPP

#include "Event/Timer.hpp"
#include "NMEA/Info.hpp"
#include "Time/PeriodClock.hpp"
#include "OS/Path.hpp"
#include <list>

class Logger;
class ProtectedTaskManager;
class AbstractReplay;
class CatmullRomInterpolator;
class Error;

class ReplayItem
{
public:
  AllocatedPath path;
  AbstractReplay *replay;
  NMEAInfo next_data;
  StaticString<32> name;
  std::vector<NMEAInfo> nmea_trace;
  
  ReplayItem(Path _path)
    :path(_path), replay(nullptr) {
  }
};

class Replay final
  : private Timer
{
  double time_scale;

  AbstractReplay *replay;

  Logger *logger;
  ProtectedTaskManager &task_manager;

  AllocatedPath path = nullptr;
  
  std::vector<ReplayItem*> itemList;

  struct ListItem {
    Path path;
    AbstractReplay *replay;
    NMEAInfo next_data;
    char name[20];
    
   /* ListItem(const Path _path)
      : path(_path) {}*/
  };

  std::vector<ListItem> list;

  /**
   * The time of day according to replay input.  This is negative if
   * unknown.
   */
  double virtual_time;

  /**
   * If this value is not negative, then we're in fast-forward mode:
   * replay is going as quickly as possible.  This value denotes the
   * time stamp when we will stop going fast-forward.  If
   * #virtual_time is negative, then this is the duration, and
   * #virtual_time will be added as soon as it is known.
   */
  double fast_forward;

  /**
   * Keeps track of the wall-clock time between two Update() calls.
   */
  PeriodClock clock;

  /**
   * The last NMEAInfo returned by the #AbstractReplay instance.  It
   * is held back until #virtual_time has passed #next_data.time.
   */
  NMEAInfo next_data;

  CatmullRomInterpolator *cli;

public:
  Replay(Logger *_logger, ProtectedTaskManager &_task_manager)
    :time_scale(1), replay(nullptr),
     logger(_logger), task_manager(_task_manager), cli(nullptr) {
  }

  ~Replay() {
    Stop();
  }

  bool IsActive() const {
    return replay != nullptr;
  }

private:
  bool Update();

public:
  void Stop();

  /**
   * Throws std::runtime_errror on error.
   */
  void Start(Path _path);

  //bool AddTrafficPath(std::list<Path> _path_list);
  bool AddTrafficPath(Path _path);
  void ClearTraffic();
  unsigned int GetTrafficCount(){
    return  itemList.size();
  }
  GeoPoint GetTrafficItem(unsigned int item){
    return itemList[item]->next_data.location;
  }
  std::vector<NMEAInfo> GetTrafficItemTrace(unsigned int item){
    return itemList[item]->nmea_trace;
  }
  int GetAltitudeItem(unsigned int item){
    return itemList[item]->next_data.gps_altitude;
  }
  int GetTimeItem(unsigned int item){
    return /*list[item].next_data.time*/ 0;
  }
  const TCHAR * GetNameItem(unsigned int item){
    return itemList[item]->name;
  }

  Path GetFilename() const {
    return path;
  }

  double GetTimeScale() const {
    return time_scale;
  }

  void SetTimeScale(const double _time_scale) {
    time_scale = _time_scale;
  }

  /**
   * Start fast-forwarding the replay by the specified number of
   * seconds.  This replays the given amount of time from the input
   * time as quickly as possible.  Returns false if unable to fast forward.
   */
  bool FastForward(double delta_s) {
    if (!IsActive())
      return false;

    fast_forward = delta_s;
    if (virtual_time >= 0) {
      fast_forward += virtual_time;
      return true;
    } else {
      return false;
    }
  }

  double GetVirtualTime() const {
    return virtual_time;
  }

private:
  void OnTimer() override;
};

#endif
