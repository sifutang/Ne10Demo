#include <jni.h>
#include <string>
#include <android/log.h>

#include "ne10/NE10.h"

#define  LOG_TAG    "MainActivity-Jni"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define SAMPLES 16

/**
 * An example of using the complex-to-complex FFT functions.
 */
static int complex_fft_sample_main(void) {
    ne10_fft_cpx_float32_t src[SAMPLES]; // A source array of input data
    ne10_fft_cpx_float32_t dst[SAMPLES]; // A destination array for the transformed data
    ne10_fft_cfg_float32_t cfg;          // An FFT "configuration structure"

    // Initialise Ne10, using hardware auto-detection to set library function pointers
    if (ne10_init() != NE10_OK) {
        LOGE("Failed to initialise Ne10.\n");
        return -1;
    }

    // Prepare the complex-to-complex single precision floating point FFT configuration
    // structure for inputs of length `SAMPLES`. (You need only generate this once for a
    // particular input size.)
    cfg = ne10_fft_alloc_c2c_float32(SAMPLES);

    // Generate test input values (with both real and imaginary components)
    for (int i = 0; i < SAMPLES; i++) {
        src[i].r = (ne10_float32_t)rand() / RAND_MAX * 50.0f;
        src[i].i = (ne10_float32_t)rand() / RAND_MAX * 50.0f;
    }

    // Perform the FFT (for an IFFT, the last parameter should be `1`)
    ne10_fft_c2c_1d_float32(dst, src, cfg, 0);

    // Display the results
    for (int i = 0; i < SAMPLES; i++) {
        LOGD( "IN[%2d]: %10.4f + %10.4fi\t", i, src[i].r, src[i].i);
        LOGD("OUT[%2d]: %10.4f + %10.4fi\n", i, dst[i].r, dst[i].i);
    }

    // Free the allocated configuration structure
    ne10_fft_destroy_c2c_float32(cfg);

    return 0;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_demo_ne10demo_MainActivity_stringFromJNI (
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    LOGD("calc start");
    complex_fft_sample_main();
    LOGD("calc end");

    return env->NewStringUTF(hello.c_str());
}


