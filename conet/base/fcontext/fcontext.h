#ifndef FCONTEXT_H
#define FCONTEXT_H

#include <cstdint>
namespace conet {
    typedef void* fcontext_t;

    extern "C" intptr_t jump_fcontext( fcontext_t * ofc, fcontext_t nfc, intptr_t vp, bool preserve_fpu = false);
    extern "C" fcontext_t make_fcontext( void * sp, std::size_t size, void (* fn)( intptr_t) );
} // namespace conet



#endif // FCONTEXT_H