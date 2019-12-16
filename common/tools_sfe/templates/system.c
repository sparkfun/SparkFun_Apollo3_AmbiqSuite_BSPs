// system function calls

#include <sys/types.h>  // provides caddr_t /* for caddr_t (typedef char * caddr_t;) */s

#ifdef __cplusplus
extern "C" {
#endif

/* Excellent insight from Martin T. https://embdev.net/topic/linker-error-undefined-reference-to-_sbrk */
static unsigned char* _eheap = NULL;    // our heap management tracks the end of the heap with this var
extern unsigned char _sheap;            // Provided by linker
caddr_t _sbrk ( int incr ){
    unsigned char* prev_heap;
    unsigned char  local;

    // Initialize the heap pointer
    if (_eheap == NULL) {
        _eheap = (unsigned char*)&_sheap;
    }
    prev_heap = _eheap;

    // Check that the requested amount of memory is available (w/ margin)
    if( ((&local)-(_eheap)) >= (incr + 128) ){
        _eheap += incr;
    }else{
        // set errno to ENOMEM
        prev_heap = (unsigned char*)-1;
    }

    // Return the previous heap (or the error code)
    return (caddr_t) prev_heap;
}

#ifdef __cplusplus
}
#endif