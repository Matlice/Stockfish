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

JNIEXPORT void JNICALL
Java_it_matlice_stockfish_Stockfish_nGetInstance(JNIEnv *env, jobject jthis, jboolean is_ches_960) {
    EngineInterface::getInstance(static_cast<bool>(is_ches_960),
                                 new std::map<std::string, UCI::Option, UCI::CaseInsensitiveLess>())->setPosition(
            StartFEN);
}

JNIEXPORT void JNICALL Java_it_matlice_stockfish_Stockfish_nDestroyInstance(JNIEnv *env, jobject jthis) {
    Threads.set(0);
}

JNIEXPORT jboolean JNICALL
Java_it_matlice_stockfish_Stockfish_nSetOption(JNIEnv *env, jobject jthis, jstring name, jstring value) {
    return EngineInterface::setOption(c_string(env, name), c_string(env, value));
}

JNIEXPORT void JNICALL Java_it_matlice_stockfish_Stockfish_nSetPosition
        (JNIEnv *env, jobject jthis, jstring fen) {
    EngineInterface::getInstance()->setPosition(c_string(env, fen));
}

JNIEXPORT jboolean JNICALL Java_it_matlice_stockfish_Stockfish_nMakeMove
        (JNIEnv *env, jobject jthis, jstring move) {
    return EngineInterface::getInstance()->makeMove(c_string(env, move));
}

JNIEXPORT void JNICALL Java_it_matlice_stockfish_Stockfish_nSearchBestMove
        (JNIEnv *env, jobject jthis, jint depth, jboolean ponder) {
    Search::LimitsType c_limits;
    c_limits.depth = depth;
    EngineInterface::getInstance()->bestMoveAsync(c_limits, ponder);
}

JNIEXPORT jint JNICALL Java_it_matlice_stockfish_Stockfish_nGetFoundNextMove
        (JNIEnv *env, jobject jthis) {
    return (unsigned int) *EngineInterface::getInstance()->getBestMove();
}

JNIEXPORT jstring JNICALL Java_it_matlice_stockfish_Stockfish_nGetFoundNextMoveStr
        (JNIEnv *env, jobject jthis) {
    return env->NewStringUTF(UCI::move(*EngineInterface::getInstance()->getBestMove(),
                                       EngineInterface::getInstance()->is_960()).c_str());
}

JNIEXPORT void JNICALL Java_it_matlice_stockfish_Stockfish_nFlip
        (JNIEnv *env, jobject jthis) {
    EngineInterface::getInstance()->flip();
}

JNIEXPORT void JNICALL Java_it_matlice_stockfish_Stockfish_nNewGame
        (JNIEnv *, jobject) {
    EngineInterface::getInstance()->newGame();
}

JNIEXPORT void JNICALL Java_it_matlice_stockfish_Stockfish_nDbgDisplay
        (JNIEnv *, jobject) {
    std::cout << *EngineInterface::getInstance() << std::endl;
}

JNIEXPORT jfloat JNICALL Java_it_matlice_stockfish_Stockfish_nGetScore
        (JNIEnv *, jobject, jboolean print) {
    return EngineInterface::getInstance()->getScore(print);
}

JNIEXPORT jstring JNICALL Java_it_matlice_stockfish_Stockfish_nGetFen(JNIEnv * env, jobject jthis){
    return env->NewStringUTF(EngineInterface::getInstance()->getPositionFen().c_str());
}