#ifndef OSALMEM_METRICS_H
#define OSALMEM_METRICS_H 

#include "type.h"

#define MAXMEMHEAP            512      //内存池大小，单位字节，此处定义分配了512

#define OSALMEM_METRICS			0		//定义有效则开启内存统计

void osal_mem_init( void );
void osal_mem_kick( void );
void *osal_mem_alloc( uint16 size );
void osal_mem_free( void *ptr );

#endif
