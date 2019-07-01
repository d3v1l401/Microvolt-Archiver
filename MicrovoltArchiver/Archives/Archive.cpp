#include "Archive.h"

#include <vector>
#include <algorithm>
#pragma comment(lib, "ZipLib.lib")
#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "lzma.lib")
#pragma comment(lib, "bzip2.lib")
#include "..\CColors\advconsole.h"
using namespace AdvancedConsole;

std::string			Archive::m_target = "";
char				Archive::m_action = 0;

const std::string	Archive::__defaultArchivePassword = "??KVM-7400-$PETER$??"; // This was encrypted & hardcoded, I just dumped the decrypted string.
	// There's also another one:
	//		DB00A62C4DC2EA9F77575C7D577E9DEEB77EFD921DAAA59BEF89288EB51B6992B4A543411D707D38761E47C51FB5B84ED5ECE4D61A757074CB11E16D7E744212
	// Which I still don't know what it is used for, but certain archives are partially encrypted, I suspect this is used for it.

std::fstream* Archive::_getOutStream(std::istream* _inStream, const std::string& _name, size_t _expectedSize) {
	if (!(_inStream != nullptr && _inStream->good()))
		return nullptr;

	std::fstream* _stream = new std::fstream(_name, std::ios::out | std::ios::binary);
	if (_stream != nullptr && _stream->is_open()) {
		for (size_t _index = 0; (_index < _expectedSize && _inStream); _index++) {
			char _byte = _inStream->get();
			_stream->write(&_byte, sizeof(char));
		}

		return _stream;
	}

	return nullptr;
}

void Archive::Init(char _action, std::string _target) {

	if ((_action == ACTION_PACK || _action == ACTION_UNPACK) && _target.length() > 0) {

		Archive::m_target.assign(_target);
		Archive::m_action = _action;

	} else
		std::cout << Color(AC_RED) << "Parsed parameters are not valid.";
}

bool Archive::Open() {
	this->m_archive = ZipFile::Open(Archive::m_target);
	if (!this->m_archive)
		return false;

	this->m_totalEntries = this->m_archive->GetEntriesCount();
	if (!(this->m_totalEntries > 0))
		return false;

	this->m_isOpen = true;
	return true;
}

const std::string& Archive::GetArchiveComment() {
	if (this->m_isOpen)
		return this->m_archive->GetComment();

	return "";
}

std::shared_ptr<ZipArchiveEntry> Archive::GetFile(size_t _index) {
	if (this->m_isOpen)
		if (_index > 0 && _index < this->m_totalEntries)
			return this->m_archive->GetEntry(_index);

	return nullptr;
}

std::shared_ptr<ZipArchiveEntry> Archive::GetFile(const std::string& _name) {
	if (this->m_isOpen) 
		for (size_t i = 0; i < this->m_totalEntries; i++)
			if (!this->m_archive->GetEntry(i)->GetName().compare(_name))
				return this->m_archive->GetEntry(i);

	return nullptr;
}

#pragma warning(disable: 4996)
bool Archive::ExtractAll() {
	if (this->m_isOpen) {

		CreateDirectory((Archive::m_target + "_out").c_str(), NULL);

		for (size_t i = 1; i < this->m_totalEntries; i++) {
			auto _entry = this->GetFile(i);

			if (_entry != nullptr) {

				auto _containsSubDir = 0;
			    _containsSubDir = _entry->GetFullName().find_first_of('/');

				if (_entry->IsDirectory()) {
					char* dir = new char[MAX_PATH]{ 0 };

					if (Archive::m_target.empty()) {
						std::cout << Color(AC_YELLOW) << "Target archive is empty (how?), using placeholder root folder \"" 
								  << Color(AC_GREEN)  << "Extracted" << Color(AC_YELLOW) << "\".";

						Archive::m_target.assign("Extracted");
					}

					sprintf(dir, "%s\\%s", (Archive::m_target + "_out").c_str(), _entry->GetFullName().c_str());
					CreateDirectory(dir, NULL);

					free(dir);
					continue;
				}

				if (_containsSubDir > 0 ? true : false) {
					auto _subDir = _entry->GetFullName().substr(0, _entry->GetFullName().find_first_of('/'));
					char* dir = new char[MAX_PATH] { 0 };

					sprintf(dir, "%s\\%s", (Archive::m_target + "_out").c_str(), _subDir.c_str());
					CreateDirectory(dir, NULL);

					free(dir);
				}

				if (_entry->IsPasswordProtected())
					_entry->SetPassword(Archive::__defaultArchivePassword);

				if (!_entry->CanExtract()) {
					std::cout << Color(AC_WHITE) << "[" << Color(AC_RED) << "!" << Color(AC_WHITE) << "]" << Color(AC_YELLOW) << "Can not extract \"" << Color(AC_GREEN) << _entry->GetFullName() << Color(AC_YELLOW) << "\"\n" << Color(AC_WHITE);
					continue;
				}
				
				{
					std::cout << Color(AC_WHITE) << "\n[" << Color(AC_GREEN) << "+" << Color(AC_WHITE) << "] Extracting \"" << Color(AC_GREEN) << _entry->GetFullName() << Color(AC_WHITE) << "\"\n";

					if (!_entry->GetComment().empty())
						std::cout << Color(AC_GREEN) << " | " << Color(AC_WHITE) << "" << Color(AC_GREEN) << _entry->GetComment() << Color(AC_WHITE) << "\n";

					std::ios_base::fmtflags f(std::cout.flags());

					std::cout << Color(AC_GREEN) << " | " << Color(AC_WHITE) << "Size: " << Color(AC_GREEN) << _entry->GetCompressedSize() << Color(AC_WHITE) << " bytes -> " << Color(AC_CYAN) << _entry->GetSize() << Color(AC_WHITE) << " bytes\n"
							  << Color(AC_GREEN) << " | " << Color(AC_WHITE) << "CRC: " << Color(AC_MAGENTA) << "0x" << std::hex << std::uppercase << _entry->GetCrc32() << Color(AC_WHITE) << "\n"
							  << Color(AC_GREEN) << " | " << Color(AC_WHITE) << "Is Encrypted: " << Color(AC_CYAN) << (_entry->IsPasswordProtected() ? "Yes" : "No") << Color(AC_WHITE) << "\n"
							  << Color(AC_GREEN) << " | " << Color(AC_WHITE) << "Compression: " << Color(AC_YELLOW) << (_entry->GetCompressionMethod() == DeflateMethod::CompressionMethod ? "DEFLATE" : "RAW STORED") << Color(AC_WHITE) << "\n"
							  << "[" << Color(AC_GREEN) << "-" << Color(AC_WHITE) << "]\n";

					std::cout.flags(f);
					
					auto _outStream = this->_getOutStream(_entry->GetDecompressionStream(), (Archive::m_target + "_out\\" + _entry->GetFullName()), _entry->GetSize());
					if (_outStream != nullptr) {

						_outStream->close();
						free(_outStream);

					} else {
						std::cout << Color(AC_WHITE) << "[" << Color(AC_RED) << "!" << Color(AC_WHITE) << "]" << Color(AC_YELLOW) << " Can not extract \"" << Color(AC_GREEN) << _entry->GetFullName() << Color(AC_YELLOW) << "\", failed output stream.\n" << Color(AC_WHITE);
						continue;
					}
				}

				_entry->~ZipArchiveEntry();

			} else {
				std::cout << Color(AC_RED) << "No file entry catched, archive might be damaged.";
				return false;
			}
		}

		return true;
	}

	return false;
}


bool ArchWriter::isDirectory(const std::string& _path) {
	return (GetFileAttributes(_path.c_str()) != INVALID_FILE_ATTRIBUTES) && (GetFileAttributes(_path.c_str()) & FILE_ATTRIBUTE_DIRECTORY);
}

std::vector<directory_entry> ArchWriter::getFileList(const std::string& _path) {
	std::vector<directory_entry> _vec;

	for (const auto& p : recursive_directory_iterator(_path))
		if (!is_directory(p))
			_vec.push_back(p);

	return _vec;
}

bool ArchWriter::PackAll(const std::string& _path, const std::string& _outArchive, bool _forceNoEncryption) {

	DeleteFile(_outArchive.c_str());

	if (this->isDirectory(_path)) {
		auto const _list = this->getFileList(_path);
		this->m_archive = ZipFile::Open(_outArchive);

		{
			for (auto i = 0; i < _list.size(); i++) {
				std::ifstream _fileStream(_list[i].path().generic_string(), std::ios::in | std::ios::binary);
				if (_fileStream.is_open() && _fileStream.good()) {

					auto _currentPath = _list[i].path().generic_string(); // Real Path
					auto _inZipName = _currentPath.substr(_currentPath.find_first_of("/") + 1, _currentPath.length()); // In-Zip path

					std::cout << Color(AC_WHITE) << "\n[" << Color(AC_GREEN) << "+" << Color(AC_WHITE) << "] Processing \"" << Color(AC_GREEN) << _inZipName << Color(AC_WHITE) << "\"\n";

					ZipArchiveEntry::Ptr _entry = this->m_archive->CreateEntry(_inZipName);

					_entry->SetPassword(Archive::__defaultArchivePassword);
					_entry->UseDataDescriptor();

					DeflateMethod::Ptr _deflate = DeflateMethod::Create();
					_deflate->SetCompressionLevel(DeflateMethod::CompressionLevel::L8); // Preserve same compression level of the original.

					_entry->SetCompressionStream(
						_fileStream, _deflate, ZipArchiveEntry::CompressionMode::Immediate
					);

					std::ios_base::fmtflags f(std::cout.flags());

					std::cout << Color(AC_GREEN) << " | " << Color(AC_WHITE) << "Size: " << Color(AC_GREEN) << _entry->GetSize() << Color(AC_WHITE) << " bytes -> " << Color(AC_CYAN) << _entry->GetCompressedSize() << Color(AC_WHITE) << " bytes\n"
						<< Color(AC_GREEN) << " | " << Color(AC_WHITE) << "CRC: " << Color(AC_MAGENTA) << "0x" << std::hex << std::uppercase << _entry->GetCrc32() << Color(AC_WHITE) << "\n"
						<< Color(AC_GREEN) << " | " << Color(AC_WHITE) << "Is Encrypted: " << Color(AC_CYAN) << (_entry->IsPasswordProtected() ? "Yes" : "No") << Color(AC_WHITE) << "\n"
						<< Color(AC_GREEN) << " | " << Color(AC_WHITE) << "Compression: " << Color(AC_YELLOW) << (_entry->GetCompressionMethod() == DeflateMethod::CompressionMethod ? "DEFLATE" : "RAW STORED") << Color(AC_WHITE) << "\n"
						<< "[" << Color(AC_GREEN) << "-" << Color(AC_WHITE) << "]\n";

					std::cout.flags(f);

				} else
					std::cout << Color(AC_WHITE) << "[" << Color(AC_RED) << "!" << Color(AC_WHITE) << "]" << Color(AC_YELLOW) << "Can not add \"" << Color(AC_GREEN) << _list[i].path().generic_string() << Color(AC_YELLOW) << "\"\n" << Color(AC_WHITE);

			}

			std::ofstream _out(_outArchive, std::ios::out | std::ios::binary);
			this->m_archive->WriteToStream(_out);
			_out.flush();
			_out.close();
		}

		return true;
	}

	return false;
}
