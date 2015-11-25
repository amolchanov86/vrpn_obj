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


void VRPN_CALLBACK vicon_track_obj(void* userData, const vrpn_TRACKERCB t)
{
  viconObjTracker* tracker = static_cast<viconObjTracker*>(userData);
  tracker->procData(t);
}

//double sign(double value)
//{
//    return value >= 0 ? 1.0 : -1.0;
//}


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

void viconObjTracker::run()
{
//  printf("mainLoop() \n");
  obj_tracker->mainloop();
  vicon_connection->mainloop();
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
}



//class RigidBody {
//    public:
//        RigidBody(
//            ros::NodeHandle& nh,
//            const std::string& ip,
//            int port,
//            const std::string& frame_id,
//            const std::string& child_frame_id,
//            const tf::Transform& transform,
//            const tf::Vector3& scaling)
//            : m_br()
//            , m_connection()
//            , m_tracker()
//            , m_target()
//            , m_transform(transform)
//            , m_scaling(scaling)
//        {
//            std::string connec_nm = ip + ":" + boost::lexical_cast<std::string>(port);
//            m_connection = vrpn_get_connection_by_name(connec_nm.c_str());

//            // child_frame_id is the same as remote object name
//            m_tracker = new vrpn_Tracker_Remote(child_frame_id.c_str(), m_connection);
//            m_tracker->register_change_handler(this, track_target);

//            m_target.header.frame_id = frame_id;
//            m_target.child_frame_id = child_frame_id;
//        }

//        ~RigidBody()
//        {
//            // There is no way to close the connection in VRPN.
//            // Leak both m_tracker and m_connection to avoid crashes on shutdown.
//        }

//        void step_vrpn()
//        {
//            m_tracker->mainloop();
//            m_connection->mainloop();
//        }

//    private:
//        friend void VRPN_CALLBACK track_target(void*, const vrpn_TRACKERCB);

//        void on_change(const vrpn_TRACKERCB t)
//        {
//            tf::Vector3 pos(t.pos[0], t.pos[1], t.pos[2]);
//            pos = m_transform(pos);
//            m_target.transform.translation.x = pos.x() * m_scaling.x();
//            m_target.transform.translation.y = pos.y() * m_scaling.y();
//            m_target.transform.translation.z = pos.z() * m_scaling.z();

//            tf::Quaternion quat(t.quat[0], t.quat[1], t.quat[2], t.quat[3]);
//            quat = m_transform * quat;
//            m_target.transform.rotation.x = quat.x() * sign(m_scaling.x());
//            m_target.transform.rotation.y = quat.y() * sign(m_scaling.y());
//            m_target.transform.rotation.z = quat.z() * sign(m_scaling.z());
//            m_target.transform.rotation.w = quat.w();

//            m_target.header.stamp = ros::Time::now();

//            m_br.sendTransform(m_target);
//        }

//    private:
//        tf::TransformBroadcaster m_br;
//        vrpn_Connection* m_connection;
//        vrpn_Tracker_Remote* m_tracker;
//        geometry_msgs::TransformStamped m_target;
//        tf::Transform m_transform;
//        tf::Vector3 m_scaling;
//};

//void VRPN_CALLBACK track_target(void* userData, const vrpn_TRACKERCB t)
//{
//    RigidBody* r = static_cast<RigidBody*>(userData);
//    r->on_change(t);
//}

//int main(int argc, char* argv[])
//{
//    ros::init(argc, argv, "ros_vrpn_client");
//    ros::NodeHandle nh("~");

//    std::string ip;
//    int port;
//    std::string frame_id;
//    std::string child_frame_id;
//    double x, y, z, yaw, pitch, roll;
//    double sx, sy, sz;

//    nh.param<std::string>("ip", ip, "localhost");
//    nh.param<int>("port", port, 3883);
//    nh.param<std::string>("frame_id", frame_id, "world");
//    nh.param<std::string>("child_frame_id", child_frame_id, "Tracker0");
//    nh.param<double>("x", x, 0);
//    nh.param<double>("y", y, 0);
//    nh.param<double>("z", z, 0);
//    nh.param<double>("yaw", yaw, 0);
//    nh.param<double>("pitch", pitch, 0);
//    nh.param<double>("roll", roll, 0);
//    nh.param<double>("sx", sx, 1);
//    nh.param<double>("sy", sy, 1);
//    nh.param<double>("sz", sz, 1);

//    tf::Transform transform(tf::createQuaternionFromRPY(roll / 180 * M_PI, pitch / 180 * M_PI, yaw / 180 * M_PI), tf::Vector3(x, y, z));
//    tf::Vector3 scaling(sx, sy, sz);
//    RigidBody body(nh, ip, port, frame_id, child_frame_id, transform, scaling);

//    ros::Rate loop_rate(10);

//    while(ros::ok())
//    {
//        body.step_vrpn();
//        loop_rate.sleep();
//    }

//    return 0;
//}
