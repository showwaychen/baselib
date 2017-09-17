#ifndef JNI_HELPERS_H_
#define JNI_HELPERS_H_
#include <android/log.h>
#include <jni.h>
#include<stdio.h>
#include <map>
#include <string>


#define TAG "kuplay"

// Abort the process if |x| is false, emitting |msg| to logcat.
#define CHECK(x, msg)                                                  \
if (x) { \
} else {\
	\
	__android_log_print(ANDROID_LOG_ERROR, TAG, "%s:%d: %s", __FILE__, \
	__LINE__, msg);                                \
}
//abort();                                                           \
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

#endif /* JNI_HELPERS_H_ */
