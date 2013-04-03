
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "assert.h"
#inlcude "seq.h"
#include "array.h"
#include "arrayrep.h"
#include "mem.h"

#define T Seq_T

struct T {
    struct Array_T array;
    int length; // ������ֵ����Ŀ
    int head; // ����������Ϊ0��ֵ����������������Ϊhead��Ԫ�ش���������������������ֵ��Ҳ����������ġ�������Ԫ���С�
};

//<static functions 128>
static void expand(T seq)
{
    int n = seq->array.length;
    Array_resize(&seq->array, 2*n);
    if (seq->head > 0) { // expand�账�������������ͻ����������Ρ�ԭ������ε���ЩԪ�أ���head֮�󣩱����ƶ�����չ֮�������ĩ��
        //<slide tail down 129>
        void **old = &((void **)seq->array.array)[seq->head];
        memcpy(old+n, old, (n - seq->head)*sizeof(void *));
        seq->head += n;
    }
}

//<functions 126>
T Seq_new(int hint)
{
    T seq;

    assert(hint >= 0);
    NEW0(seq);
    if (hint == 0) {
        hint = 16;
    }
    ArrayRep_init(&seq->array, hint, sizeof(void *), ALLOC(hint * sizeof(void *)));
    return seq;
}

void Seq_seq(void *x, ...)
{
    va_list ap;
    T seq = Seq_new(0);

    va_start(ap, x);
    for (; x; x = va_arg(ap, void *)) {
        Seq_addhi(seq, x);
    }
    va_end(ap);
    return seq;
}

void Seq_free(T *seq)
{
    assert(seq && *seq);
    // ���Ｔ��Array_T�ṹ������Seq_T�ṹ�еĵ�һ���ֶΣ�����Seq_new��NEW0���ص�ָ���ָ��һ��Seq_Tʵ����ͬʱҲָ��һ��Array_Tʵ����
    assert((void *)*seq == (void *)&(*seq)->array);
    Array_free((Array_T *)seq); // Array_free(T *array)��Ҫ����Ϊ��FREE((*array)->array);FREE(*array);
}

int Seq_length(T seq)
{
    assert(seq);
    return seq->length;
}

void *Seq_get(T seq, int i)
{
    assert(seq);
    assert(i >= 0 && i < seq->length);
    return /*<seq[i] 127>*/((void **)seq->array.array)[(seq->head + i)%seq->array.length];
}

void *Seq_put(T seq, int i, void *x)
{
    void *prev;

    assert(seq);
    assert(i >= 0 && i < seq->length);
    prev = /*<seq[i] 127>*/((void **)seq->array.array)[(seq->head + i)%seq->array.length];
    /*<seq[i] 127>*/((void **)seq->array.array)[(seq->head + i)%seq->array.length]
     = x;
    return prev;
}

void *Seq_remhi(T seq)
{
    int i;

    assert(seq);
    assert(seq->length > 0);
    i = --seq->length;
    return /*<seq[i] 127>*/((void **)seq->array.array)[(seq->head + i)%seq->array.length];
}

void *Seq_remlo(T seq)
{
    int i = 0;
    void *x;
    assert(seq);
    assert(seq->length > 0);
    x = /*<seq[i] 127>*/((void **)seq->array.array)[(seq->head + i)%seq->array.length];
    seq->head = (seq->head + 1)%seq->array.length;
    --seq->length;
    return x;
}

void *Seq_addhi(T seq, void *x)
{
    int i;

    assert(seq);
    if (seq->length == seq->array.length) {
        expand(seq);
    }
    i = seq->length++;
    return /*<seq[i] 127>  = x;*/ /*<seq[i] 127>*/((void **)seq->array.array)[(seq->head + i)%seq->array.length] = x;
}

void *Seq_addlo(T seq, void *x)
{
    int i = 0;

    assert(seq);
    if (seq->length == seq->array.length) {
        expand(seq);
    }
    if (--seq->head < 0) {
        seq->head = seq->array.length - 1;
    }
    seq->length++;
    return /*<seq[i] 127>  = x;*/ /*<seq[i] 127>*/((void **)seq->array.array)[(seq->head + i)%seq->array.length] = x;
}
