#include <ros/ros.h>
#include <std_srvs/Trigger.h>
#include <stm_client/relay_control.h>
#include <thread>
#include <chrono>

ros::ServiceClient pump_off_client;
ros::ServiceClient relay_toggle_client;

// void pumpOffCallback(const ros::TimerEvent&)
// {
//     std_srvs::Trigger srv;

//     if (pump_off_client.call(srv))
//     {
//         if (srv.response.success)
//         {
//             ROS_INFO("Pump turned off successfully");
//         }
//         else
//         {
//             ROS_WARN("Failed to turn off the pump");
//         }
//     }
//     else
//     {
//         ROS_ERROR("Failed to call service /pump_off_service");
//     }
// }

// void relayToggleCallback(const stm_client::relay_control::Request &req, stm_client::relay_control::Response &res)
// {   
    
//}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "relay_monitor_node");
    ros::NodeHandle nh;

    pump_off_client = nh.serviceClient<std_srvs::Trigger>("/pump_off_service");

    //ros::ServiceServer service = nh.advertiseService("/relay_toggle_channel", relayToggleCallback);

    relay_toggle_client = nh.serviceClient<stm_client::relay_control>("/relay_toggle_channel");
    ROS_INFO("Relay monitor node is ready");

    stm_client::relay_control srv2;
    if (srv2.request.data == 4)
    {
        ROS_INFO("Received data: 4. Will trigger /pump_off_service in 1 second");

        
        std::this_thread::sleep_for(std::chrono::seconds(1));

        
        std_srvs::Trigger srv;
        if (pump_off_client.call(srv))
        {
            if (srv.response.success)
            {
                ROS_INFO("Pump turned off successfully");
            }
            else
            {
                ROS_WARN("Failed to turn off the pump");
            }
        }
        else
        {
            ROS_ERROR("Failed to call service /pump_off_service");
        }
    }
    else
    {
        ROS_INFO("Received data: %d. No action taken");
    }

    //res.response = true;

    ros::spin();

    return 0;
}
