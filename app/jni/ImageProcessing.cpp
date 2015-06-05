#include "io_github_melvincabatuan_opencvpipeline_MainActivity.h"

#include <android/bitmap.h>
#include <stdlib.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

 

/*
 * Class:     io_github_melvincabatuan_opencvpipeline_MainActivity
 * Method:    decode
 * Signature: (Landroid/graphics/Bitmap;[BI)V
 */
JNIEXPORT void JNICALL Java_io_github_melvincabatuan_opencvpipeline_MainActivity_decode
  (JNIEnv * pEnv, jobject pClass, jobject pTarget, jbyteArray pSource, jint pFilter){

   AndroidBitmapInfo bitmapInfo;
   uint32_t* bitmapContent;

   if(AndroidBitmap_getInfo(pEnv, pTarget, &bitmapInfo) < 0) abort();
   if(bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) abort();
   if(AndroidBitmap_lockPixels(pEnv, pTarget, (void**)&bitmapContent) < 0) abort();

   /// 1. Access source array data... OK
   jbyte* source = (jbyte*)pEnv->GetPrimitiveArrayCritical(pSource, 0);
   if (source == NULL) abort();

   /// 2. cv::Mat for YUV420sp source
    Mat srcGray(bitmapInfo.height, bitmapInfo.width, CV_8UC1, (unsigned char *)source);



/***************************************************************************************************/
    /// Native Image Processing HERE...  
   
    Mat mbgra(bitmapInfo.height, bitmapInfo.width, CV_8UC4, (unsigned char *)bitmapContent);

    /// Otsu
    if (pFilter == 1){

       Mat otsuFrame(srcGray.size(), CV_8UC3);

       threshold( srcGray, otsuFrame, 0, 255, THRESH_BINARY | THRESH_OTSU );  // 0 = theshold w/c is ignored

       /// Display to Android
       cvtColor(otsuFrame, mbgra, CV_GRAY2BGRA);
    } 

    /// Sobel    
    else if (pFilter == 2){

        int scale = 1;
        int delta = 0;
        int ddepth = CV_16S;

        Mat sobelFrame(srcGray.size(), CV_8UC3);

        /// Generate grad_x and grad_y
        Mat grad_x, grad_y;
        Mat abs_grad_x, abs_grad_y;

  	/// Gradient X
  	Scharr( srcGray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
  	//Sobel( srcGray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
  	convertScaleAbs( grad_x, abs_grad_x );

  	/// Gradient Y
  	Scharr( srcGray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  	//Sobel( srcGray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
  	convertScaleAbs( grad_y, abs_grad_y );

  	/// Total Gradient (approximate)
  	addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, sobelFrame );

        /// Display to Android
        cvtColor(sobelFrame, mbgra, CV_GRAY2BGRA);
    }

    /// Gray
    else {
        cvtColor(srcGray, mbgra, CV_GRAY2BGRA);
    }
/***************************************************************************************************/


    /// Release Java byte buffer and unlock backing bitmap
    pEnv-> ReleasePrimitiveArrayCritical(pSource,source,0);
   if (AndroidBitmap_unlockPixels(pEnv, pTarget) < 0) abort();
}
