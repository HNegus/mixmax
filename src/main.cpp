#include <opencv2/opencv.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
// #include <string>

using namespace cv;

int main(int argc, char *argv[]) {

    using namespace std::chrono;
    
    if (argc < 4 || std::stoi(argv[3]) < 0) {
        std::cout << "Usage\t: ./Max <input filename> <output filename> <N frames> [--show]" << std::endl;
        std::cout << "Example\t: ./Max input.mp4 output.mp4 30" << std::endl;
        std::cout << "Example\t: ./Max input.mp4 output.mp4 30 --show" << std::endl;
        exit(1);
    }

    std::cout << "Program init" << std::endl;
    std::cout << "GPU support: " << cv::cuda::getCudaEnabledDeviceCount() << std::endl;

    const char *input_filename = argv[1];
    const char *output_filename = argv[2];
    const int n_frames = std::stoi(argv[3]);
    const bool show = argc >= 5 ? std::strcmp("--show", argv[4]) == 0 : false;


    VideoCapture input(input_filename);
    // VideoCapture input(input_filename);
    auto width = input.get(cv::CAP_PROP_FRAME_WIDTH);
    auto height =  input.get(cv::CAP_PROP_FRAME_HEIGHT);

    std::cout << "Width, height: " << width << ", " << height << std::endl;

    int fourcc = cv::VideoWriter::fourcc('M', 'P', '4', 'v');
    VideoWriter output(output_filename, fourcc, 24, {(int)width, (int) height}, true);

    Mat avg, frame, tmp;
    input.read(frame);
    frame.convertTo(avg, CV_32FC3);
    double n = n_frames;
    double factor = 1.0/n;
    
    avg /= n;

    for (int i = 1; i < n; i++) {
        if (i % 100)
        input.read(frame);
        frame.convertTo(tmp, CV_32FC3);
        avg += tmp / n;
    }
    // frame /= n;

    high_resolution_clock::time_point prev = high_resolution_clock::now();
    for (int j = 0; ; j++) {
        // tmp = frame;
        // CV_8UC3
        // std::cout << "Here " << frame.at<float>(100, 100, 0) << std::endl;
        // std::cout << "There " << tmp.at<double>(0, 0, 0) << std::endl;
        // tmp = cv::Mat((int)height, (int)width, CV_8U, &frame);
        // avg.convertTo(tmp, CV_32FC3);
        // tmp *= factor;
        // avg.convertTo(frame, CV_8UC3);
        cv::convertScaleAbs(avg, frame);
        if (show)
            imshow("Frame", frame);
        output.write(frame);

        // input.read(frame);
        // frame.convertTo(tmp, CV_32FC3);
        // avg -= avg / n;
        input.read(frame);
        frame.convertTo(tmp, CV_32FC3);
        cv::accumulateWeighted(tmp, avg, factor);
        // avg += tmp / n;

        // input.read(tmp);
        // tmp.convertTo(frame, CV_32FC3);
        // avg += frame * factor;
        // avg -= avg * factor;


        high_resolution_clock::time_point current = high_resolution_clock::now();
        duration<double, std::milli> time_span = current - prev;
        if (j % 10 == 0) {
            std::cout << "\33[2K\r";
            std::cout << j << ": "  << time_span.count() << "ms;\t" << 10/(time_span.count()/1000) << "fps";
            std::cout.flush();
            prev = current;
        }

        if ((waitKey(1) & 0xFF) == 'q') {
            
            exit(0);
        } 
    }

}
