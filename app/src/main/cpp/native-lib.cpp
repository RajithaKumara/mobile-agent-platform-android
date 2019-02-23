#include <jni.h>
#include <string>
#include <cstdlib>
#include <node.h>

extern "C" JNIEXPORT jstring JNICALL
Java_mobile_agentplatform_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
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
