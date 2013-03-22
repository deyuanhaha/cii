
#ifndef MEM_INCLUDED
#define MEM_INCLUDED

#include "except.h"


//<exported exceptions 51>
extern const Except_T Mem_Failed;

//<exported functions 51>
// ʹ��long���ͣ������Ǳ�׼�������size_t����unsigned int/unsigned long��
// ��Ϊ�˱��⽫�������ݸ��ߺ��ò���������ɵĴ���
extern void *Mem_alloc(long nbytes, const char *file, int line); //������ڴ�δ��ʼ�������������������ȫ���᲻��й¶��Ϣ��
extern void *Mem_calloc(long count, long nbytes, const char *file, int line);
extern void Mem_free(void *ptr, const char *file, int line);

extern void *Mem_resize(void *ptr, long nbytes, const char *file, int line);

//<exported macros 51>
#define ALLOC(nbytes) \
    Mem_alloc((nbytes), __FILE__, __LINE__)
#define CALLOC(count, nbytes) \
    Mem_calloc((count), (nbytes), __FILE, __LINE__)

// FREE(ptr)���ζ�ptr������ֵ��
#define FREE(ptr) ((void)(Mem_free((ptr), __FILE__, __LINE__), \
                          (ptr) = 0))
// RESIZE(ptr, nbytes)���ζ�ptr������ֵ��
#define RESIZE(ptr, nbytes) ((ptr) = Mem_resize((ptr), (nbytes), __FILE, __LINE__))

// ʹ�þ��и�ЧӦ�ı��ʽ��Ϊ���������ʵ���ǰ�ȫ�ģ�����NEW(a[i++])����Ϊsizeof������������ֵ��
#define NEW(p) ((p) = ALLOC((long)sizeof *(p)))
#define NEW0(p) ((p) = CALLOC(1, (long)sizeof *(p)))

#endif
