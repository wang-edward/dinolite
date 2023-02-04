#pragma once


#include <cmath>
#include <string>

namespace dinolite {

    struct CameraContext {
        const bool file = false;
        const int fps = 30;
        const std::string filename = "";
        const int index = 0; // 0 is default index of webcam
        const int width = 640;
        const int height = 480;
        const std::string camera_info_path = "info.ini";
        const std::string camera_frame_id = "camera_frame";
    };

}