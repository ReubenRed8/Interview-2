// #include <opencv2/opencv.hpp>
// #include "C:\mingw64\opencv\build\include\opencv2\opencv.hpp"
#include"C:\OpenCV-MinGW-Build-OpenCV-4.5.5-x64\include\opencv2\opencv.hpp"
#include <thread>
#include <mutex>
#include <atomic>
#include <iostream>
#include <chrono>

std::mutex frame_mutex;
cv::Mat shared_frame;
std::atomic<bool> keep_running(true);
std::atomic<bool> take_snapshot(false);

void capture_thread_func(cv::VideoCapture& cap) {
    while (keep_running) {
        cv::Mat frame;
        if (!cap.read(frame)) {
            std::cerr << "Failed to read frame from camera.\n";
            break;
        }

        {
            std::lock_guard<std::mutex> lock(frame_mutex);
            shared_frame = frame.clone();  // copy to shared frame
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void writer_thread_func(cv::VideoWriter& writer) {
    while (keep_running) {
        cv::Mat frame_copy;
        {
            std::lock_guard<std::mutex> lock(frame_mutex);
            if (shared_frame.empty()) continue;
            frame_copy = shared_frame.clone();
        }

        writer.write(frame_copy);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void snapshot_thread_func() {
    int img_count = 0;
    while (keep_running) {
        if (take_snapshot) {
            cv::Mat frame_copy;
            {
                std::lock_guard<std::mutex> lock(frame_mutex);
                frame_copy = shared_frame.clone();
            }

            if (!frame_copy.empty()) {
                std::string filename = "snapshot_" + std::to_string(img_count++) + ".png";
                cv::imwrite(filename, frame_copy);
                std::cout << "Saved " << filename << "\n";
            }
            take_snapshot = false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main() {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Cannot open webcam\n";
        return -1;
    }

    int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    cv::VideoWriter writer("output.avi",
                           cv::VideoWriter::fourcc('M','J','P','G'),
                           30,
                           cv::Size(frame_width, frame_height));

    std::thread capture_thread(capture_thread_func, std::ref(cap));
    std::thread writer_thread(writer_thread_func, std::ref(writer));
    std::thread snapshot_thread(snapshot_thread_func);

    while (keep_running) {
        cv::Mat frame_copy;
        {
            std::lock_guard<std::mutex> lock(frame_mutex);
            if (shared_frame.empty()) continue;
            frame_copy = shared_frame.clone();
        }

        cv::imshow("Live Feed", frame_copy);
        char key = cv::waitKey(1);

        if (key == 27) { // ESC
            keep_running = false;
        } else if (key == 's') {
            take_snapshot = true;
        }
    }

    capture_thread.join();
    writer_thread.join();
    snapshot_thread.join();

    cap.release();
    writer.release();
    cv::destroyAllWindows();
    return 0;
}
