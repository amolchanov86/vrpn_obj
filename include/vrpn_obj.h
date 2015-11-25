#ifndef VRPN_OBJ_H
#define VRPN_OBJ_H

//VRPN
#include <vrpn_Connection.h>
#include <vrpn_Tracker.h>

//MY stuff
#include <art_timer.h>

//STD
#include <string>

void VRPN_CALLBACK vicon_track_obj(void* userData, const vrpn_TRACKERCB t);

class viconObjTracker: public ArtTimer
{
public:
  // If self_loop_freq > 0 it automatically starts the self loop using timer
  viconObjTracker(const std::string host_name, const std::string obj_name, double self_loop_freq = 100.0);

  // Updated variables
  double x;
  double y;
  double z;
  double qw;
  double qx;
  double qy;
  double qz;

  // If you want a self looping to start just use
  // startFreq(freq); //If freq param <0 or ommited it uses self_loop_freq provided in the constructor

protected:
   void run(); // The loop function

private:
  // --- Functions
  void procData(const vrpn_TRACKERCB t);
  friend void VRPN_CALLBACK vicon_track_obj(void* userData, const vrpn_TRACKERCB t);

  // --- Variables
  vrpn_Connection* vicon_connection;
  vrpn_Tracker_Remote* obj_tracker;

};


#endif // VRPN_OBJ_H
