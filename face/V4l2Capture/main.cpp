#include "opencv2/opencv.hpp"

//分类器的路径
#define source_xml_addr "/home/zwh/File/opencv-3.4.15/opencv-3.4.15/data/haarcascades_cuda/haarcascade_frontalface_alt2.xml"
#define source_pix_addr "/home/zwh/File/CheckImage/5.jpg"

int main(int argc, char const *argv[])
{
    cv::CascadeClassifier cascade;
    std::string imageName, cascadeName;
    cv::Mat image, gray;
    std::vector<cv::Rect> faces;
    cv::Scalar greenColor = cv::Scalar(0,255,0);

    cascadeName = source_xml_addr;
    imageName = source_pix_addr;
    //load image and class
    if(!cascade.load(cascadeName)){
        std::cerr << "Could not load classifier cascade" << std::endl;
    }else if(imageName.empty()){
        std::cout<<"Could not read"<<imageName<<std::endl;
    }else{
        image = cv::imread(imageName,1);
        if (image.empty())
        {
            std::cout << "Could not imread image" << std::endl;
            /* code */
        }
    }
    //convert to gray
    cv::cvtColor(image,gray,cv::COLOR_BGR2GRAY);

    // run face detector
    cascade.detectMultiScale(image,faces,1.1,2,0,cv::Size(30,30));
    printf("%d\n", faces.size());

    //output rectange
    for(size_t i = 0; i < faces.size(); ++i){
        cv::Rect r = faces[i];
        cv::rectangle(
                    image,cv::Point(cvRound(r.x),cvRound(r.y)),
                    cv::Point(cvRound(r.x+r.width-1),cvRound(r.y+r.height-1)),
                    greenColor,3,8,0);
    }

    std::vector<int> compression_params;
    if(CV_VERSION_MAJOR==3)
        compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    // if OpenCV2 comment this
    else
        compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);

    // save image
    imwrite("alpha.png", image, compression_params);

    return 0;
}
