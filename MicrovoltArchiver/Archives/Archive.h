#pragma once
#ifndef _ARCHIVE_H_
#define _ARCHIVE_H_

#include <experimental/filesystem>
#include <iostream>
#include <fstream>
#include <ZipArchive.h>
#include <ZipFile.h>
#include <streams/memstream.h>
#include <methods/DeflateMethod.h>
using namespace std::experimental::filesystem::v1;

#define ACTION_UNPACK 1
#define ACTION_PACK   2

class Archive {

protected:
	static std::string m_target;
	static char		   m_action;

	static const std::string __defaultArchivePassword;

private:
	ZipArchive::Ptr m_archive	   = nullptr;
	size_t			m_totalEntries = NULL;
	bool			m_isOpen	   = false;

	std::fstream* _getOutStream(std::istream* _inStream, const std::string& _name, size_t _expectedSize);

public:

	static void Init(char _action, std::string _target);

	Archive() {};

	~Archive() {
		if (this->m_archive != nullptr) {
			this->m_archive->~ZipArchive();
			this->m_isOpen = false;
		}

		if (Archive::m_target.length() > 0)
			Archive::m_target.clear();
	}

	bool Open();
	const std::string& GetArchiveComment();

	std::shared_ptr<ZipArchiveEntry> GetFile(const std::string& _name);
	std::shared_ptr<ZipArchiveEntry> GetFile(size_t _index);
	bool ExtractAll();

};

class ArchWriter : public Archive {

private:
	ZipArchive::Ptr m_archive = nullptr;

	bool isDirectory(const std::string& _path);
	std::vector<directory_entry> getFileList(const std::string& _path);

public:

	ArchWriter() {};
	~ArchWriter() {
		if (this->m_archive != nullptr)
			this->m_archive->~ZipArchive();


	}

	bool PackAll(const std::string& _path, const std::string& _outArchive, bool _forceNoEncryption = false);
};

#endif