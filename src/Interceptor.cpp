#include "Interceptor.h"

Interceptor::Interceptor( const std::function<void()> &func ) {
    func();
}
