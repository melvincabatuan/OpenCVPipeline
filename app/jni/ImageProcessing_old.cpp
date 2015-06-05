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
   // Mat src(bitmapInfo.height + bitmapInfo.height/2, bitmapInfo.width, CV_8UC1, (unsigned char *)source);  
    Mat srcGray(bitmapInfo.height, bitmapInfo.width, CV_8UC1, (unsigned char *)source);

    /// 3. Allocate new image buffer
    //Mat mbgr(bitmapInfo.height, bitmapInfo.width, CV_8UC3);    
    //cvtColor(src, mbgr, CV_YUV420sp2BGR);

    /// Image Processing HERE...  
   
    Mat mbgra(bitmapInfo.height, bitmapInfo.width, CV_8UC4, (unsigned char *)bitmapContent);
    // cvtColor(mbgr, mbgra, CV_BGR2BGRA);
    cvtColor(srcGray, mbgra, CV_GRAY2BGRA);

    /// Release Java byte buffer and unlock backing bitmap
    pEnv-> ReleasePrimitiveArrayCritical(pSource,source,0);
   if (AndroidBitmap_unlockPixels(pEnv, pTarget) < 0) abort();
}
