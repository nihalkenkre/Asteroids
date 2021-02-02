#ifndef LOG_HPP
#define LOG_HPP

#include "error.hpp"

#include <string>

AGE_RESULT log_error (const AGE_RESULT result);

#ifdef __ANDROID__
AGE_RESULT log_verbose (const std::string& info);
#endif

#endif