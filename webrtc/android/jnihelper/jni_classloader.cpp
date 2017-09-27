#include"jni_classloader.h"

#include <android/log.h>
#define CLASSLOAD_TAG              "classload"
#define     _LOGI(...)          __android_log_print(ANDROID_LOG_INFO, CLASSLOAD_TAG, __VA_ARGS__)
#define     _LOGD(...)          __android_log_print(ANDROID_LOG_DEBUG, CLASSLOAD_TAG, __VA_ARGS__)
#define     _LOGW(...)          __android_log_print(ANDROID_LOG_WARN, CLASSLOAD_TAG, __VA_ARGS__)
#define     _LOGE(...)          __android_log_print(ANDROID_LOG_ERROR, CLASSLOAD_TAG, __VA_ARGS__)

//
//char* g_classes[] = {
//};

ClassReferenceHolder* g_class_reference_holder = NULL;

bool LoadClasses(JNIEnv* jni)
{
	_LOGI("load all classes start");
	if (jni == NULL)
	{
		return false;
	}
	g_class_reference_holder = new ClassReferenceHolder(jni, NULL,0);
	return true;
}


void ReleaseClasses(JNIEnv* jni)
{
	if (jni == NULL)
	{
		return;
	}
	if (g_class_reference_holder != NULL)
	{
		g_class_reference_holder->FreeReferences(jni);
		delete g_class_reference_holder;
	}
}


void RegisterNativeMethods(JNIEnv* jni)
{
	_LOGI("RegisterNativeMethods start");
	CRegisterNativeM::RegisterAllNativeMethods(jni);
	_LOGI("RegisterNativeMethods over");
}


std::vector<SRegisterNativeData*> CRegisterNativeM::s_nativemethods;
static SRegisterNativeData* ls_srnd[100] = {0};
static int ls_srndindex = 0;

void CRegisterNativeM::RegisterAllNativeMethods(JNIEnv* jni)
{
	if (jni == NULL)
	{
		return;
	}
	_LOGI("RegisterAllNativeMethods classes size = %d", ls_srndindex);
	/*for (int i = 0; i < s_nativemethods.size(); i++)
	{
		if (s_nativemethods[i]->m_nativem != NULL && s_nativemethods[i]->m_nnum > 0)
		{
			jclass clz = g_class_reference_holder->GetClass(s_nativemethods[i]->m_classname);
			if (clz != NULL)
			{
				if (jni->RegisterNatives(clz, s_nativemethods[i]->m_nativem, s_nativemethods[i]->m_nnum)) {
					LOGE("class : %s   methods not registered", s_nativemethods[i]->m_classname.c_str());
				}
			}
		}
	}*/
	//first load all classes
	for (int i = 0; i < ls_srndindex; i++)
	{
		g_class_reference_holder->LoadClass(jni, ls_srnd[i]->m_classname); 
		_LOGI("index = %d classes name = %s", ls_srndindex, ls_srnd[i]->m_classname.c_str());
	}
	//second registe all native methods;
	for (int i = 0; i < ls_srndindex; i++)
	{
		if (ls_srnd[i]->m_nativem != NULL && ls_srnd[i]->m_nnum > 0)
		{
			jclass clz = g_class_reference_holder->GetClass(ls_srnd[i]->m_classname);
			if (clz != NULL)
			{
				if (jni->RegisterNatives(clz, ls_srnd[i]->m_nativem, ls_srnd[i]->m_nnum)) {
					_LOGE("class : %s   methods not registered", ls_srnd[i]->m_classname.c_str());
				}
				else
				{
					_LOGI("class : %s   methods count: %d   registered success", ls_srnd[i]->m_classname.c_str(), ls_srnd[i]->m_nnum);
				}
			}
		}
	}
}

CRegisterNativeM::CRegisterNativeM(const char* classname, JNINativeMethod* nativem, int nativemnum)
{
	_LOGI("CRegisterNativeM %s", classname);
	SRegisterNativeData* temdata = new SRegisterNativeData;
	temdata->m_classname.assign(classname);
	temdata->m_nativem = nativem;
	temdata->m_nnum = nativemnum;
	//s_nativemethods.push_back(temdata);
	ls_srnd[ls_srndindex] = temdata;
	ls_srndindex++;
	_LOGI("CRegisterNativeM end %s   all size = %d", classname, s_nativemethods.size());

}


