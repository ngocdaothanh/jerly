/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class Jerly */

#ifndef _Included_Jerly
#define _Included_Jerly
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     Jerly
 * Method:    init
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_Jerly_init
  (JNIEnv *, jobject, jlong);

/*
 * Class:     Jerly
 * Method:    destroy
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Jerly_destroy
  (JNIEnv *, jobject);

/*
 * Class:     Jerly
 * Method:    put
 * Signature: (Ljava/lang/String;[BI)V
 */
JNIEXPORT void JNICALL Java_Jerly_put
  (JNIEnv *, jobject, jstring, jbyteArray, jint);

/*
 * Class:     Jerly
 * Method:    get
 * Signature: (Ljava/lang/String;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_Jerly_get
  (JNIEnv *, jobject, jstring);

/*
 * Class:     Jerly
 * Method:    remove
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_Jerly_remove
  (JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif
