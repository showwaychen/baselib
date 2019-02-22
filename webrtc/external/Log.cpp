#include "Log.h"
const char *CURTAG = "baselib";

void SetTag(const std::string &tag)
{
    CURTAG = tag.c_str();
}
