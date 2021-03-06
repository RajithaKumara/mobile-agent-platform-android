#include <jni.h>
#include <string>
#include <cstdlib>
#include <node.h>

extern "C" JNIEXPORT jstring JNICALL
Java_mobile_agentplatform_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {

#if defined(__arm__)
    #if defined(__ARM_ARCH_7A__)
    #if defined(__ARM_NEON__)
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a/NEON (hard-float)"
      #else
        #define ABI "armeabi-v7a/NEON"
      #endif
    #else
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a (hard-float)"
      #else
        #define ABI "armeabi-v7a"
      #endif
    #endif
  #else
   #define ABI "armeabi"
  #endif
#elif defined(__i386__)
#define ABI "x86"
#elif defined(__x86_64__)
#define ABI "x86_64"
#elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
#define ABI "mips64"
#elif defined(__mips__)
#define ABI "mips"
#elif defined(__aarch64__)
#define ABI "arm64-v8a"
#else
#define ABI "unknown"
#endif

    return env->NewStringUTF("Hello from JNI !  Compiled with ABI " ABI ".");
}

extern "C" JNIEXPORT jint JNICALL
Java_mobile_agentplatform_MainActivity_startNodeWithArguments(
        JNIEnv *env,
        jobject /* this */,
        jobjectArray arguments) {

    jsize argument_count = env->GetArrayLength(arguments);

    int c_arguments_size = 0;
    for (int i = 0; i < argument_count ; i++) {
        c_arguments_size += strlen(env->GetStringUTFChars((jstring)env->GetObjectArrayElement(arguments, i), 0));
        c_arguments_size++; // for '\0'
    }

    char* args_buffer = (char*) calloc(c_arguments_size, sizeof(char));

    char* argv[argument_count];

    char* current_args_position = args_buffer;

    for (int i = 0; i < argument_count ; i++)
    {
        const char* current_argument = env->GetStringUTFChars((jstring)env->GetObjectArrayElement(arguments, i), 0);

        strncpy(current_args_position, current_argument, strlen(current_argument));

        argv[i] = current_args_position;

        current_args_position += strlen(current_args_position) + 1;
    }

    int node_result = node::Start(argument_count, argv);
    free(args_buffer);

    return jint(node_result);
}
