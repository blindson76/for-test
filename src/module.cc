#include <napi.h>
#include "Wrapped.h"
using namespace Napi;
// Initialize native add-on
Napi::Object Init (Napi::Env env, Napi::Object exports) {
    Wrapped::Init(env, exports);

    return exports;
}

// Register and initialize native add-on
NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)