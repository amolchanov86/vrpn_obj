/*
# Copyright (c) 2014, Automatic Coordination of Teams Laboratory (ACT-Lab), 
#                     University of Souther California
# Copyright (c) 2011, Georgia Tech Research Corporation
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the Georgia Tech Research Corporation nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY GEORGIA TECH RESEARCH CORPORATION ''AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL GEORGIA TECH BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
# OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
# OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

## author Wolfgang Hoenig (ACT-Lab, USC)
## author Advait Jain (Healthcare Robotics Lab, Georgia Tech.)
## author Chih-Hung Aaron King (Healthcare Robotics Lab, Georgia Tech.)
*/

#include <stdio.h>
#include <math.h>

#include <vrpn_Connection.h>
#include <vrpn_Tracker.h> 

#include <vrpn_obj.h>

#include <art_time.h>

ViconFreqEstimator::ViconFreqEstimator():
  freq(0),
  last_tick(0),
  pack_counter(0)
{

}

double ViconFreqEstimator::getFreq() const
{
  return freq;
}

double ViconFreqEstimator::tick(bool main_loop)
{
  double new_tick = Art::ArtClock();
  double tick_interval = new_tick - last_tick;
//  printf("tick_interval = %f \n", tick_interval);

  if(main_loop)
  {
    if(pack_counter)
    {
      freq = (double) pack_counter  / tick_interval;
      pack_counter = 0;
      last_tick = new_tick;
    }
    else
    {
      freq = std::min( (double) 1.0  / tick_interval, freq);
    }
  }
  else
  {
    pack_counter++;
  }

  return tick_interval;
}


void VRPN_CALLBACK vicon_track_obj(void* userData, const vrpn_TRACKERCB t)
{
  viconObjTracker* tracker = static_cast<viconObjTracker*>(userData);
  tracker->procData(t);
}

viconObjTracker::viconObjTracker(const std::string host_name, const std::string obj_name, double self_loop_freq):
  ArtTimer(self_loop_freq),
  x(0),
  y(0),
  z(0),
  qw(1),
  qx(0),
  qy(0),
  qz(0)

{
  // Since if connection is not established it will not complain there is not much point in checking anything here
  vicon_connection = vrpn_get_connection_by_name( host_name.c_str() );

  if(vicon_connection == NULL)
  {
    fprintf(stderr, "ERROR: vicon connection was not established. NULL pointer returned ...\n");
    return;
  }

  // child_frame_id is the same as remote object name
  obj_tracker = new vrpn_Tracker_Remote(obj_name.c_str(), vicon_connection);
  obj_tracker->register_change_handler(this, vicon_track_obj);

  if(self_loop_freq > 0)
  {
    startFreq(self_loop_freq);
  }

}

double viconObjTracker::getFreq() const
{
  return freq_estim.getFreq();
}

void viconObjTracker::run()
{
//  printf("mainLoop() \n");
  obj_tracker->mainloop();
  vicon_connection->mainloop();
  freq_estim.tick(); //< the main loop tick.
}

void viconObjTracker::procData(const vrpn_TRACKERCB t)
{
  x = t.pos[0];
  y = t.pos[1];
  z = t.pos[2];

  qw = t.quat[3];
  qx = t.quat[0];
  qy = t.quat[1];
  qz = t.quat[2];

  freq_estim.tick(false);//< designates that the package received, but it is not the main loop tick
}
