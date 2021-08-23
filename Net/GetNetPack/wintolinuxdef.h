#ifndef WINTOLINUXDEF_H
#define WINTOLINUXDEF_H

#include <cstdint>
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include <fstream>
#include <thread>

#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <map>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>

using namespace  std;

#define IN
#define OUT
#define WINAPI
#define APIENTRY    WINAPI
#define FAR
#define far
#define near

typedef int (FAR WINAPI *FARPROC)();
typedef char *NPSTR, *LPSTR, *PSTR;
typedef const char *LPCSTR, *PCSTR;
typedef char    CHAR;

typedef int                 BOOL;
typedef void* HMODULE;
typedef void* HINSTANCE;
typedef void* HANDLE;
typedef void VOID;

typedef unsigned long ULONG_PTR, *PULONG_PTR;
typedef unsigned long ULONG;
typedef long LONG;
typedef  int64_t LONGLONG;
typedef unsigned int       DWORD;
typedef void            *LPVOID;
typedef void    *PVOID;
typedef int64_t __time64_t;
typedef ULONG *PULONG;
typedef unsigned int        UINT;
typedef unsigned long long ULONGLONG;

#define UNREFERENCED_PARAMETER(P)          (void)P
#define DBG_UNREFERENCED_PARAMETER(P)      (void)P
#define DBG_UNREFERENCED_LOCAL_VARIABLE(V) (V)



#ifndef FALSE
#define FALSE               0
#endif
#ifndef TRUE
#define TRUE                1
#endif
typedef long HRESULT;
#define S_OK                                   ((HRESULT)0L)
#define S_FALSE                                ((HRESULT)1L)
#define ERROR_SUCCESS                    0L


typedef unsigned char       BYTE;
typedef BYTE far            *LPBYTE;
typedef BYTE                *PBYTE;



#define RTL_NUMBER_OF_V1(A) (sizeof(A)/sizeof((A)[0]))
#define _ARRAYSIZE(A)   RTL_NUMBER_OF_V1(A)
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#define MAX_PATH          260

#define _ASSERT assert

typedef unsigned short      WORD;

typedef unsigned short USHORT;
typedef unsigned char  UCHAR;

#define INFINITE 0xffffffff

#define _strcmp(X1,X2) strcasecmp(X1,X2)


typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;

#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))

#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))



#define VRV_EXPORT __attribute__ ((visibility ("default")))
#define VRV_HIDE   __attribute__((visibility ("hidden")))
#define EXTERNC extern "C"

#define ZeroMemory(p,s) memset(p,0,s)
//#ifndef max
//#define max(a,b)            (((a) > (b)) ? (a) : (b))
//#endif

#ifndef MIN
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define SOCKET int
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)

#define STRING_CHECK_NULL(x) x==NULL?"":x;

//注:此处代码仅框架使用,用于次wchar转换为char
#ifdef USE_IN_CEMS
typedef char *NWPSTR, *LPWSTR, *PWSTR;
typedef const char *LPCWSTR, *PCWSTR;
typedef char CHAR;

typedef LPWSTR PTSTR, LPTSTR;
typedef LPCWSTR PCTSTR, LPCTSTR;

typedef char TCHAR, *PTCHAR;

#ifdef UNICODE
#define __T(x)      L ## x
#else
#define __T(x)      x
#endif
#define _T(x)       __T(x)
#define _TEXT(x)    __T(x)
#endif

#endif // WINTOLINUXDEF_H

