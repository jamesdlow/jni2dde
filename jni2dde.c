#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include "Jni2DDE.h"
#include "dde.h"

JNIEXPORT jbyteArray JNICALL Java_Jni2DDE_doDDE (JNIEnv *env, jobject object, jstring system, jstring topic, jstring item, jint timeout)
{
    jbyteArray data;

    char* str;
    unsigned int length;

    str = (char *) doDDE((*env)->GetStringUTFChars(env,system,FALSE),(*env)->GetStringUTFChars(env,topic,FALSE),(*env)->GetStringUTFChars(env,item,FALSE), &length, timeout);      
    if (str == NULL)
    {
        printf("JNI failed to get data from DDE\n");
    }
    else
    {
        data = (*env)->NewByteArray(env,length);
        (*env)->SetByteArrayRegion(env, data, 0, strlen(str), (jbyte*) str);
    }

    return data;
}

JNIEXPORT jint JNICALL Java_Jni2DDE_initDDE (JNIEnv *env, jobject object, jint timeout)
{
    return initDDE(timeout);    
}

JNIEXPORT jint JNICALL Java_Jni2DDE_connectDDE (JNIEnv *env, jobject object, jstring system, jstring topic)
{
   return connectDDE((*env)->GetStringUTFChars(env,system,FALSE),(*env)->GetStringUTFChars(env,topic,FALSE));
}

JNIEXPORT jbyteArray JNICALL Java_Jni2DDE_queryDDE (JNIEnv *env, jobject object, jstring item)
{
    char* str;
    unsigned int length;
    jbyteArray data;

    str = (char *) queryDDE((*env)->GetStringUTFChars(env,item,FALSE), &length);      
    if (str == NULL)
    {
        printf("JNI Failed to get data from DDE\n");
    }
    else
    {
        data = (*env)->NewByteArray(env,length);
        (*env)->SetByteArrayRegion(env, data, 0, strlen(str), (jbyte*) str);
    }

    return data;
}

JNIEXPORT jint JNICALL Java_Jni2DDE_disconnectDDE (JNIEnv *env, jobject object)
{
    return disconnectDDE();
}

JNIEXPORT jint JNICALL Java_Jni2DDE_uninitDDE (JNIEnv *env, jobject object)
{
    return uninitDDE();
}
