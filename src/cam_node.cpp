#include "dinolite/cam_node.hpp"

namespace dinolite {

    std::string mat_type2encoding(int mat_type) {
        switch (mat_type) {
        case CV_8UC1:
            return "mono8";
        case CV_8UC3:
            return "bgr8";
        case CV_16SC1:
            return "mono16";
        case CV_8UC4:
            return "rgba8";
        default:
            throw std::runtime_error("unsupported encoding type");
        }
    }

    CamNode:: CamNode(const rclcpp::NodeOptions &options) : Node("dinolite_cam", options) {
        RCLCPP_INFO(get_logger(), "hello wordln");
        RCLCPP_INFO(get_logger(), "OpenCV version: %d", CV_VERSION_MAJOR);

        capture_ = std::make_shared<cv::VideoCapture>(cxt_.index_);

        if (!capture_->isOpened()) {
            RCLCPP_ERROR(get_logger(), "cannot open device %d", cxt_.index_);
            return;
        }
        if (cxt_.height_ > 0) {
            capture_->set(cv::CAP_PROP_FRAME_HEIGHT, cxt_.height_);
        }
        if (cxt_.width_ > 0) {
            capture_->set(cv::CAP_PROP_FRAME_WIDTH, cxt_.width_);
        }
        if (cxt_.fps_ > 0) {
            capture_->set(cv::CAP_PROP_FPS, cxt_.fps_);
        }

        double width = capture_->get(cv::CAP_PROP_FRAME_WIDTH);
        double height = capture_->get(cv::CAP_PROP_FRAME_HEIGHT);
        double fps = capture_->get(cv::CAP_PROP_FPS);

        RCLCPP_INFO(get_logger(), 
            "device %d open, width %g, height %g, device fps %g",
            cxt_.index_, width, height, fps
        );

        assert(!cxt_.camera_info_path_.empty()); // readCalibration will crash if file_name is ""

        std::string camera_name;
        if (camera_calibration_parsers::readCalibration(cxt_.camera_info_path_, camera_name, camera_info_msg_)) {
            RCLCPP_INFO(get_logger(), "got camera info for '%s'", camera_name.c_str());
            camera_info_msg_.header.frame_id = cxt_.camera_frame_id_;
            camera_info_pub_ = create_publisher<sensor_msgs::msg::CameraInfo>("camera_info", 10);
        } else {
            RCLCPP_ERROR(get_logger(), "cannot get camera info, will not publish");
            camera_info_pub_ = nullptr;
        }

        image_pub_ = create_publisher<sensor_msgs::msg::Image>("image_raw", 10);

        // send message
        timer_ = create_wall_timer(
            std::chrono::duration
                <int, std::chrono::millliseconds::period>(1),
                std::bind(CamNode::frame, this)
        );

    }

    CamNode:: frame() {
        cv::Mat frame;
        while(rclcpp::ok()) {
            if (!capture_->read(frame)) {
                RCLCPP_INFO(get_logger(), "EOF, stop publishing");
                break;
            }

            auto stamp = now();

            sensor_msgs::msg::Image::UniquePtr image_msg(new sensor_msgs::msg::Image());

            image_msg->header.stamp = stamp;
            image_msg->header.frame_id = cxt_.camera_frame_id_;
            image_msg->height = frame.rows;
            image_msg->width = frame.cols;
            image_msg->encoding = mat_type2encoding(frame.type());
            image_msg->is_bigendian = false;
            image_msg->step = static_cast<sensor_msgs::msg::Image::_step_type>(frame.step);
            image_msg->data.assign(frame.datastart, frame.dataend);

            // publish message
            image_pub_->publish(std::move(image_msg));
            if (camera_info_pub_) {
                camera_info_msg_.header.stamp = stamp;
                camera_info_pub_->publish(camera_info_msg_);
            }
        }
    }

} // namespace dinolite

#include "rclcpp_components/register_node_macro.hpp"

RCLCPP_COMPONENTS_REGISTER_NODE(dinolite::CamNode)