
#include "opencv2/video/tracking.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SerialPort.h"

using namespace cv;
using namespace std;

//String for getting the output from arduino
char output[MAX_DATA_LENGTH];
char m_str[3];
int l = 3;
unsigned int suma;
int suma2;
/*Portname must contain these backslashes, and remember to replace the following com port*/
char *port_name = "\\\\.\\COM15";
string input_string;
char *c_string = new char[input_string.size() + 1];
//String for incoming data
char incomingData[MAX_DATA_LENGTH];

string face_cascade_name = "haarcascade_frontalface_alt.xml";    //Nazwa kaskady któr¹ wykorzystujemy do rozpoznania twarzy
CascadeClassifier face_cascade;                                //Utworzenie obiektu cascady twarzy
string window_name = "Hello Face !";
const string img_name = "Images.jpg";
int confirm = 0;
void detectFace( Mat img );

int main( int argc, char** argv )
{

	char c = (char) waitKey(10);
		namedWindow("Detekcja Twarzy z ARDUINO", WINDOW_AUTOSIZE);
		VideoCapture cap, cap2;
		if (argc == 1) {
			cap.open(0);
			cap2.open(0);// otwarcie pierwszej kamery
		} else {
			cap.open(argv[1]);
			cap2.open(argv[1]);
		}
		if (!cap.isOpened() || !cap2.isOpened()) { // sprawdzenie, czy siê uda³o
			cerr << "Nie uda³o siê otworzyæ strumienia." << std::endl;
			return -1;
		}

		Mat frame, frame2;
			c = 'h';
			for (;;) {
				char c = (char) waitKey(10);
				cap >> frame;
				cap2 >> frame2;
				if (frame.empty())

				break; // koniec filmu
				imshow("Detektor", frame);



				if (c == 27) { // EXIT
					cap >> frame;
												//waitKey(1000);
					//imshow( "Example2_4", frame );
					imwrite( "C:/Users/Pv/eclipse-workspace/detect/Images.jpg", frame );
					break;
				}
			}

    Mat img;                                            //Utworzenie nowej macierzy na nasz obrazek
    img = imread( img_name );                            //Wczytanie obrazka z dysku
    if ( !img.data )                                    //Sprawdzenie czy ³adowanie obrazka przebieg³o pomyslnie
    {
        cout << "Nie znaleziono pliku " <<  img_name << ".";
        return -1;
    }
    if( !face_cascade.load( face_cascade_name ) )        //£adowanie pliku ze sprawdzeniem poprawnoci
    {
        cout << "Nie znaleziono pliku " << face_cascade_name << ".";
        return -2;
    }
    //namedWindow(window_name, CV_WINDOW_AUTOSIZE);    //Utworzenie okna (nazwa okna, ustalenie rozmiaru)
    detectFace(img);

    SerialPort arduino(port_name);
     if (arduino.isConnected()) cout << "Connection ok" << endl;
     else cout << "ERROR, check port name";

     cout <<"Ilosc osób -> " << confirm;
     if(arduino.isConnected()) {
    	 if (confirm>0) { //arduino.isConnected() && (

    		 //std::copy(input_string, itoa(confirm, m_str, 2));
    		 input_string = itoa(confirm, m_str, 10);
    		 //copying the std::string to c string
    		 std::copy(input_string.begin(), input_string.end(), c_string);
    		 //Adding the delimiter
    		 c_string[input_string.size()] = '\n';
    		 //Writing string to arduino
    		 arduino.writeSerialPort(c_string, MAX_DATA_LENGTH);
    		 puts(output);
    		 delete[] c_string;
    		 /*
    		 suma = suma2;
    		 cout << " suma=" << suma;
    		 input_string = itoa(suma, m_str, 10);
    		 //copying the std::string to c string
    		 std::copy(input_string.begin(), input_string.end(), c_string);
    		 //Adding the delimiter
    		 c_string[input_string.size()] = '\n';
    		 //Writing string to arduino
    		 arduino.writeSerialPort(c_string, MAX_DATA_LENGTH);
    		 puts(output);
    		 delete[] c_string; */
     }

    	 else {
    		 input_string = "NO";
    		 //copying the std::string to c string
    		 std::copy(input_string.begin(), input_string.end(), c_string);
    		 //Adding the delimiter
    		 c_string[input_string.size()] = '\n';
    		 //Writing string to arduino
    		 arduino.writeSerialPort(c_string, MAX_DATA_LENGTH);
    		 puts(output);
    		 delete[] c_string;

    		 /*
    		 	confirm = 0;
    		 //std::copy(input_string, itoa(confirm, m_str, 2));
    		   input_string = itoa(confirm, m_str, 10);
    		   //copying the std::string to c string
    		   std::copy(input_string.begin(), input_string.end(), c_string);
    		   //Adding the delimiter
    		   c_string[input_string.size()] = '\n';
    		   //Writing string to arduino
    		   arduino.writeSerialPort(c_string, MAX_DATA_LENGTH);
    		   puts(output);
    		   delete[] c_string; */
    	 }
     }

    confirm = 0;
    waitKey(13000); //Odczekanie na wcisniêce klawisza z opóŸnieniem 0ms

    return 0;
}

void detectFace( Mat img )
{
    vector<Rect> faces;                            //Utworzenie bufora na twarze
    Mat img_gray;                                //Utworzenie bufora na obrazek w odcieniach szarosci

    cvtColor(img, img_gray,  CV_BGR2GRAY );                //Konwersja obrazu do odcieni szarosci
    equalizeHist(img_gray, img_gray);                    //Zaaplikowanie do obrazu jego historogramu, poprawa kontrastu

    face_cascade.detectMultiScale(img_gray, faces, 1.1, 3, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50) );
    for( unsigned i = 0; i < faces.size(); i++ )
    {
        Rect rect_face( faces[i] );    //Kwadrat okreslaj¹cy twarz

        rectangle(img, rect_face, Scalar( 120, 5, 86 ), 2, 2, 0  );
        confirm++;
    }

    imshow(window_name, img);                        //Pokazanie obrazka w oknmie o nazwie "Hello Face !"

}
