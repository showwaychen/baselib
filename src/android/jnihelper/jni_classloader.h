#ifndef JNI_CLASSLOADER_H_
#define JNI_CLASSLOADER_H_

#include<jni.h>
#include"jni_helpers.h"
#include<vector>
#include<string>
//extern const char* g_classes[];


extern ClassReferenceHolder* g_class_reference_holder;


struct SRegisterNativeData
{
	std::string m_classname;
	JNINativeMethod* m_nativem;
	int m_nnum;
	~SRegisterNativeData()
	{
		
	}
};
class CRegisterNativeM
{
protected:
	static std::vector<SRegisterNativeData*> s_nativemethods;
	

public:
	static void RegisterAllNativeMethods(JNIEnv* jni);
	CRegisterNativeM(const char* classname, JNINativeMethod* nativem, int nativemnum);

};
bool LoadClasses(JNIEnv* jni);

void ReleaseClasses(JNIEnv* jni);

void RegisterNativeMethods(JNIEnv* jni);

#endif /* JNI_CLASSLOADER_H_ */
