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
pixel calBoderMode(pixel *src, uint32_t width, uint32_t height, uint32_t stride)
{
    for (int i = 0; i < 256; i++)
        modecount[i] = 0;

    for (uint32_t i = 0; i < width; i++)
    {
        modecount[src[i]]++;
    }
    for (uint32_t i = 0; i < height; i++)
    {
        modecount[src[i*stride]]++;
    }

    //to be added
    return 0;
}