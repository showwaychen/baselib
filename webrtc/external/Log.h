#ifndef LOG_H_
#define LOG_H_
#include "webrtc/base/logging.h"
#define SRTCLIB_TAG "srtclib"

extern const char *TAG;
void SetTag(const std::string &tag);
#define ALOG(sev)                       \
	LOG_SEVERITY_PRECONDITION(rtc::sev) \
	rtc::LogMessage(__FILE__, __LINE__, rtc::sev, TAG).stream()

#define LOGV \
	ALOG(LS_VERBOSE)
#define LOGI \
	ALOG(LS_INFO)
#define LOGW \
	ALOG(LS_WARNING)
#define LOGE \
	ALOG(LS_ERROR)
#define LOGD \
	ALOG(LS_INFO)

#endif /* LOG_H_ */
