#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>

using namespace std;

// Shared variables
cv::Mat shared_frame;
mutex frame_mutex;
atomic<bool> running(true);
atomic<bool> take_snapshot(false);

// Thread: capture frames from camera
void captureThread(cv::VideoCapture& cap) {
    while (running) {
        cv::Mat frame;
        cap >> frame;

        if (frame.empty()) {
            cerr << "Warning: Empty frame\n";
            continue;
        }

        {
            lock_guard<mutex> lock(frame_mutex);
            shared_frame = frame.clone();
        }

        this_thread::sleep_for(chrono::milliseconds(10));
    }
}

int main() {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Error: Could not open webcam\n";
        return -1;
    }

    int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    cv::VideoWriter writer("output.avi",
                           cv::VideoWriter::fourcc('M','J','P','G'),
                           30,
                           cv::Size(width, height));

    thread cap_thread(captureThread, ref(cap));
    int snapshot_count = 0;

    cout << "Press 's' to take a snapshot. Press ESC to quit.\n";

    while (running) {
        cv::Mat frame_copy;

        {
            lock_guard<mutex> lock(frame_mutex);
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
            string filename = "snapshot_" + to_string(snapshot_count++) + ".png";
            cv::imwrite(filename, frame_copy);
            cout << "Saved snapshot: " << filename << "\n";
        }
    }

    cap_thread.join();
    cap.release();
    writer.release();
    cv::destroyAllWindows();
    return 0;
}
