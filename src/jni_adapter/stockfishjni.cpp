//
// Created by stefano on 1/8/21.
//
#include <iostream>
#include <thread.h>
#include <unistd.h>
#include "interface/interface.h"
#include "it_matlice_stockfish_Stockfish.h"

std::string c_string(JNIEnv *env, jstring jStr) {
    if (!jStr)
        return "";

    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte *pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char *) pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}

EngineInterface * iface;

JNIEXPORT void JNICALL
Java_it_matlice_stockfish_Stockfish_nGetInstance(JNIEnv *env, jobject jthis, jboolean is_ches_960) {
    auto options_map = std::map<std::string, UCI::Option, UCI::CaseInsensitiveLess>();
    iface = EngineInterface::getInstance(static_cast<bool>(is_ches_960), &options_map);
    iface->setPosition(StartFEN);
}

JNIEXPORT void JNICALL Java_it_matlice_stockfish_Stockfish_nDestroyInstance(JNIEnv *env, jobject jthis) {
    iface = nullptr;
    Threads.set(0);
}

JNIEXPORT jboolean JNICALL
Java_it_matlice_stockfish_Stockfish_nSetOption(JNIEnv *env, jobject jthis, jstring name, jstring value) {
    return EngineInterface::setOption(c_string(env, name), c_string(env, value));
}

JNIEXPORT void JNICALL Java_it_matlice_stockfish_Stockfish_nSetPosition
        (JNIEnv * env, jobject jthis, jstring fen){
    iface->setPosition(c_string(env, fen));
}

JNIEXPORT jboolean JNICALL Java_it_matlice_stockfish_Stockfish_nMakeMove
        (JNIEnv * env, jobject jthis, jstring move){
    return iface->makeMove(c_string(env, move));
}

JNIEXPORT void JNICALL Java_it_matlice_stockfish_Stockfish_nSearchBestMove
        (JNIEnv * env, jobject jthis, jint depth, jboolean ponder){
    Search::LimitsType c_limits;
    c_limits.depth = depth;
    iface->bestMoveAsync(c_limits, ponder);
}

JNIEXPORT jint JNICALL Java_it_matlice_stockfish_Stockfish_getFoundNextMove
        (JNIEnv * env, jobject jthis){
    return (unsigned int) *iface->getBestMove();
}

JNIEXPORT jstring JNICALL Java_it_matlice_stockfish_Stockfish_getFoundNextMoveStr
        (JNIEnv * env, jobject jthis){
    return env->NewStringUTF(UCI::move(*iface->getBestMove(), iface->is_960()).c_str());
}

JNIEXPORT void JNICALL Java_it_matlice_stockfish_Stockfish_flip
        (JNIEnv * env, jobject jthis){
    iface->flip();
}

JNIEXPORT void JNICALL Java_it_matlice_stockfish_Stockfish_nNewGame
        (JNIEnv *, jobject){
    iface->newGame();
}

JNIEXPORT void JNICALL Java_it_matlice_stockfish_Stockfish_nDbgDisplay
        (JNIEnv *, jobject){
    std::cout << *iface << std::endl;
}

JNIEXPORT jfloat JNICALL Java_it_matlice_stockfish_Stockfish_nGetScore
        (JNIEnv *, jobject, jboolean print){
    return iface->getScore(print);
}