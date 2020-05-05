/* DO NOT EDIT THIS FILE - it is machine generated */
#include "netspeak/jni/jni.h"
/* Header for class org_netspeak_Netspeak */

#ifndef _Included_org_netspeak_Netspeak
#define _Included_org_netspeak_Netspeak
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_netspeak_Netspeak
 * Method:    native_start
 * Signature: ([B)J
 */
JNIEXPORT jlong JNICALL
Java_org_netspeak_Netspeak_native_1start(JNIEnv *, jclass, jbyteArray);

/*
 * Class:     org_netspeak_Netspeak
 * Method:    native_getProperties
 * Signature: (J)[B
 */
JNIEXPORT jbyteArray JNICALL
Java_org_netspeak_Netspeak_native_1getProperties(JNIEnv *, jclass, jlong);

/*
 * Class:     org_netspeak_Netspeak
 * Method:    native_searchRaw
 * Signature: (J[B)[B
 */
JNIEXPORT jbyteArray JNICALL
Java_org_netspeak_Netspeak_native_1searchRaw(JNIEnv *, jclass, jlong,
                                             jbyteArray);

/*
 * Class:     org_netspeak_Netspeak
 * Method:    native_search
 * Signature: (J[B)[B
 */
JNIEXPORT jbyteArray JNICALL
Java_org_netspeak_Netspeak_native_1search(JNIEnv *, jclass, jlong, jbyteArray);

/*
 * Class:     org_netspeak_Netspeak
 * Method:    native_stop
 * Signature: (J)V
 */
JNIEXPORT void JNICALL
Java_org_netspeak_Netspeak_native_1stop(JNIEnv *, jclass, jlong);

#ifdef __cplusplus
}
#endif
#endif
