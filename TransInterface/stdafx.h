// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����

#ifdef PLATFROM_WINDOWS
// Windows ͷ�ļ�:
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
// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
