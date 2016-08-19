/*********************************************
* Filename: trumpet.h
* Author: PIzhou
* Mail: yuanzhou@outlook.com
* CreateTime: 2014年03月24日 星期一 19时00分36秒
*********************************************/

#include <stdio.h>

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifndef container_of
#define container_of(ptr, type, member) ({ \
            const typeof(((type *)0)->member)*__mptr = (ptr); \
            (type *)((char *)__mptr - offsetof(type, member)); })
#endif


