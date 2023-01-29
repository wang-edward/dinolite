#include "dinolite/CamNode.hpp"

namespace dinolite {

    CamNode:: CamNode(const rclcpp::NodeOptions &options) : Node("CamNode", options) {
        RCLCPP_INFO(get_logger(), "hello wordln");
    }
} // namespace dinolite