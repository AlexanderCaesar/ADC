/**
* @file
* @brief
* @author   caiyangang
* <pre><b>email: </b>caiyangang@pku.edu.cn</pre>
* <pre><b>All rights reserved.</b></pre>
* @date 2020/04/13
* @see
*/

#include <time.h>
#include <stdio.h>
#include <string>

#ifdef WIN32
#include "windows.h"
#include <sys/types.h>
#include <sys/timeb.h>
#define snprintf _snprintf
#else
#include <sys/time.h>
#include <unistd.h>
#endif


#include "common.h"
#include "picyuv.h"

using namespace std;

static string os_version = "";


int64_t time_mdate()
{
#if _WIN32
  struct timeb tb;
  ftime(&tb);
  return ((int64_t)tb.time * 1000 + (int64_t)tb.millitm) * 1000;
#else
  struct timeval tv_date;
  gettimeofday(&tv_date, NULL);
  return (int64_t)tv_date.tv_sec * 1000000 + (int64_t)tv_date.tv_usec;
#endif
}


#ifdef WIN32

#pragma warning(disable:4996)

void BuildLogFilePath(const char *path)
{
  char creatpath[512];
  snprintf(creatpath, 256, "md %s", path);
  system(creatpath);
}
#else
void BuildLogFilePath(const char *path)
{

}
#endif


#ifdef WIN32
static bool GetVersionExt(LPOSVERSIONINFOW lpVersionInformation)
{
  HMODULE hNtDll = GetModuleHandleW(L"NTDLL"); // get ntdll.dll handle
  typedef NTSTATUS(NTAPI*tRtlGetVersion)(PRTL_OSVERSIONINFOW povi); // RtlGetVersion
  tRtlGetVersion pRtlGetVersion = NULL;
  if (hNtDll)
  {
    pRtlGetVersion = (tRtlGetVersion)GetProcAddress(hNtDll, "RtlGetVersion"); // RtlGetVersion address
  }
  if (pRtlGetVersion)
  {
    return pRtlGetVersion((PRTL_OSVERSIONINFOW)lpVersionInformation) >= 0; // call RtlGetVersion
  }
  return false;
}

const char* GetOsVersion()
{
  OSVERSIONINFOEXW ver = { sizeof ver };
  GetVersionExt((LPOSVERSIONINFOW)&ver);

  if (ver.dwMajorVersion == 5)
  {
    switch (ver.dwMinorVersion)
    {
    case 0:
      os_version = "Windows 2000";
      break;
    case 1:
      os_version = "Windows XP";
      break;
    case 2:
      os_version = "Windows Server 2003";
      break;
    default:
      os_version = "Unknown System";
    }
  }
  else if (ver.dwMajorVersion == 6)
  {
    switch (ver.dwMinorVersion)
    {
    case 0:
      os_version = "Windows Vista";
      break;
    case 1:
      os_version = "Windows 7";
      break;
    case 2:
      os_version = "Windows 8";
      break;
    case 3:
      os_version = "Windows 8.1";
      break;
    default:
      os_version = "Unknown System";
    }
  }
  else if (ver.dwMajorVersion == 10)
  {
    switch (ver.dwMinorVersion)
    {
    case 0:
      os_version = "Windows 10";
      break;
    default:
      os_version = "Unknown System";
    }
  }
  else
  {
    os_version = "Unknown System";
  }
  return os_version.c_str();
}
#elif ANDROID
static string GetOsVersion()
{
  if (os_version.length() <= 0){
    os_version = "android";
  }
  return os_version.c_str();
}
#else
static string GetOsVersion()
{
  if (os_version.length() <= 0){
    os_version = "ios";
  }
  return os_version.c_str();
}
#endif


static uint32_t modecount[256];
/*The mode is that value that is repeated most often in the data set.*/
int calBoderMode(pixel *src, uint32_t X, uint32_t Y, uint32_t width, uint32_t height, uint32_t stride)
{
    uint32_t all_ref_pixels = 0;

    if (!X&&!Y)
    {
        all_ref_pixels = 0;
        return 128;
    }

    for (int i = 0; i < 256; i++)
        modecount[i] = 0;

    if (X > 0) // left
    {
        pixel* left = src - 1;

        for (uint32_t i = 0; i < height; i++)
        {
            modecount[left[i*stride]]++;
        }

        all_ref_pixels += height;
    }

    if (Y > 0) //top
    {
        pixel* top = src - stride;

        for (uint32_t i = 0; i < width; i++)
        {
            modecount[top[i]]++;
        }

        all_ref_pixels += width;
    }

    if (X > 0 && Y > 0) //top-left
    {
        pixel* top_left = src - stride - 1;

        modecount[top_left[0]]++;

        all_ref_pixels += 1;
    }

    uint32_t max_cout = 0;
    int      mode = 0;

    for (int i = 0; i < 256; i++)
    {
        if (modecount[i] > max_cout)
        {
            max_cout = modecount[i];
            mode = i;
        }
    }

    if (max_cout > all_ref_pixels / 2)
    {
        return mode;
    }
    else
    {
        return -1;
    }
}

/*cal directions.*/
int calDirection(pixel *rec, int mode, uint32_t width, uint32_t height, uint32_t stride, uint32_t& direction)
{
    int left = (int)rec[ (height/2)*stride-1]; //middle-bottom
    int top = (int)rec[width / 2 - stride];    //middle-right

    if (abs(left - mode) < abs(top - mode))
    {
        direction = 0;
        return left;
    }
    else
    {
        direction = 1;
        return top;
    }
}

/*get direction Ref.*/
int getDirectionRef(pixel *rec, uint32_t width, uint32_t height, uint32_t stride, uint32_t direction)
{
    int left = (int)rec[(height / 2)*stride - 1]; //middle-bottom
    int top = (int)rec[width / 2 - stride];    //middle-right

    if (direction)
    {
        return top;
    }
    else
    {
        return left;
    }
}

int calCUMode(pixel *src, uint32_t width, uint32_t height, uint32_t stride, int& min, int& max, uint32_t& mode_cout)
{
    for (int i = 0; i < 256; i++)
        modecount[i] = 0;


    for (uint32_t h = 0; h < height; h++)
    {
        for (uint32_t w = 0; w < width; w++)
        {
            modecount[src[w]]++;
        }
        src += stride;
    }
    int      mode = 0;
    mode_cout = 0;

    for (int i = 0; i < 256; i++)
    {
        if (modecount[i])
        {
            min = i;
            break;
        }
    }

    for (int i = 255; i >= 0; i--)
    {
        if (modecount[i])
        {
            max = i;
            break;
        }
    }

    for (int i = 0; i < 256; i++)
    {
        if (modecount[i] > mode_cout)
        {
            mode_cout = modecount[i];
            mode = i;
        }
    }
    return mode;
}

void find_psnr(Frame* curFrame, double psnr[3], int bit_depth)
{
    double sum[3], mse[3];
    pixel *o, *r;
    int i, j, k;
    int peak_val = (bit_depth == 8) ? 255 : 1023;
    int width[3];
    int height[3];

    PicYuv*     rec = curFrame->m_reconPic;
    PicYuv*     enc = curFrame->m_fencPic;

    width[0] = enc->m_picWidth;
    width[1] = enc->m_picWidth >> 1;
    width[2] = enc->m_picWidth >> 1;
    height[0] = enc->m_picHeight;
    height[1] = enc->m_picHeight >> 1;
    height[2] = enc->m_picHeight >> 1;
    for (i = 0; i < 3; i++) {
        o = (pixel *)enc->m_picOrg[i];
        r = (pixel *)rec->m_picOrg[i];
        sum[i] = 0;
        for (j = 0; j < height[i]; j++) {
            for (k = 0; k < width[i]; k++) {
                sum[i] += (o[k] - r[k]) * (o[k] - r[k]);
            }
            o = (pixel *)((unsigned char *)o + enc->m_stride[i]);
            r = (pixel *)((unsigned char *)r + rec->m_stride[i]);
        }
        mse[i] = sum[i] / (width[i] * height[i]);
        // psnr[i] = (mse[i] == 0.0) ? 100. : fabs(10 * log10(((255 * 255 * 16) / mse[i])));
        psnr[i] = (mse[i] == 0.0) ? 100. : fabs(10 * log10(((peak_val * peak_val) / mse[i])));
    }
}

