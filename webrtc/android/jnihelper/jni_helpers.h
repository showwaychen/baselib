#ifndef JNI_HELPERS_H_
#define JNI_HELPERS_H_
#include <android/log.h>
#include <jni.h>
#include<stdio.h>
#include <map>
#include <string>


#define TAG "jninative"

// Abort the process if |x| is false, emitting |msg| to logcat.
#define CHECK(x, msg)                                                  \
if (x) { \
} else {\
	\
	__android_log_print(ANDROID_LOG_ERROR, TAG, "%s:%d: %s", __FILE__, \
	__LINE__, msg);                                \
}
//abort();                                                           
// Abort the process if |jni| has a Java exception pending, emitting |msg| to
// logcat.
#define MYCHECK_EXCEPTION(jni, msg) \
if (0) { \
} else {	\
	if (jni->ExceptionCheck()) {		\
		jni->ExceptionDescribe();   \
		jni->ExceptionClear();      \
		CHECK(0, msg);              \
	}                             \
}

#define ARRAYSIZE(instance)                                     \
	static_cast<int>(sizeof(instance) / sizeof(instance[0]))


jlong jlongFromPointer(void* ptr);

JNIEnv* GetEnv(JavaVM* jvm);
void SetJavaVM(JavaVM *javaVM);
JavaVM * GetJavaVM();
jmethodID GetMethodID(
    JNIEnv* jni, jclass c, const char* name, const char* signature);

jmethodID GetStaticMethodID(
    JNIEnv* jni, jclass c, const char* name, const char* signature);
jclass FindClass(JNIEnv* jni, const char* name);
// Java references to "null" can only be distinguished as such in C++ by
// creating a local reference, so this helper wraps that logic.
bool IsNull(JNIEnv* jni, jobject obj);

// Given a UTF-8 encoded |native| string return a new (UTF-16) jstring.
jstring JavaStringFromStdString(JNIEnv* jni, const std::string& native);

// Given a (UTF-16) jstring return a new UTF-8 native string.
std::string JavaToStdString(JNIEnv* jni, const jstring& j_string);

// Return the (singleton) Java Enum object corresponding to |index|;
jobject JavaEnumFromIndex(JNIEnv* jni, jclass state_class,
                          const std::string& state_class_name, int index);

jobject NewGlobalRef(JNIEnv* jni, jobject o);

void DeleteGlobalRef(JNIEnv* jni, jobject o);


class ClassReferenceHolder {
public:
	ClassReferenceHolder(JNIEnv* jni, char** classes, int size);
	~ClassReferenceHolder();

	void FreeReferences(JNIEnv* jni);

	jclass GetClass(const std::string& name);


	void LoadClass(JNIEnv* jni, const std::string& name);
private:
	std::map<std::string, jclass> classes_;
};

class AttachThreadScoped {
public:
	explicit AttachThreadScoped(JavaVM* jvm);
	~AttachThreadScoped();
	JNIEnv* env();

private:
	bool attached_;
	JavaVM* jvm_;
	JNIEnv* env_;
};

// Scoped holder for global Java refs.
template<class T>  // T is jclass, jobject, jintArray, etc.
class ScopedGlobalRef {
 public:
  ScopedGlobalRef(JNIEnv* jni, T obj)
      : jni_(jni), obj_(static_cast<T>(NewGlobalRef(jni, obj))) {}
  ~ScopedGlobalRef() {
    DeleteGlobalRef(jni_, obj_);
  }
  T operator*() const {
    return obj_;
  }
 private:
  JNIEnv* jni_;
  T obj_;
};

class ScopedLocalRefFrame {
 public:
  explicit ScopedLocalRefFrame(JNIEnv* jni);
  ~ScopedLocalRefFrame();

 private:
  JNIEnv* jni_;
};
#endif /* JNI_HELPERS_H_ */
