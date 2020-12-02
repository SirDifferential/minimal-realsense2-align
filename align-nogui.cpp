#include <librealsense2/rs.hpp>
#include <librealsense2/rs_advanced_mode.hpp>
#include <iostream>
#include "rs_settings.h"

enum class direction {
    to_depth,
    to_color
};

int main(int argc, char * argv[]) try {
	rs2::context context;
    rs2::pipeline pipe(context);

	rs2::device_list devs = context.query_devices();
	if (devs.size() != 1) {
		std::cout << "No Realsense devices found" << std::endl;
		return 1;
	}

	std::cout << "There are " << devs.size() << " devices connected" << std::endl;
	rs2::device device = devs[0];

	const char* serial = device.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER);
	std::cout << "Using camera: " << serial << std::endl;

	std::vector<rs2::sensor> sensors = device.query_sensors();
	std::cout << "Device has " << sensors.size() << " sensors:" << std::endl;

	rs400::advanced_mode adv(device);
	if (!adv.is_enabled()) {
		std::cout << "advanced mode is not enabled -> enabling it" << std::endl;

		adv.toggle_advanced_mode(true);
		std::cout << "Finished toggling advanced mode" << std::endl;
	} else {
		std::cout << "advanced mode is already enabled" << std::endl;
	}

	std::cout << "loading custom settings" << std::endl;
	adv.load_json(realsense_advanced_settings_json);

    rs2::config cfg;

	const int cDepthW = 848;
	const int cDepthH = 480;
	const int cColorW = 848;
	const int cColorH = 480;

	cfg.enable_device(serial);
	cfg.enable_stream(RS2_STREAM_COLOR, -1, cColorW, cColorH, RS2_FORMAT_RGB8, 15);
	cfg.enable_stream(RS2_STREAM_DEPTH, -1, cDepthW, cDepthH, RS2_FORMAT_Z16, 30);
	cfg.enable_stream(RS2_STREAM_ACCEL, RS2_FORMAT_MOTION_XYZ32F);
	cfg.enable_stream(RS2_STREAM_GYRO, RS2_FORMAT_MOTION_XYZ32F);

    pipe.start(cfg);

    rs2::align align_to_depth(RS2_STREAM_DEPTH);
    rs2::align align_to_color(RS2_STREAM_COLOR);

    //direction dir = direction::to_depth;
	direction dir = direction::to_color;

	int framecount = 0;

    while (true) {
        rs2::frameset frameset = pipe.wait_for_frames(3000);

        if (dir == direction::to_depth) {
            frameset = align_to_depth.process(frameset);
        } else {
            frameset = align_to_color.process(frameset);
        }

        auto depth = frameset.get_depth_frame();
        auto color = frameset.get_color_frame();
		framecount++;
		std::cout << "aligned frames: " << framecount << std::endl;
    }

    return EXIT_SUCCESS;
} catch (const rs2::error & e) 
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" <<
		e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
} catch (const std::exception & e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}

