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


class ViconFreqEstimator
{
public:
  ViconFreqEstimator();

  double getFreq() const;

  /// Call this function in the main loop which designates
  /// moments between buffer readings with parameter main_loop = true
  /// Call it in the package reading callback too with main_loop = false
  double tick(bool main_loop = true);

private:
  double last_tick;
  double freq;
  int pack_counter;
};


class viconObjTracker: public ArtTimer
{
public:
  /// Main constructor
  /// If self_loop_freq > 0 it automatically starts the self loop using timer
  /// If you want a self looping to start just use
  /// startFreq(freq); //If freq param <0 or ommited it uses self_loop_freq provided in the constructor
  viconObjTracker(const std::string host_name, const std::string obj_name, double self_loop_freq = 100.0);

  /// Get connection frequency
  double getFreq() const;

  // Updated variables
  double x;
  double y;
  double z;
  double qw;
  double qx;
  double qy;
  double qz;

protected:
   void run(); // The loop function

private:
  // --- Functions
  void procData(const vrpn_TRACKERCB t);
  friend void VRPN_CALLBACK vicon_track_obj(void* userData, const vrpn_TRACKERCB t);

  // --- Variables
  vrpn_Connection* vicon_connection;
  vrpn_Tracker_Remote* obj_tracker;
  ViconFreqEstimator freq_estim;

};


#endif // VRPN_OBJ_H
