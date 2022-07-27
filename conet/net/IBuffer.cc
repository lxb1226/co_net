#include "IBuffer.h"

namespace conet {
namespace net{

const size_t IBuffer::kCheapPrepend = 8;
const size_t IBuffer::kInitialSize = 1024;
const char IBuffer::kCRLF[] = "\r\n";

}
}