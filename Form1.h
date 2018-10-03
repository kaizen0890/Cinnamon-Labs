#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <fstream> // lam viec voi file text
#include <sstream>
#include <stdlib.h>
#include <Windows.h>
#include <msclr/marshal_cppstd.h>


#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include "SLIC.h"




/////////// Functions //////////////

static float mean_1D_array(float arr[], int n)
{
	float mean_array;
	for (int i = 0; i < n; i++)
	{
		mean_array += arr[i];
	}
	return (float) (mean_array/n);
}

static float mean_color_label(Mat_<float> image)
{
	float mean_label;
	for (int i = 0;i<image.cols;i++)
	{
		for (int j =0; j<image.rows;j++)
		{
			int count;
			float temp;
			if(image.at<float>(cv::Point(i, j)) != 0)
			{
				count++;
				temp += image.at<float>(cv::Point(i, j));		
			}
			mean_label = (float) (temp/ count);		
		}
	}

	return mean_label;
}


static Mat get_ROI_of_label(Mat image)
{
	vector<vector<cv::Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(image, contours , hierarchy, CV_RETR_TREE,  CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );



	Mat region_of_label;

	for (int i = 0; i < contours.size(); i++)
	{		
		Rect r = boundingRect(contours[i]);
		region_of_label = (image(r));       /////// extract processing label 
	}
	return region_of_label;
}





using namespace std;
using namespace cv;

using namespace System::Runtime::InteropServices;

using namespace msclr::interop;
using namespace System::Data;
using namespace System::Drawing;







namespace SLICSVMASVD {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog;
	protected: 
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::TextBox^  txt_test;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->txt_test = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// openFileDialog
			// 
			this->openFileDialog->FileName = L"openFileDialog";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(56, 47);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(109, 67);
			this->button1->TabIndex = 0;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// txt_test
			// 
			this->txt_test->Location = System::Drawing::Point(365, 53);
			this->txt_test->Name = L"txt_test";
			this->txt_test->Size = System::Drawing::Size(89, 20);
			this->txt_test->TabIndex = 1;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(753, 357);
			this->Controls->Add(this->txt_test);
			this->Controls->Add(this->button1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

				 // ----------------OPEN FILE IMAGE---------------
				 openFileDialog->Filter = "Image Files|*.bmp; *.jpg; *.png|All Files (*.*)|*.*||";  openFileDialog->Title = "Select a image file"; 
				 openFileDialog->InitialDirectory = "";
				 openFileDialog-> Multiselect = "True"; 

				 int number_of_files;
				 array<System::String^>^ filename_src; // array of filenames
				 if ( openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK )  
				 {
					 filename_src = openFileDialog->FileNames;
					 number_of_files = openFileDialog->FileNames->Length;

				 }
				 else return;
				 System::String^ OpenFileName;
				 System::String^ step_str;
				 int step = 0;
				 while (step <number_of_files)

				 {	
					 OpenFileName = filename_src[step];	
					 OpenFileName = OpenFileName->Replace("\\","\\\\");
					 char* imgname = (char*)Marshal::StringToHGlobalAnsi(OpenFileName).ToPointer();  // convert to kieu char lay ten filecho opencv	
					 Mat img = cv::imread(imgname);

					 Mat gray = img.clone();
					 cvtColor(img, gray , CV_BGR2GRAY);	 


					 // Ket thuc mo file


					 Mat img_input = img.clone();
					


					 // Ket thuc mo file




					 ///// Create number of superpixels (number of label)
					 UINT numSuperpixel = 20;

					 SLIC slic_img;
					 slic_img.GenerateSuperpixels(gray, numSuperpixel );

					 ////// Get image with contour after superpixels
					 Mat slic_output = slic_img.GetImgWithContours(cv::Scalar(0, 0, 255)); 

					 

					 


					 // Get the labels
					 Mat1i labelImg(gray.rows, gray.cols, slic_img.GetLabel());


					 


					 

					 // Get the actual number of labels
					 // may be less that n_of_superpixels

					 double max_dlabel;
					 minMaxLoc(labelImg, NULL, &max_dlabel);
					 int max_label = int(max_dlabel);

					

					 
					 for (int label = 0; label < 1 ; ++label)
					 {
						 Mat label_image;
						 label_image = labelImg == label; //////// mask of region label

						

							 



						 //////// superpixel label on original image demension ///////

						 Mat superpixel_in_img;
						 img_input.copyTo(superpixel_in_img, label_image);

						 
						

						 //////// crop each superpixel label with label dimension  ///////// 

						 


						 Mat region_of_label;
						 Rect r;

						 std::vector< std::vector<cv::Point> > contours; 
						 std::vector<cv::Vec4i>hierarcy;  
						 findContours(label_image,contours,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);

						
						 for( int i = 0; i< contours.size(); i++ )
						 {					 
							 drawContours(label_image, contours, i, Scalar (112,255,255), 1 ,8);
							 r = boundingRect(contours[i]);			 
						 }

						 region_of_label = superpixel_in_img(r);
						 Mat region_of_label2;
						 cvtColor(region_of_label, region_of_label2, CV_BGR2GRAY);

						 Mat mask_image( superpixel_in_img.size(), CV_8U, Scalar(0));
						 drawContours(mask_image, contours, -1, Scalar(255), CV_FILLED);
						


						 
						 region_of_label2.copyTo(superpixel_in_img, mask_image);


						 imshow("", superpixel_in_img);

						 
						

						 
						 

						 


						
						 

						 


						



						 	

						 

							 
						
						 
						
						 
						
						 













						 ////////////////// Mask for each label ////////////////

						 //Mat superpixel_mask;   //// create mask of label region
						 //superpixel_mask = region_of_label == 0;  //// take label to mask image

						

						 /////////////////// After processing, copy processed labels to original image 
						 //region_of_label.copyTo(img_input, superpixel_mask);


						 
						 




					 }













					 waitKey(0);
					 return;

				 }

				 step++;










			 }
	};
}

