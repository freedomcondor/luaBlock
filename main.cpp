#include <stdlib.h>
#include <stdio.h>

#include <opencv2/opencv.hpp>
/*
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
*/

#include <apriltag/apriltag.h>
#include <apriltag/tag36h11.h>

using namespace cv;

// conver n into something like "00001"
char *numberToString5(int n, char str[]);

// opencv draw
//int setColor(uchar pix[],int R,int G, int B);
int setColor(cv::Vec<unsigned char, 3> &pix,int R,int G, int B);
int drawCross(Mat img, int x, int y, const char color[]);

int main(int n_arg_count, char* ppch_args[])
{
	int i,j,k;
	int x_temp,y_temp;
	char c;

	////////// apriltag initial ////
				// these might be needed
				//		m_cCameraMatrix(c_camera_matrix),
				//		m_cDistortionParameters(c_distortion_parameters) {
	apriltag_detector* m_psTagDetector;
	apriltag_family* m_psTagFamily;
	m_psTagDetector = apriltag_detector_create();
		// not sure what does these means yet, just copied from Michael
	m_psTagDetector->quad_decimate = 1.0f;
	m_psTagDetector->quad_sigma = 0.0f;
	m_psTagDetector->nthreads = 1;
	m_psTagDetector->debug = 0;
	m_psTagDetector->refine_edges = 1;
	m_psTagDetector->refine_decode = 0;
	m_psTagDetector->refine_pose = 0;
	/* create the tag family */
	m_psTagFamily = tag36h11_create();
	m_psTagFamily->black_border = 1;
	apriltag_detector_add_family(m_psTagDetector, m_psTagFamily);

	////////// loop image initail /////////
	char fileName[30];
	char fileNameBase[20] = "../data/output_";
	//char fileNameBase[20] = "data/output_";
	char fileExt[10] = ".png";
	char fileNumber[5];

	Mat imageRGB,image;
	namedWindow("output",WINDOW_NORMAL);
	moveWindow("output",100,100);
	resizeWindow("output",1000,700);

	/////////// main loop //////////////////////////////
	for (i = 0; i < 197; i++)
	{
		///////// Open Image  //////////////////////////
		// conver i into "00001"
		numberToString5(i,fileNumber);
		// making filename
		strcpy(fileName,fileNameBase);
		strcat(fileName,fileNumber);
		strcat(fileName,fileExt);
		printf("%d, %s : ",i, fileName);
		// open image
		imageRGB = imread( fileName, 1 );
		//printf("channels,%d ",image.channels());
		printf("row and col,%d %d ",imageRGB.rows,imageRGB.cols);
		if ( !imageRGB.data ) { printf("Can't open image %s \n",fileName); return -1; }
		cvtColor(imageRGB,image,CV_BGR2GRAY);

		/* convert image to apriltags format */
		image_u8_t* ptImageY = image_u8_create(image.cols, image.rows);
		// remember to destroy it
		for (unsigned int un_row = 0; un_row < ptImageY->height; un_row++) 
		{
			memcpy(&ptImageY->buf[un_row * ptImageY->stride],
						image.row(un_row).data,
						ptImageY->width);
		}
		//now ptImageY is the image_u8_t type image

		zarray_t* psDetections = apriltag_detector_detect(m_psTagDetector, ptImageY);
		image_u8_destroy(ptImageY);
		//now psDetections is an array of detected tags

		/////////  Trick Start  ///////////////////////
		// go through all the tags
		printf("tags: %d\n: ",zarray_size(psDetections));
		for (j = 0; j < zarray_size(psDetections); j++)
		{
			apriltag_detection_t *psDetection;
			zarray_get(psDetections, j, &psDetection);

									//								 x
															////////////////////
															//				  //
															//				  //
									//					y	//				  //
															//				  //
															//				  //
															////////////////////

			////  output and draw
			printf("\tid = %d\n",psDetection->id);
			////////// draw center /////////
			x_temp = psDetection->c[1];
			y_temp = psDetection->c[0];
			//printf("%d %d\n",x_temp, y_temp);
			drawCross(imageRGB,x_temp,y_temp,"green");

			////////// draw corners /////////
			for (k = 0; k < 4; k++)
			{
				x_temp = psDetection->p[k][1];
				y_temp = psDetection->p[k][0];
				drawCross(imageRGB,x_temp,y_temp,"blue");
			}
		}

		imshow("output", imageRGB);
		c = waitKey(30);
		//c = waitKey(0);
	}


	return 0;
}

///////  OpenCV draw  ////////////////////////////////////////////////

int drawCross(Mat img, int x, int y, const char color[])
{
	int R,G,B;
	if (strcmp(color,"blue") == 0)
	{ R = 0; G = 0; B = 255; }
	else if (strcmp(color,"green") == 0)
	{ R = 0; G = 255; B = 0; }
	else if (strcmp(color,"red") == 0)
	{ R = 255; G = 0; B = 0; }
	Mat_<Vec3b> _image = img;
	setColor(_image(x,y),R,G,B);
	if (x != 0) 			setColor(_image(x-1,y),R,G,B);
	if (x != img.cols-1) 	setColor(_image(x+1,y),R,G,B);
	if (y != 0) 			setColor(_image(x,y-1),R,G,B);
	if (y != img.rows-1) 	setColor(_image(x,y+1),R,G,B);
	img = _image;
	return 0;
}

int setColor(cv::Vec<unsigned char, 3> &pix,int R,int G, int B)
{
	pix[0] = B;
	pix[1] = G;
	pix[2] = R;
	return 0;
}

////////  Number to String  /////////////////////////////////////////////////////////
char* numberToString(int n, int k, char str[])
{
	int i;
	int tens;
												// take 5 as an example
	tens = 1;
	for (i = 0; i < k; i++)
		tens *= 10;
												// tens = 100000
	n = n % tens;
	tens /= 10;

	for (i = 0; i < k; i++)						//5 times
	{
		str[i] = n / tens + '0' - 0;
		n = n % tens;
		tens /= 10;
	}

	str[k] = '\0';

	return str;
}

char* numberToString5(int n, char str[])
{
	numberToString(n,5,str);
	return 0;
}
