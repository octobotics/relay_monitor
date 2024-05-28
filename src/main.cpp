#include <ros/ros.h>
#include <std_srvs/Trigger.h>
#include <stm_client/relay_control.h>


bool startPumpCallback(std_srvs::Trigger::Request &req, std_srvs::Trigger::Response &res) {
    ros::NodeHandle nh;
    ros::ServiceClient relay_client = nh.serviceClient<stm_client::relay_control>("relay_toggle_channel");
    ros::ServiceClient pump_off_client = nh.serviceClient<std_srvs::Trigger>("/pump_off_service");

    
    stm_client::relay_control relay_srv;
    relay_srv.request.data = 4;

   
    if (relay_client.call(relay_srv)) {
        ROS_INFO("Successfully sent data 4 to relay_toggle_channel");

        
        std_srvs::Trigger pump_off_srv;
        if (pump_off_client.call(pump_off_srv)) {
            if (pump_off_srv.response.success) {
                ROS_INFO("Successfully called /pump_off_service");
                res.success = true;
                res.message = "Pump turned off successfully";
                return true;
            } else {
                ROS_ERROR("Failed to execute /pump_off_service");
                res.success = false;
                res.message = "Failed to execute /pump_off_service";
                return false;
            }
        } else {
            ROS_ERROR("Failed to call service /pump_off_service");
            res.success = false;
            res.message = "Failed to call service /pump_off_service";
            return false;
        }
    } else {
        ROS_ERROR("Failed to call service relay_toggle_channel");
        res.success = false;
        res.message = "Failed to call service relay_toggle_channel";
        return false;
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "relay_monitor_node");
    ros::NodeHandle nh;

    
    ros::ServiceServer service = nh.advertiseService("/start_pump", startPumpCallback);
    ROS_INFO("Service /start_pump is ready");

    ros::spin();

    return 0;
}
