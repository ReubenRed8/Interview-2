#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>

// Shared variables
cv::Mat shared_frame;
std::mutex frame_mutex;
std::atomic<bool> running(true);
std::atomic<bool> take_snapshot(false);

// Thread: capture frames from camera
void captureThread(cv::VideoCapture& cap) {
    while (running) {
        cv::Mat frame;
        cap >> frame;

        if (frame.empty()) {
            std::cerr << "Warning: Empty frame\n";
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(frame_mutex);
            shared_frame = frame.clone();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main() {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open webcam\n";
        return -1;
    }

    int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    cv::VideoWriter writer("output.avi",
                           cv::VideoWriter::fourcc('M','J','P','G'),
                           30,
                           cv::Size(width, height));

    std::thread cap_thread(captureThread, std::ref(cap));
    int snapshot_count = 0;

    std::cout << "Press 's' to take a snapshot. Press ESC to quit.\n";

    while (running) {
        cv::Mat frame_copy;

        {
            std::lock_guard<std::mutex> lock(frame_mutex);
            if (shared_frame.empty()) continue;
            frame_copy = shared_frame.clone();
        }

        // Show the frame
        cv::imshow("Live Feed", frame_copy);

        // Write to video file
        writer.write(frame_copy);

        // Check for key input
        char key = (char)cv::waitKey(1);
        if (key == 27) {  // ESC
            running = false;
        } else if (key == 's') {
            std::string filename = "snapshot_" + std::to_string(snapshot_count++) + ".png";
            cv::imwrite(filename, frame_copy);
            std::cout << "Saved snapshot: " << filename << "\n";
        }
    }

    cap_thread.join();
    cap.release();
    writer.release();
    cv::destroyAllWindows();
    return 0;
}
