#pragma once

#include <string>
typedef struct rsa_st RSA;

class RSAKey
{
public:
	RSAKey();
	RSAKey(const std::wstring_view path);
	RSAKey(const RSAKey& other);
	~RSAKey();

	RSA* newKey();
	RSA* getKey();
	RSA* readKey();
	RSA* generateKey();
	int writeKey();

	operator RSA* () const;

protected:
	std::wstring path;
	RSA* key = nullptr;
};

