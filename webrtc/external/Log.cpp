#include "Log.h"
const char *TAG = "baselib";

void SetTag(const std::string &tag)
{
    TAG = tag.c_str();
}
