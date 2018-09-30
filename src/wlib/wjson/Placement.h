#ifndef __WLIB_JSON_PLACEMENT_H__
#define __WLIB_JSON_PLACEMENT_H__

extern void *operator new(decltype(sizeof(0)), void *ptr) noexcept;

#endif
