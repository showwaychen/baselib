
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
 jmethodID GetMethodID (
    JNIEnv* jni, jclass c, const char* name, const char* signature) {
  jmethodID m = jni->GetMethodID(c, name, signature);
  MYCHECK_EXCEPTION(jni, "Error during GetMethodID: ");
  return m;
}

jmethodID GetStaticMethodID (
    JNIEnv* jni, jclass c, const char* name, const char* signature) {
  jmethodID m = jni->GetStaticMethodID(c, name, signature);
  MYCHECK_EXCEPTION(jni, "Error during GetStaticMethodID: ");
  return m;
}

 jclass FindClass(JNIEnv* jni, const char* name) {
  jclass c = jni->FindClass(name);
  MYCHECK_EXCEPTION(jni, "Error during FindClass: ");
  return c;
}
 // Java references to "null" can only be distinguished as such in C++ by
// creating a local reference, so this helper wraps that logic.
bool IsNull(JNIEnv* jni, jobject obj) {
  ScopedLocalRefFrame local_ref_frame(jni);
  return jni->NewLocalRef(obj) == NULL;
}

// Given a UTF-8 encoded |native| string return a new (UTF-16) jstring.
jstring JavaStringFromStdString(JNIEnv* jni, const std::string& native) {
  jstring jstr = jni->NewStringUTF(native.c_str());
  MYCHECK_EXCEPTION(jni, "error during NewStringUTF");
  return jstr;
}

// Given a (UTF-16) jstring return a new UTF-8 native string.
std::string JavaToStdString(JNIEnv* jni, const jstring& j_string) {
  const char* chars = jni->GetStringUTFChars(j_string, NULL);
  MYCHECK_EXCEPTION(jni, "Error during GetStringUTFChars");
  std::string str(chars, jni->GetStringUTFLength(j_string));
  MYCHECK_EXCEPTION(jni, "Error during GetStringUTFLength");
  jni->ReleaseStringUTFChars(j_string, chars);
  MYCHECK_EXCEPTION(jni, "Error during ReleaseStringUTFChars");
  return str;
}

// Return the (singleton) Java Enum object corresponding to |index|;
jobject JavaEnumFromIndex(JNIEnv* jni, jclass state_class,
                          const std::string& state_class_name, int index) {
  jmethodID state_values_id = GetStaticMethodID(
      jni, state_class, "values", ("()[L" + state_class_name  + ";").c_str());
  jobjectArray state_values = static_cast<jobjectArray>(
      jni->CallStaticObjectMethod(state_class, state_values_id));
  MYCHECK_EXCEPTION(jni, "error during CallStaticObjectMethod");
  jobject ret = jni->GetObjectArrayElement(state_values, index);
  MYCHECK_EXCEPTION(jni,  "error during GetObjectArrayElement");
  return ret;
}

jobject NewGlobalRef(JNIEnv* jni, jobject o) {
  jobject ret = jni->NewGlobalRef(o);
  MYCHECK_EXCEPTION(jni,  "error during NewGlobalRef");
  return ret;
}

void DeleteGlobalRef(JNIEnv* jni, jobject o) {
  jni->DeleteGlobalRef(o);
  MYCHECK_EXCEPTION(jni, "error during DeleteGlobalRef");
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

ScopedLocalRefFrame::ScopedLocalRefFrame(JNIEnv* jni) : jni_(jni) {
  jni_->PushLocalFrame(0);
}
ScopedLocalRefFrame::~ScopedLocalRefFrame() {
  jni_->PopLocalFrame(NULL);
}