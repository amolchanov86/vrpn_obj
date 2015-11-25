#include <vrpn_obj.h>
#include <stdio.h>

int main (int argc, char** argv)
{
  //Provide negative frequency, s.t. the self loop would not start
  viconObjTracker obj_tracker("192.168.33.3:3883","right_wrist", -1);

  //Start the self looping
  obj_tracker.startFreq(100);


  while(true)
  {
    sleep(1);
    printf("x=%f y=%f z=%f qw=%f qx=%f qy=%f qz=%f Freq = %f \n", \
           obj_tracker.x, \
           obj_tracker.y, \
           obj_tracker.z, \
           obj_tracker.qw, \
           obj_tracker.qx, \
           obj_tracker.qy, \
           obj_tracker.qz,
           obj_tracker.getFreq() \
           );
//        if ( 'x' == getchar() )
//        {
//           break;
//        }
  }



  return 0;
}
