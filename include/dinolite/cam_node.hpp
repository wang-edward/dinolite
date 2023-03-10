#pragma once

#include <string>
#include "opencv2/highgui/highgui.hpp"
#include <rclcpp/rclcpp.hpp>
#include "sensor_msgs/msg/camera_info.hpp"
#include "sensor_msgs/msg/image.hpp"

namespace dinolite {

class CamNode : public rclcpp::Node {

public:
    explicit CamNode(const rclcpp::NodeOptions &options);

private:
    void validate_parameters();
    void frame();

    void printer();

    CameraContext cxt_;
    std::shared_ptr<cv::VideoCapture> capture_;

    sensor_msgs::msg::CameraInfo camera_info_msg_;

    int publish_fps_;
    rclcpp::Time next_stamp_;

    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr image_pub_;
    rclcpp::Publisher<sensor_msgs::msg::CameraInfo>::SharedPtr camera_info_pub_;
};

} // namespace dinolite