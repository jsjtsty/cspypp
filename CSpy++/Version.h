#pragma once
#include "general.h"
#include "Hash.h"
#include <string>

class Version
{
public:
	enum class PreviewType : unsigned int {
		UNKNOWN,
		RELEASE,
		TECHNICAL_PREVIEW,
		INSIDER_PREVIEW,
		ALPHA,
		BETA
	};

	struct Package {
		bool available;
		std::wstring href;
		Hash sha256;
	};

	Version();
	Version(unsigned int major, unsigned int minor, unsigned int fix, unsigned int build);
	Version(unsigned int major, unsigned int minor, unsigned int fix, unsigned int build, PreviewType type);

	void setPreview(PreviewType type);
	bool isPreview() const;
	PreviewType getPreviewType() const;
	std::wstring getPreviewName() const;

	void setMajor(unsigned int _val);
	void setMinor(unsigned int _val);
	void setFix(unsigned int _val);
	void setBuild(unsigned int _val);

	unsigned int getMajor() const;
	unsigned int getMinor() const;
	unsigned int getFix() const;
	unsigned int getBuild() const;

	void setPackage(Package pkg);
	void setPackage(const std::wstring& href, Hash sha256);
	void setPackage64(Package pkg64);
	void setPackage64(const std::wstring& href, Hash sha256);

	Package getPackage() const;
	Package getPackage64() const;

	std::wstring getVersionName() const;
	std::wstring getFullVersionName() const;

	void setDescription(const std::wstring _val);
	std::wstring getDescription() const;

	bool operator==(const Version& ver) const;
	bool operator<(const Version& ver) const;
	bool operator<=(const Version& ver) const;
	bool operator>(const Version& ver) const;
	bool operator>=(const Version& ver) const;

	static Version currentVersion;
protected:
	unsigned int major, minor, fix, build;
	PreviewType previewType;
	std::wstring description;
	Package package, package64;
};

