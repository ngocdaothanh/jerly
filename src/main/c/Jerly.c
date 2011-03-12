#include <string.h>
#include <stdlib.h>

#include "Jerly.h"
#include "cherly.h"

typedef struct _cache_value_t {
    jbyte *bytes;
    int   size;
} cache_value_t;

static cherly_t cherly;

void destroy(char *key, int keylen, void *value, int vallen) {
    cache_value_t *cache_value = (cache_value_t *) value;
    free(cache_value->bytes);
    free(cache_value);
}

JNIEXPORT void JNICALL Java_Jerly_init(JNIEnv *env, jobject jobj, jlong maxSize) {
    cherly_init(&cherly, maxSize);
}

JNIEXPORT void JNICALL Java_Jerly_put(JNIEnv *env, jobject jobj, jstring jkey, jbyteArray jbytes) {
    jboolean iscopy;
    const char *ckey = (*env)->GetStringUTFChars(env, jkey, &iscopy);
    
    // Copy bytes from JVM, because JVM may release the memory any time
    jsize size    = (*env)->GetArrayLength(env, (jarray) jbytes);
    jbyte *cbytes = (jbyte *) malloc(size);
    (*env)->GetByteArrayRegion(env, jbytes, 0, size, cbytes);
    
    cache_value_t cache_value;
    cache_value.bytes = cbytes;
    cache_value.size  = size;
  	cherly_put(&cherly, (char *)ckey, strlen(ckey), &cache_value, sizeof(cache_value_t), &destroy);
  	
  	(*env)->ReleaseStringUTFChars(env, jkey, ckey);
}

JNIEXPORT jbyteArray JNICALL Java_Jerly_get(JNIEnv *env, jobject jobj, jstring jkey) {
    jboolean iscopy;
    const char *ckey = (*env)->GetStringUTFChars(env, jkey, &iscopy);

    cache_value_t *cache_value = (cache_value_t *) cherly_get(&cherly, (char *) ckey, strlen(ckey));
    jbyteArray     jbytes      = (*env)->NewByteArray(env, cache_value->size);
    (*env)->SetByteArrayRegion(env, jbytes, 0, cache_value->size, cache_value->bytes);

	  (*env)->ReleaseStringUTFChars(env, jkey, ckey);
    return jbytes;
}

JNIEXPORT void JNICALL Java_Jerly_remove(JNIEnv *env, jobject jobj, jstring jkey) {
    jboolean iscopy;
    const char *ckey = (*env)->GetStringUTFChars(env, jkey, &iscopy);

    cherly_remove(&cherly, ckey, strlen(ckey));
    
    (*env)->ReleaseStringUTFChars(env, jkey, ckey);
}

JNIEXPORT void JNICALL Java_Jerly_destroy(JNIEnv *env, jobject jobj) {
    cherly_destroy(&cherly);
}
