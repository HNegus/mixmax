#include <opencv2/opencv.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
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
    std::cout << "GPU support: " << cuda::getCudaEnabledDeviceCount() << std::endl;

    const char *input_filename = argv[1];
    const char *output_filename = argv[2];
    const int n_frames = std::stoi(argv[3]);
    const bool show = argc >= 5 ? std::strcmp("--show", argv[4]) == 0 : false;


    VideoCapture input(input_filename);
    // VideoCapture input(input_filename);
    auto width = input.get(cv::CAP_PROP_FRAME_WIDTH);
    auto height =  input.get(cv::CAP_PROP_FRAME_HEIGHT);

    std::cout << "Width, height: " << width << ", " << height << std::endl;

    int fourcc = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
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

    int stepsize = 1;

    high_resolution_clock::time_point prev = high_resolution_clock::now();
    for (int j = 0; input.read(frame); j++) {

        // input.read(frame);

        frame.convertTo(tmp, CV_32FC3);
        cv::accumulateWeighted(tmp, avg, factor);
        cv::convertScaleAbs(avg, frame);

        if (show)
            imshow("Frame", frame);
        
        if ((waitKey(1) & 0xFF) == 'q') {
            exit(0);
        } 
        output.write(frame);
        high_resolution_clock::time_point current = high_resolution_clock::now();
        duration<double, std::milli> time_span = current - prev;
        if (j % stepsize == 0) {
            // std::cout << "\33[2K\r";
            std::cout << j << ": "  << time_span.count()/stepsize << "ms;\t" << stepsize/(time_span.count()/1000) << "fps" << std::endl;
            std::cout.flush();
            prev = current;
        }

        // try {
        // } catch (cv::Exception e)  {

        //     std::cout << std::endl;

        //     std::cout << "factor: " << factor << std::endl;

        //     std::cout << std::endl;
    
        //     std::cout << "tmp channels: " << tmp.channels() << std::endl;
        //     std::cout << "tmp cols: " << tmp.cols << std::endl;
        //     std::cout << "tmp channels: " << tmp.rows << std::endl;
        //     std::cout << "tmp size: " << tmp.size() << std::endl;
        //     std::cout << "tmp total: " << tmp.total() << std::endl;
        //     std::cout << "tmp elemsize: " << tmp.elemSize() << std::endl;
        //     std::cout << "tmp type: " << tmp.type() << std::endl;

        //     std::cout << std::endl;

        //     std::cout << "frame channels: " << frame.channels() << std::endl;
        //     std::cout << "frame cols: " << frame.cols << std::endl;
        //     std::cout << "frame channels: " << frame.rows << std::endl;
        //     std::cout << "frame size: " << frame.size() << std::endl;
        //     std::cout << "frame total: " << frame.total() << std::endl;
        //     std::cout << "frame elemsize: " << frame.elemSize() << std::endl;
        //     std::cout << "frame type: " << frame.type() << std::endl;

        //     std::cout << std::endl;

        //     std::cout << "avg channels: " << avg.channels() << std::endl;
        //     std::cout << "avg cols: " << avg.cols << std::endl;
        //     std::cout << "avg channels: " << avg.rows << std::endl;
        //     std::cout << "avg size: " << avg.size() << std::endl;
        //     std::cout << "avg total: " << avg.total() << std::endl;
        //     std::cout << "avg elemsize: " << avg.elemSize() << std::endl;
        //     std::cout << "avg type: " << avg.type() << std::endl;
        //     exit(0);
        // }

        // }
        
    }

}
