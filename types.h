#ifndef __TYPES_H
#define __TYPES_H

typedef unsigned char 		u8;
typedef signed char   		s8;

typedef unsigned short int 	u16;
typedef signed short int 	s16;

typedef unsigned long int 	u32;
typedef signed long int 	s32;

typedef unsigned long long 	u64;
typedef signed long long 	s64;

// typedef unsigned __int128 	u128;
// typedef signed __int128 	s128;

typedef struct __u128
{
	float w, z, y, x;
	__u128(){ }
	__u128 (float w1, float z1, float y1, float x1) {
		w = w1; z = z1; y = y1; x = x1;
	}
	

} u128;

#endif