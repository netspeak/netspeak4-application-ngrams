#include "netspeak/jni/org_netspeak_Netspeak.h"

#include <memory>

#include "aitools/util/check.hpp"

#include "netspeak/Netspeak.hpp"
#include "netspeak/RetrievalStrategy3.hpp"
#include "netspeak/jni/jni_utils.hpp"

typedef netspeak::Netspeak<netspeak::RetrievalStrategy3Tag> Netspeak;

static const char* java_runtime_exception("java/lang/RuntimeException");

/*
 * Class:     org_netspeak_Netspeak
 * Method:    native_start
 * Signature: ([B)J
 */
JNIEXPORT jlong JNICALL Java_org_netspeak_Netspeak_native_1start(
    JNIEnv* env, jclass /* jnetspeak */, jbyteArray jprops) {
  jlong nativePtr = 0;
  try {
    netspeak::generated::Properties cprops;
    aitools::check(
        cprops.ParseFromArray(env->GetByteArrayElements(jprops, nullptr),
                              env->GetArrayLength(jprops)),
        "Properties::ParseFromArray failed");
    std::unique_ptr<Netspeak> netspeak(new Netspeak());
    netspeak->initialize(netspeak::to_netspeak_config(cprops));
    nativePtr = reinterpret_cast<jlong>(netspeak.release());
  } catch (const std::exception& error) {
    env->ThrowNew(env->FindClass(java_runtime_exception), error.what());
  }
  return nativePtr;
}

/*
 * Class:     org_netspeak_Netspeak
 * Method:    native_getProperties
 * Signature: (J)[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_netspeak_Netspeak_native_1getProperties(
    JNIEnv* env, jclass /* jnetspeak */, jlong jnativePtr) {
  jbyteArray jproperties = nullptr;
  try {
    aitools::check(jnativePtr != 0, __func__, "JNI: jnativePtr is 0");
    const auto properties = netspeak::to_properties(
        reinterpret_cast<Netspeak*>(jnativePtr)->properties());
    const size_t byte_size = properties.ByteSize();
    std::unique_ptr<char[]> buffer(new char[byte_size]);
    properties.SerializeToArray(buffer.get(), byte_size);
    jproperties = env->NewByteArray(byte_size);
    aitools::check(jproperties != nullptr, "JNIEnv::NewByteArray failed",
                   byte_size);
    env->SetByteArrayRegion(jproperties, 0, byte_size,
                            reinterpret_cast<jbyte*>(buffer.get()));
  } catch (const std::exception& error) {
    env->ThrowNew(env->FindClass(java_runtime_exception), error.what());
  }
  return jproperties;
}

/*
 * Class:     org_netspeak_Netspeak
 * Method:    native_searchRaw
 * Signature: (J[B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_netspeak_Netspeak_native_1searchRaw(
    JNIEnv* env, jclass /* jnetspeak */, jlong jnativePtr,
    jbyteArray jrequest) {
  jbyteArray jresponse = nullptr;
  try {
    aitools::check(jnativePtr != 0, __func__, "JNI: jnativePtr is 0");
    netspeak::generated::Request request;
    aitools::check(
        request.ParseFromArray(env->GetByteArrayElements(jrequest, nullptr),
                               env->GetArrayLength(jrequest)),
        "Request::ParseFromArray failed");
    std::shared_ptr<netspeak::generated::RawResponse> response(
        reinterpret_cast<Netspeak*>(jnativePtr)->search_raw(request));
    const size_t byte_size = response->ByteSize();
    std::unique_ptr<char[]> buffer(new char[byte_size]);
    response->SerializeToArray(buffer.get(), byte_size);
    jresponse = env->NewByteArray(byte_size);
    aitools::check(jresponse != nullptr, "JNIEnv::NewByteArray failed",
                   byte_size);
    env->SetByteArrayRegion(jresponse, 0, byte_size,
                            reinterpret_cast<jbyte*>(buffer.get()));
  } catch (const std::exception& error) {
    env->ThrowNew(env->FindClass(java_runtime_exception), error.what());
  }
  return jresponse;
}

/*
 * Class:     org_netspeak_Netspeak
 * Method:    native_search
 * Signature: (J[B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_netspeak_Netspeak_native_1search(
    JNIEnv* env, jclass /* jnetspeak */, jlong jnativePtr,
    jbyteArray jrequest) {
  jbyteArray jresponse = nullptr;
  try {
    aitools::check(jnativePtr != 0, __func__, "JNI: jnativePtr is 0");
    netspeak::generated::Request request;
    aitools::check(
        request.ParseFromArray(env->GetByteArrayElements(jrequest, nullptr),
                               env->GetArrayLength(jrequest)),
        "Request::ParseFromArray failed");
    std::shared_ptr<netspeak::generated::Response> response(
        reinterpret_cast<Netspeak*>(jnativePtr)->search(request));
    const size_t byte_size = response->ByteSize();
    std::unique_ptr<char[]> buffer(new char[byte_size]);
    response->SerializeToArray(buffer.get(), byte_size);
    jresponse = env->NewByteArray(byte_size);
    aitools::check(jresponse != nullptr, "JNIEnv::NewByteArray failed",
                   byte_size);
    env->SetByteArrayRegion(jresponse, 0, byte_size,
                            reinterpret_cast<jbyte*>(buffer.get()));
  } catch (const std::exception& error) {
    env->ThrowNew(env->FindClass(java_runtime_exception), error.what());
  }
  return jresponse;
}

/*
 * Class:     org_netspeak_Netspeak
 * Method:    native_stop
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_netspeak_Netspeak_native_1stop(
    JNIEnv* /* env */, jclass /* jnetspeak */, jlong jnativePtr) {
  delete reinterpret_cast<Netspeak*>(jnativePtr);
}
