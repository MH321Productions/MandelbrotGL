#include <jni.h>
#include <GLES3/gl3.h>
#include <android/log.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

extern "C"
JNIEXPORT jboolean JNICALL
Java_io_github_mh321productions_mandelbrot_MandelbrotRenderer_saveImage(JNIEnv *env, jobject thiz, jint width, jint height, jstring path) {
    glReadBuffer(GL_BACK);

    unsigned char* data = new unsigned char[width * height * 3];
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

    stbi_flip_vertically_on_write(true);

    jboolean isCopy;
    const char* cpath = env->GetStringUTFChars(path, &isCopy);

    __android_log_print(ANDROID_LOG_DEBUG, "Save image", "Saving to %s", cpath);

    int result = stbi_write_png(cpath, width, height, 3, data, width * 3);

    delete[] data;

    return result;
}