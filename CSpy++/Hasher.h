#pragma once

#include "Hash.h"

Hash hash_file(const std::wstring& path);
Hash hash_file(const std::string& path);

Hash hash_bytes(const unsigned char* data, size_t length);