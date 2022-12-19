// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料

#ifdef PLATFROM_WINDOWS
// Windows 头文件:
#include <windows.h>
#include <string>
#else
#include <string.h>
#endif

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t)&((TYPE*)0)->MEMBER)
#endif

#include <functional>
#include <iostream>
#include <string>
#include <functional>
#include <memory>
#include <vector>
#include <map>
#include <set>
#include <list>
#include "assert.h"
// TODO:  在此处引用程序需要的其他头文件
