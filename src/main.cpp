#include <ros/ros.h>
#include <std_srvs/Trigger.h>
#include <stm_client/relay_control.h>
#include <thread>


ros::ServiceClient pump_off_client;

// void pumpOffCallback(const ros::TimerEvent&)
// {
//     std_srvs::Trigger srv;

    
//     if (pump_off_client.call(srv))
//     {
//         if (srv.response.success)
//         {
//             ROS_INFO("Successfully called stop pump service: %s", srv.response.message.c_str());
//         }
//         else
//         {
//             ROS_WARN("Failed to call stop motors service: %s", srv.response.message.c_str());
//         }
//     }
//     else
//     {
//         ROS_ERROR("Failed to call service /pump_off_service");
//     }
// }

bool relayToggleCallback(stm_client::relay_control::Request &req, stm_client::relay_control::Response &res)
{
    if (req.data == 4)
    {
        ROS_INFO("Received data: 4. Will trigger /pump_off_service in 1 second");

        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        
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
        ROS_INFO("Received data: %d. No action taken", req.data);
    }

    res.response = true;
    return true;
}

int main(int argc, char **argv)
{
   
    ros::init(argc, argv, "relay_monitor_node");

   
    ros::NodeHandle nh;

    
    pump_off_client = nh.serviceClient<std_srvs::Trigger>("/pump_off_service");

    
    ros::ServiceServer service = nh.advertiseService("relay_toggle_channel", relayToggleCallback);
    
    ROS_INFO("Relay monitor node is ready");

    
    ros::spin();

    return 0;
}
