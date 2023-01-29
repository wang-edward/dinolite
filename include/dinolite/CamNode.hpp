#pragma once

#include <string>
#include <rclcpp/rclcpp.hpp>

namespace dinolite {

class CamNode : public rclcpp::Node {

public:
    explicit CamNode(const rclcpp::NodeOptions &options);

private:
    // CameraContext cxt_;
    // std::shared_ptr<cv::VideoCapture> capture_;

    void printer();

};

} // namespace dinolite

#include "rclcpp_components/register_node_macro.hpp"

RCLCPP_COMPONENTS_REGISTER_NODE(dinolite::CamNode)