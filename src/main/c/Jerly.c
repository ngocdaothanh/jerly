#include <string.h>
#include <stdlib.h>

#include "Jerly.h"
#include "cherly.h"
#include "common.h"

static void destroy(char *key, int keylen, void *value, int vallen) {
    dprintf("destroy key: %s, vallen: %d\n", key, vallen);
    free(key);
    free(value);
}

static cherly_t * get_cherly(JNIEnv *env, jobject obj) {
    jclass   cls    = (*env)->GetObjectClass(env, obj);
    jfieldID fid    = (*env)->GetFieldID(env, cls, "cherly", "J");
    jlong    cherly = (*env)->GetLongField(env, obj, fid);
    return (cherly_t *) cherly;
}

JNIEXPORT void JNICALL Java_Jerly_init(JNIEnv *env, jobject obj, jlong maxSize) {
    cherly_t *cherly = (cherly_t *) malloc(sizeof(cherly_t));
    cherly_init(cherly, maxSize);

    jclass   cls = (*env)->GetObjectClass(env, obj);
    jfieldID fid = (*env)->GetFieldID(env, cls, "cherly", "J");
    (*env)->SetLongField(env, obj, fid, (jlong) cherly);
}

JNIEXPORT void JNICALL Java_Jerly_destroy(JNIEnv *env, jobject obj) {
    cherly_destroy(get_cherly(env, obj));
}

JNIEXPORT void JNICALL Java_Jerly_put(JNIEnv *env, jobject obj, jstring jkey, jbyteArray jbytes, jint ttl) {
    jboolean iscopy;
    const char *ckey1 = (*env)->GetStringUTFChars(env, jkey, &iscopy);    
    
    // Copy bytes from JVM, because JVM may release the memory any time

    int   keylen = strlen(ckey1);
    char *ckey2  = (char *) malloc(keylen);
    strcpy(ckey2, ckey1);

    int   byteslen = (*env)->GetArrayLength(env, (jarray) jbytes);
    int   vallen   = sizeof(int) + byteslen;
    char *value    = (char *) malloc(vallen);
    memcpy(value, &byteslen, sizeof(int));
    (*env)->GetByteArrayRegion(env, jbytes, 0, byteslen, value + sizeof(int));
    
  	cherly_put(get_cherly(env, obj), ckey2, keylen, value, vallen, ttl, &destroy);
  	(*env)->ReleaseStringUTFChars(env, jkey, ckey1);
}

JNIEXPORT jbyteArray JNICALL Java_Jerly_get(JNIEnv *env, jobject obj, jstring jkey) {
    jboolean iscopy;
    const char *ckey = (*env)->GetStringUTFChars(env, jkey, &iscopy);

    char *value = (char *) cherly_get(get_cherly(env, obj), ckey, strlen(ckey));
    if (value == NULL) {
        (*env)->ReleaseStringUTFChars(env, jkey, ckey);
        return NULL;
    }

    int byteslen;
    memcpy(&byteslen, value, sizeof(int));

    jbyteArray jbytes = (*env)->NewByteArray(env, byteslen);
    (*env)->SetByteArrayRegion(env, jbytes, 0, byteslen, value + sizeof(int));

	  (*env)->ReleaseStringUTFChars(env, jkey, ckey);
    return jbytes;
}

JNIEXPORT void JNICALL Java_Jerly_remove(JNIEnv *env, jobject obj, jstring jkey) {
    jboolean iscopy;
    const char *ckey = (*env)->GetStringUTFChars(env, jkey, &iscopy);

    cherly_remove(get_cherly(env, obj), ckey, strlen(ckey));    
    (*env)->ReleaseStringUTFChars(env, jkey, ckey);
}
