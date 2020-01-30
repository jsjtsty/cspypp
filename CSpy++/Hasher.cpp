#include "general.h"
#include "Hasher.h"
#include <cstdio>
#include <stdexcept>
#include <openssl/sha.h>
#include <openssl/evp.h>
using namespace std;

Hash hash_file(const std::wstring& path)
{
	FILE* file;
	errno_t err = _wfopen_s(&file, path.c_str(), L"rb");
	if (err != 0) {
		throw runtime_error("Could not open file.");
	}

	unsigned char res[32];
	EVP_MD_CTX* ctx = EVP_MD_CTX_new();
	EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
	
	size_t readCount, full = 0;
	unsigned char buffer[1024];
	while (readCount = fread_s(buffer, 1024, 1, 1024, file)) {
		EVP_DigestUpdate(ctx, buffer, readCount);
		full += readCount;
	}

	unsigned int len;
	EVP_DigestFinal_ex(ctx, res, &len);
	EVP_MD_CTX_free(ctx);
	fclose(file);

	return Hash(res);
}

Hash hash_file(const std::string& path)
{
	FILE* file;
	errno_t err = fopen_s(&file, path.c_str(), "rb");
	if (err != 0) {
		throw runtime_error("Could not open file.");
	}

	unsigned char res[32];
	EVP_MD_CTX* ctx = EVP_MD_CTX_new();
	EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);

	size_t readCount;
	unsigned char buffer[1024];
	while (readCount = fread_s(buffer, sizeof(buffer), 1, sizeof(buffer), file)) {
		EVP_DigestUpdate(ctx, buffer, readCount);
	}

	unsigned int len;
	EVP_DigestFinal_ex(ctx, res, &len);
	EVP_MD_CTX_free(ctx);
	fclose(file);

	return Hash(res);
}

Hash hash_bytes(const unsigned char* data, size_t length)
{
	Hash result;
	unsigned int len;
	EVP_MD_CTX* ctx = EVP_MD_CTX_new();
	EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
	EVP_DigestUpdate(ctx, data, length);
	EVP_DigestFinal_ex(ctx, result, &len);
	return result;
}