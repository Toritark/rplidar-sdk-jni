#include <jni.h>

#include "RPLidar.h"

extern "C" {

JNIEXPORT jlong JNICALL Java_com_toritark_kiosk_lidar_lidar_RPLidar_create(JNIEnv *env, jobject obj) {
    return reinterpret_cast<jlong>(new RPLidar());
}

JNIEXPORT void JNICALL Java_com_toritark_kiosk_lidar_lidar_RPLidar_destroy(JNIEnv *env, jobject obj, jlong ptr) {
    delete reinterpret_cast<RPLidar *>(ptr);
}

JNIEXPORT jboolean JNICALL
Java_com_toritark_kiosk_lidar_lidar_RPLidar_connect(JNIEnv *env, jobject obj, jlong ptr, jstring serialPort,
                                                    jint baudRate) {
    const char *serialPortStr = env->GetStringUTFChars(serialPort, nullptr);
    bool result = reinterpret_cast<RPLidar *>(ptr)->connect(serialPortStr, baudRate);
    env->ReleaseStringUTFChars(serialPort, serialPortStr);
    return result;
}

JNIEXPORT jboolean JNICALL Java_com_toritark_kiosk_lidar_lidar_RPLidar_disconnect(JNIEnv *env, jobject obj, jlong ptr) {
    return reinterpret_cast<RPLidar *>(ptr)->disconnect();
}

JNIEXPORT jboolean JNICALL
Java_com_toritark_kiosk_lidar_lidar_RPLidar_isConnected(JNIEnv *env, jobject obj, jlong ptr) {
    return reinterpret_cast<RPLidar *>(ptr)->isConnected();
}

JNIEXPORT jobject JNICALL
Java_com_toritark_kiosk_lidar_lidar_RPLidar_getDeviceHealthInfo(JNIEnv *env, jobject obj, jlong ptr) {
    DeviceHealth health = reinterpret_cast<RPLidar *>(ptr)->getDeviceHealthInfo();
    jclass deviceHealthClass = env->FindClass("com/toritark/kiosk/lidar/lidar/DeviceHealth");
    jmethodID constructor = env->GetMethodID(deviceHealthClass, "<init>", "(BS)V");
    return env->NewObject(deviceHealthClass, constructor, health.status, health.errorCode);
}

JNIEXPORT jboolean JNICALL Java_com_toritark_kiosk_lidar_lidar_RPLidar_isHealthy(JNIEnv *env, jobject obj, jlong ptr) {
    return reinterpret_cast<RPLidar *>(ptr)->isHealthy();
}

JNIEXPORT jboolean JNICALL
Java_com_toritark_kiosk_lidar_lidar_RPLidar_setMotorSpeed(JNIEnv *env, jobject obj, jlong ptr, jint speed) {
    return reinterpret_cast<RPLidar *>(ptr)->setMotorSpeed(speed);
}

JNIEXPORT jboolean JNICALL Java_com_toritark_kiosk_lidar_lidar_RPLidar_isScanning(JNIEnv *env, jobject obj, jlong ptr) {
    return reinterpret_cast<RPLidar *>(ptr)->isScanning();
}

JNIEXPORT jboolean JNICALL Java_com_toritark_kiosk_lidar_lidar_RPLidar_startScan(JNIEnv *env, jobject obj, jlong ptr) {
    return reinterpret_cast<RPLidar *>(ptr)->startScan();
}

JNIEXPORT jboolean JNICALL
Java_com_toritark_kiosk_lidar_lidar_RPLidar_stopScan(JNIEnv *env, jobject obj, jlong ptr, jint timeoutMs) {
    return reinterpret_cast<RPLidar *>(ptr)->stopScan(timeoutMs);
}

JNIEXPORT jboolean JNICALL
Java_com_toritark_kiosk_lidar_lidar_RPLidar_getScanData(JNIEnv *env, jobject obj, jlong ptr, jobjectArray scanDataList,
                                                        jint timeoutMs) {
    jsize length = env->GetArrayLength(scanDataList);
    auto scanData = new ScanData[length];
    bool result = reinterpret_cast<RPLidar *>(ptr)->getScanData(scanData, timeoutMs);

    jclass scanDataClass = env->FindClass("com/toritark/kiosk/lidar/lidar/ScanData");
    jfieldID distanceField = env->GetFieldID(scanDataClass, "distance", "F");
    jfieldID qualityField = env->GetFieldID(scanDataClass, "quality", "B");

    for (jsize i = 0; i < length; i++) {
        jobject scanDataObj = env->GetObjectArrayElement(scanDataList, i);
        env->SetFloatField(scanDataObj, distanceField, scanData[i].distance);
        env->SetByteField(scanDataObj, qualityField, scanData[i].quality);
        env->DeleteLocalRef(scanDataObj);
    }

    delete[] scanData;
    return result;
}

}