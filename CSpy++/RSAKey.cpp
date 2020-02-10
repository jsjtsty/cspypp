#include "RSAKey.h"
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
using namespace std;

RSAKey::RSAKey()
{
}

RSAKey::RSAKey(const wstring_view path)
{
	this->path = path.data();
}

RSAKey::RSAKey(const RSAKey& other)
{
	BIO* bio = BIO_new(BIO_s_mem());
	PEM_write_bio_RSAPublicKey(bio, other.key);
	PEM_read_bio_RSAPublicKey(bio, &key, NULL, NULL);
	BIO_free(bio);
}

RSAKey::~RSAKey()
{
	if (key) {
		RSA_free(key);
	}
}

RSA* RSAKey::newKey()
{
	key = RSA_new();
	return key;
}

RSA* RSAKey::getKey()
{
	return key;
}

RSA* RSAKey::readKey()
{
	FILE* file;
	errno_t err = _wfopen_s(&file, path.c_str(), L"r");
	if (err) {
		return nullptr;
	}
	PEM_read_RSAPublicKey(file, &key, NULL, NULL);
	fclose(file);
	return key;
}

RSA* RSAKey::generateKey()
{
	if (key) {
		RSA_free(key);
	}

	int ret = 0;
	key = RSA_new();
	BIGNUM* bn = BN_new();
	BN_set_word(bn, RSA_F4);
	ret = RSA_generate_key_ex(key, 2048, bn, NULL);
	return key;
}

int RSAKey::writeKey()
{
	FILE* file;
	errno_t err = _wfopen_s(&file, path.c_str(), L"w");
	if (err) {
		fclose(file);
		return err;
	}
	int ret = PEM_write_RSAPublicKey(file, key);
	if(file)
	fclose(file);
	return ret;
}

RSAKey::operator RSA* () const
{
	return key;
}
