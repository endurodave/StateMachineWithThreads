#ifndef _USER_MSG_H
#define _USER_MSG_H

#include "winuser.h"

#define WM_USER_BEGIN				(WM_USER)
#define WM_EXIT_THREAD				(WM_USER + 1)
#define WM_DISPATCH_CALLBACK		(WM_USER + 2)
#define WM_USER_TIMER				(WM_USER + 3)
#define WM_USER_END					(WM_USER + 100)		

#endif