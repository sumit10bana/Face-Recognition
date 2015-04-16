#include "stdafx.h"
#include "SIFT.h"
#include <cv.h>
#include <highgui.h>
#include <direct.h>
#include <conio.h>
#include "image.h";

CvRect detectFaceInImage(const IplImage *inputImg, const CvHaarClassifierCascade* cascade )
{
	const CvSize minFeatureSize = cvSize(20, 20);
	const int flags = CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_DO_ROUGH_SEARCH;	
	const float search_scale_factor = 1.1f;
	IplImage *detectImg;
	IplImage *greyImg = 0;
	CvMemStorage* storage;
	CvRect rc;
	double t;
	CvSeq* rects;
	//int i;

	storage = cvCreateMemStorage(0);
	cvClearMemStorage( storage );

	
	detectImg = (IplImage*)inputImg;	
	if (inputImg->nChannels > 1)
	{
		greyImg = cvCreateImage(cvSize(inputImg->width, inputImg->height), IPL_DEPTH_8U, 1 );
		cvCvtColor( inputImg, greyImg, CV_BGR2GRAY );
		detectImg = greyImg;	// Use the greyscale as the input.
	}

	// Detect all the faces.
	t = (double)cvGetTickCount();
	rects = cvHaarDetectObjects( detectImg, (CvHaarClassifierCascade*)cascade, storage,
				search_scale_factor, 3, flags, minFeatureSize );
	t = (double)cvGetTickCount() - t;
	printf("[Face Detection took %d ms and found %d objects]\n", cvRound( t/((double)cvGetTickFrequency()*1000.0) ), rects->total );

	// Get the first detected face (the biggest).
	if (rects->total > 0) {
        rc = *(CvRect*)cvGetSeqElem( rects, 0 );
    }
	else
		rc = cvRect(-1,-1,-1,-1);	

	//cvReleaseHaarClassifierCascade( &cascade );
	//cvReleaseImage( &detectImg );
	if (greyImg)
		cvReleaseImage( &greyImg );
	cvReleaseMemStorage( &storage );

	return rc;	// Return the biggest face found, or (-1,-1,-1,-1).
}

int main()
{
	ofstream ofs1;
	ofs1.open("attendance.txt", std::ofstream::out | std::ofstream::trunc);
	ofs1.close();
	int check = 0, ctr = 0;
//	CvRect faceRect;
//	IplImage *frame, *greyImg, *faceImg, *sizedImg, *equalizedImg, *processedFaceImg, *shownImg;
	CvHaarClassifierCascade* faceCascade;
	CvCapture* camera = 0;
	int keyPressed = 0;
	const char *faceCascadeFilename = "haarcascade_frontalface_alt.xml"; 
	faceCascade = (CvHaarClassifierCascade*)cvLoad(faceCascadeFilename, 0, 0, 0 );

	// If the camera hasn't been initialized, then open it.
	
	IplImage *frame;
	int counter = 0;
//	printf("Acessing the camera ...\n");
	if (!camera) {
			printf("Acessing the camera ...\n");
			camera = cvCaptureFromCAM( 0 );
			if (!camera) {
				printf("ERROR in getCameraFrame(): Couldn't access the camera.\n");
				exit(1);
			}
			// Try to set the camera resolution
			cvSetCaptureProperty( camera, CV_CAP_PROP_FRAME_WIDTH, 320 );
			cvSetCaptureProperty( camera, CV_CAP_PROP_FRAME_HEIGHT, 240 );
			// Wait a little, so that the camera can auto-adjust itself
			#if defined WIN32 || defined _WIN32
				Sleep(1000);	// (in milliseconds)
			#endif
			frame = cvQueryFrame( camera );	// get the first frame, to make sure the camera is initialized.
			if (frame) {
				printf("Got a camera using a resolution of %dx%d.\n", (int)cvGetCaptureProperty( camera, CV_CAP_PROP_FRAME_WIDTH), (int)cvGetCaptureProperty( camera, CV_CAP_PROP_FRAME_HEIGHT) );
			}
	}
	cvNamedWindow("Input", CV_WINDOW_AUTOSIZE);
	cvResizeWindow("Input", 320, 240);
	string name34 = "";	
	while(1) {
		SIFT *sift;
		CvRect faceRect;
		IplImage *greyImg, *faceImg, *sizedImg, *equalizedImg, *processedFaceImg, *shownImg;
		
		// Handle non-blocking keyboard input in the console.
		
		if (kbhit())
			keyPressed = getch();

		if (keyPressed == VK_ESCAPE) {	// Check if the user hit the 'Escape' key
			break;	// Stop processing input.
		}

		camera = cvCaptureFromCAM( 0 );
		frame = cvQueryFrame( camera );
		greyImg = convertImageToGreyscale(frame);
		faceRect = detectFaceInImage(greyImg, faceCascade );
		if(faceRect.width < 0 ){
			name34 = "";
		}
		switch (keyPressed) {
			case 'n':					
					if (faceRect.width > 0) {
			
					faceImg = cropImage(greyImg, faceRect);	// Get the detected face image.
					sizedImg = faceImg; 
				//	sizedImg = resizeImage(faceImg, 240, 240);
				// Give the image a standard brightness and contrast, in case it was too dark or low contrast.
					equalizedImg = cvCreateImage(cvGetSize(sizedImg), 8, 1);	// Create an empty greyscale image
					cvEqualizeHist(sizedImg, equalizedImg);
					processedFaceImg = equalizedImg;
					if (!processedFaceImg) {
						printf("ERROR in recognizeFromCam(): Don't have input image!\n");
						continue;
					}
				}
				else {
					cout << "***Please Stand Properly Infront of Camera***" << endl;
					break;
					
				}

				sift = new SIFT(processedFaceImg, 4, 3);
				sift->DoSift();				// Find keypoints
				//sift->ShowAbsSigma();	
				sift->print_all_descriptors();	// Display the sigma table
				//sift->ShowKeypoints();	
				// Show the keypoints
	
				//cvWaitKey(0);
				// Wait for a keypress
				delete sift;
				break;

			case 'p':
				if (faceRect.width > 0) {
					faceImg = cropImage(greyImg, faceRect);	// Get the detected face image.
					sizedImg = faceImg; 
				//	sizedImg = resizeImage(faceImg, 240, 240);
				// Give the image a standard brightness and contrast, in case it was too dark or low contrast.
					equalizedImg = cvCreateImage(cvGetSize(sizedImg), 8, 1);	// Create an empty greyscale image
					cvEqualizeHist(sizedImg, equalizedImg);
					processedFaceImg = equalizedImg;
					if (!processedFaceImg) {
						printf("ERROR in recognizeFromCam(): Don't have input image!\n");
						continue;
					}
				}
				else {
					cout << "***Please Stand Properly Infront of Camera***" << endl;
					break;
				}
				sift = new SIFT(processedFaceImg, 4, 3);
				sift->DoSift();				
				name34 = sift->Recognise();	
				char text1[256];
				int abc;
				for(abc = 0; abc < name34.size(); abc++) {
					text1[abc] = name34[abc];
				}
				text1[abc] = '\0';
				FILE *atten;
				atten = fopen("attendance.txt", "a");
				fprintf(atten, "%s\n", text1);
				fclose(atten);
				//cvWaitKey(0);
				//Wait for a keypress
				delete sift;
				break;

			case 'o':
					check = 1;
					ctr++;
					if (faceRect.width > 0) {
			
					faceImg = cropImage(greyImg, faceRect);	// Get the detected face image.
					// Make sure the image is the same dimensions as the training images.
					sizedImg = faceImg; 
				//	sizedImg = resizeImage(faceImg, 240, 240);
				// Give the image a standard brightness and contrast, in case it was too dark or low contrast.
					equalizedImg = cvCreateImage(cvGetSize(sizedImg), 8, 1);	// Create an empty greyscale image
					cvEqualizeHist(sizedImg, equalizedImg);
					processedFaceImg = equalizedImg;
					if (!processedFaceImg) {
						printf("ERROR in recognizeFromCam(): Don't have input image!\n");
						continue;
					}
				}
				else {
					cout << "***Please Stand Properly Infront of Camera***" << endl;
					break;
					
				}

				sift = new SIFT(processedFaceImg, 4, 3);
				sift->DoSift();				// Find keypoints
				//sift->ShowAbsSigma();	
				sift->print_all_descriptors_offline(ctr);	// Display the sigma table
				//sift->ShowKeypoints();	
				// Show the keypoints
	
				//cvWaitKey(0);
				// Wait for a keypress
				delete sift;
				break;

			case 'l':
				if (faceRect.width > 0) {
					faceImg = cropImage(greyImg, faceRect);	// Get the detected face image.
					sizedImg = faceImg; 
				//	sizedImg = resizeImage(faceImg, 240, 240);
				// Give the image a standard brightness and contrast, in case it was too dark or low contrast.
					equalizedImg = cvCreateImage(cvGetSize(sizedImg), 8, 1);	// Create an empty greyscale image
					cvEqualizeHist(sizedImg, equalizedImg);
					processedFaceImg = equalizedImg;
					if (!processedFaceImg) {
						printf("ERROR in recognizeFromCam(): Don't have input image!\n");
						continue;
					}
				}
				else {
					cout << "***Please Stand Properly Infront of Camera***" << endl;
					break;
				}
				sift = new SIFT(processedFaceImg, 4, 3);
				sift->DoSift();				
					name34 = sift->Recognise1();	
					char text2[256];
					int j = 0;
					for(j = 0; j < name34.size(); j++) {
						text2[j] = name34[j];
					}
					text2[j] = '\0';
					FILE *atten1;
					atten1 = fopen("attendance.txt", "a");
					fprintf(atten1, "%s\n", text2);
					fclose(atten1);
				//cvWaitKey(0);
				//Wait for a keypress
				delete sift;
				break;

		}

		shownImg = cvCloneImage(frame);
	
		if (faceRect.width > 0) {
			cvRectangle(shownImg, cvPoint(faceRect.x, faceRect.y), cvPoint(faceRect.x + faceRect.width-1, faceRect.y + faceRect.height-1), CV_RGB(0,255,0), 1, 8, 0);
			CvFont font;
				cvInitFont(&font,CV_FONT_HERSHEY_PLAIN, 1.0, 1.0, 0,1,CV_AA);
				CvScalar textColor = CV_RGB(0,255,255);	// light blue text
				if(name34 != ""){
					char text[256];
					int i = 0;
					for(i = 0; i < name34.size(); i++) {
						text[i] = name34[i];
					}
					text[i] = '\0';
					
					//snprintf(text, sizeof(text)-1, "Name: '%s'", personNames[nearest-1].c_str());
					cvPutText(shownImg, text, cvPoint(faceRect.x, faceRect.y + faceRect.height + 15), &font, textColor);
					
		//printf("\n");
				//	snprintf(text, sizeof(text)-1, "Confidence: %f", confidence);
				//	cvPutText(shownImg, text, cvPoint(faceRect.x, faceRect.y + faceRect.height + 30), &font, textColor);
                }else
                {
                      	char text[]="UNKNOWN PERSON";
                       	CvScalar textColor = CV_RGB(255,0,255);	// light blue text
						cvPutText(shownImg, text, cvPoint(faceRect.x, faceRect.y + faceRect.height + 15), &font, textColor);
				}
		}
		
		cvShowImage("Input", shownImg);
		keyPressed = cvWaitKey(10);
		if(keyPressed == VK_ESCAPE){
			break;
		}
		cvReleaseImage(&shownImg);
		//cvReleaseImage(&frame);
	}
	int incr = 0;
	cout << "Enter Password For Offline Testing";
	cin >> check;
	if(check == 1) {
		cout << "Doing Offline Testing" << endl;
		string possibleName;
		float val,diff,minval = 100000;
		fstream of("offline.txt",ios::in);
		string a,b,c = "", d;
		map<string, int> m;
		map<string, int> :: iterator it;
		while(of.good()) {
			string e = c;
			getline(of,c,'\n');
			getline(of, d,'\n');
			if((e != c && incr > 0) || of.eof()) {
					int tmp = 0;
					string finalName;
					for(it = m.begin(); it != m.end(); it++) {
						if((*it).second > tmp) {
							finalName = (*it).first;
							tmp = (*it).second;
						}
					}
					cout << "maximum matching wid " << finalName << " with " << tmp << " features matching " << endl;
					char text3[256];
					int j = 0;
					for(j = 0; j < finalName.size(); j++) {
						text3[j] = finalName[j];
					}
					text3[j] = '\0';
					FILE *atten2;
					atten2 = fopen("attendance.txt", "a");
					fprintf(atten2, "%s\n", text3);
					fclose(atten2);

					m.clear();
					incr = -1;
			}
			if(c == "") break;
			incr++;
			//stringstream ss;
			float minval = 100000;
			string possible;
			fstream fs("data.txt",ios::in);
			while(fs.good()) {
				stringstream ss;
				ss << d;
				float sum = 0,val,val2;
				string a;
				string b;
				getline(fs, a,'\n');
				getline(fs, b,'\n');
				if(a == "") break;
				stringstream ss1;
				ss1 << b;
				int ctrn = 0;
				while(ss1 >> val && ss >> val2) {
					ctrn++;
					diff = (fabs(val - val2));
					sum += diff*diff;
					if(sqrt(sum) > minval) break;
				}
				sum = sqrt(sum);
				if(sum < minval) {
					minval = sum;
					possible = a;
				}
			}
			fs.close();
			m[possible]++;
		//	cout << "final name " << possible << " " << minval << endl;
		}
		of.close();
	}
	ofstream ofs;
	ofs.open("offline.txt", std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	cvReleaseCapture( &camera );
	cvReleaseHaarClassifierCascade( &faceCascade );
	
	return 0;
}

