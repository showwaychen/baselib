
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include"jni_classloader.h"
#include "jni_onload.h"
//extern JavaVM *g_JavaVM;
int on_JNI_OnLoad(JavaVM* vm, int jniversion) {
	// Only called once.
	JNIEnv *env = NULL;
	if (JNI_OK != vm->GetEnv((void **)&env, jniversion)) {
		return -1;
	}
	//load all classes
	LoadClasses(env);

	//register native methods
	RegisterNativeMethods(env);
	return 0;
}