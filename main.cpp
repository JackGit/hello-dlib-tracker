#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv_modules.hpp>

#include <iostream>

#define FACE_DOWNSAMPLE_RATIO 1
#define SKIP_FRAMES 2
#define DLIB_PNG_SUPPORT
#define DLIB_JPEG_SUPPORT

using namespace cv;
using namespace std;
using namespace dlib;

int main(int argc, char** argv) {
    try {
        cv::VideoCapture cap(1);   //**NOTE: if you have one camera,(0) instad of (1)**
                                //i have two camera,(1) is the front one.
        if (!cap.isOpened()) {
            cerr << "Unable to connect to camera" << endl;
            return 1;
        }

        image_window win;

        // Load face detection and pose estimation models.
        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor pose_model;
        deserialize("D:/jack/qt-workspace/hello-dlib/hello-dlib/shape_predictor_68_face_landmarks.dat") >> pose_model;

        // tracker
        dlib::correlation_tracker tracker;
        bool isTracking = false;

        double fps = 0;
        // Grab and process frames until the main window is closed by the user.
        while (true) {
            auto time_start = cv::getTickCount();

            // Grab a frame
            cv::Mat temp;
            cap >> temp;

            // Turn OpenCV's Mat into something dlib can deal with.  Note that this just
            // wraps the Mat object, it doesn't copy anything.  So cimg is only valid as
            // long as temp is valid.  Also don't do anything to temp that would cause it
            // to reallocate the memory which stores the image as that will make cimg
            // contain dangling pointers.  This basically means you shouldn't modify temp
            // while using cimg.
            cv_image<bgr_pixel> cimg(temp);

            // Detect faces
            std::vector<dlib::rectangle> faces;
            dlib::rectangle targetFace;

            // Find the pose of each face.
            std::vector<full_object_detection> shapes;

            if (!isTracking) {
                faces = detector(cimg);
                if (faces.size() > 0) {
                    targetFace = faces[0];
                    tracker.start_track(
                                cimg,
                                dlib::centered_rect(
                                    dlib::point(targetFace.left() + targetFace.width() / 2, targetFace.top() + targetFace.height() / 2),
                                    targetFace.width(),
                                    targetFace.height()
                                )
                    );
                    isTracking = true;
                } else {
                    isTracking = false;
                }
            } else {
                tracker.update(cimg);
                dlib::array2d<unsigned char> subImage;
                dlib::extract_image_chip(cimg, tracker.get_position(), subImage);
                std::vector<dlib::rectangle> fs = detector(subImage);
                if (fs.size() == 0) {
                    isTracking = false;
                    printf("--------------------- losing face ---------------------\n");
                } else {
                    shapes.push_back(pose_model(cimg, tracker.get_position()));
                }
            }

            win.clear_overlay();
            win.add_overlay(tracker.get_position());
            if (shapes.size() > 0) {
                win.add_overlay(render_face_detections(shapes));
            }
            win.set_image(cimg);

            auto time_end = cv::getTickCount();
            auto time_per_frame = (time_end - time_start) / cv::getTickFrequency();
            fps = (15 * fps + (1 / time_per_frame)) / 16;
            printf("Total time: %3.5f | FPS: %3.2f\n", time_per_frame, fps);
        }
    } catch (serialization_error& e) {
        cout << "You need dlib's default face landmarking model file to run this example." << endl;
        cout << "You can get it from the following URL: " << endl;
        cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
        cout << endl << e.what() << endl;
    } catch (exception& e) {
        cout << e.what() << endl;
    }
}
