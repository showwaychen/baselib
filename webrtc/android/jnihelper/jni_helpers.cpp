
#include "jni_helpers.h"
#include <limits>

static JavaVM *sl_jvm = NULL;
//using icu::UnicodeString;

//jmethodID GetMethodID(JNIEnv* jni, jclass c, const std::string& name,
//	const char* signature) {
//	jmethodID m = jni->GetMethodID(c, name.c_str(), signature);
//	MYCHECK_EXCEPTION(jni, "error during GetMethodID");
//	return m;
//}

jlong jlongFromPointer(void* ptr) {
	CHECK(sizeof(intptr_t) <= sizeof(jlong), "Time to rethink the use of jlongs");
	// Going through intptr_t to be obvious about the definedness of the
	// conversion from pointer to integral type.  intptr_t to jlong is a standard
	// widening by the COMPILE_ASSERT above.
	jlong ret = reinterpret_cast<intptr_t>(ptr);
	CHECK(reinterpret_cast<void*>(ret) == ptr,
		"jlong does not convert back to pointer");
	return ret;
}
JNIEnv* GetEnv(JavaVM* jvm) {
	void* env = NULL;
	jint status = jvm->GetEnv(&env, JNI_VERSION_1_6);
	CHECK(((env != NULL) && (status == JNI_OK)) ||
		((env == NULL) && (status == JNI_EDETACHED)), "Unexpected GetEnv return: %d");
		//<< "Unexpected GetEnv return: " << status << ":" << env;
	return reinterpret_cast<JNIEnv*>(env);
}
// Given a (UTF-16) jstring return a new UTF-8 native string.
//std::string JavaToStdString(JNIEnv* jni, const jstring& j_string) {
//	const char* chars = jni->GetStringUTFChars(j_string, nullptr);
//	MYCHECK_EXCEPTION(jni, "Error during GetStringUTFChars");
//	std::string str(chars, jni->GetStringUTFLength(j_string));
//	MYCHECK_EXCEPTION(jni, "Error during GetStringUTFLength");
//	jni->ReleaseStringUTFChars(j_string, chars);
//	MYCHECK_EXCEPTION(jni, "Error during ReleaseStringUTFChars");
//	return str;
//}
 void SetJavaVM(JavaVM *javaVM)
{
	 sl_jvm = javaVM;
}
 JavaVM * GetJavaVM()
 {
	 return sl_jvm;
 }
ClassReferenceHolder::ClassReferenceHolder(JNIEnv* jni, char** classes,
	int size) {
	for (int i = 0; i < size; ++i) {
		LoadClass(jni, classes[i]);
	}
}
ClassReferenceHolder::~ClassReferenceHolder() {
	CHECK(classes_.empty(), "Must call FreeReferences() before dtor!");
}

void ClassReferenceHolder::FreeReferences(JNIEnv* jni) {
	for (std::map<std::string, jclass>::const_iterator it = classes_.begin();
		it != classes_.end(); ++it) {
		jni->DeleteGlobalRef(it->second);
	}
	classes_.clear();
}

jclass ClassReferenceHolder::GetClass(const std::string& name) {
	std::map<std::string, jclass>::iterator it = classes_.find(name);
	CHECK(it != classes_.end(), "Could not find class");
	return it->second;
}

void ClassReferenceHolder::LoadClass(JNIEnv* jni, const std::string& name) {
	jclass localRef = jni->FindClass(name.c_str());
	MYCHECK_EXCEPTION(jni, "Could not load class");
	CHECK(localRef, name.c_str());
	jclass globalRef = reinterpret_cast<jclass>(jni->NewGlobalRef(localRef));
	MYCHECK_EXCEPTION(jni, "error during NewGlobalRef");
	CHECK(globalRef, name.c_str());
	bool inserted = classes_.insert(std::make_pair(name, globalRef)).second;
	CHECK(inserted, "Duplicate class name");
}




AttachThreadScoped::AttachThreadScoped(JavaVM* jvm)
: attached_(false), jvm_(jvm), env_(NULL) {
	env_ = GetEnv(jvm);
	if (!env_) {
		// Adding debug log here so we can track down potential leaks and figure
		// out why we sometimes see "Native thread exiting without having called
		// DetachCurrentThread" in logcat outputs.
		//ALOGD("Attaching thread to JVM%s", GetThreadInfo().c_str());
		jint res = jvm->AttachCurrentThread(&env_, NULL);
		attached_ = (res == JNI_OK);
		CHECK(attached_, "AttachCurrentThread failed: ");
	}
}

AttachThreadScoped::~AttachThreadScoped() {
	if (attached_) {
		//ALOGD("Detaching thread from JVM%s", GetThreadInfo().c_str());
		jint res = jvm_->DetachCurrentThread();
		CHECK(res == JNI_OK, "DetachCurrentThread failed: ");
		CHECK(!GetEnv(jvm_), "");
	}
}

JNIEnv* AttachThreadScoped::env() { return env_; }
