#pragma once
#include <jni.h>
#define JNI_NEWINSTANCE_DECLAR static jlong JNICALL newinstance(JNIEnv *env, jobject thiz);
#define JNI_NEWINSTANCE_IMPLEMENT(clsname)                                                         \
    jlong JNICALL clsname::newinstance(JNIEnv *env, jobject thiz)                                  \
    {                                                                                              \
        std::shared_ptr<clsname> *instance = new std::shared_ptr<clsname>(new clsname(env, thiz)); \
        return jlongFromPointer((void *)instance);                                                 \
    }
#define JNI_GETINSTANCE_DECLAR(clsname) static std::shared_ptr<clsname> GetInst(JNIEnv *jni, jobject j_object);
#define JNI_GETINSTANCE_IMPLEMENT(clsname)                                          \
    std::shared_ptr<clsname> clsname::GetInst(JNIEnv *jni, jobject j_object)        \
    {                                                                               \
        jclass j_class = jni->GetObjectClass(j_object);                             \
        jfieldID nativeobject_id = jni->GetFieldID(j_class, "m_NativeObject", "J"); \
        MYCHECK_EXCEPTION(jni, "GetInst failed");                                   \
        jlong j_p = jni->GetLongField(j_object, nativeobject_id);                   \
        MYCHECK_EXCEPTION(jni, "GetInst failed");                                   \
        if (j_p == 0)                                                               \
        {                                                                           \
            return nullptr;                                                         \
        }                                                                           \
        return *reinterpret_cast<std::shared_ptr<clsname> *>(j_p);                  \
    }
#define JNI_GETINSTANCE_STATEMENT(clsname) std::shared_ptr<clsname> instance = clsname::GetInst(env, thiz);
#define JNI_REGISTERNATIVE_DECLAR static CRegisterNativeM s_registernm;
#define JNI_NATIVEMETHOD_BEGIN static JNINativeMethod ls_nm[] =
#define JNI_NATIVEMETHOD_END ;
#define JNI_REGISTERNATIVE_IMPLEMENT(clsname, javaclsname) CRegisterNativeM clsname::s_registernm(javaclsname, ls_nm, ARRAYSIZE(ls_nm));
