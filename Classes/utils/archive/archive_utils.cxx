// Copyright (c) 2018 Mozart Alexander Louis. All rights reserved.

// Includes
#include "archive_utils.hxx"
#include "unzip/unzip.h"
#include "xxhash/xxhash.h"

Data ArchiveUtils::loadData(const string& file, const string& archive) {
  ssize_t t;

// Getting the data from the zip file varies based on platform.
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID or CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
  const auto data = getFileDataFromZip(FileUtils::getInstance()->getWritablePath() + archive,
                                       __ARCHIVE_ROOT__ + file, &t, genZipPassword(archive));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  const auto data = getFileDataFromZip(FileUtils::getInstance()->fullPathForFilename(archive),
                                       __ARCHIVE_ROOT__ + file, &t, genZipPassword(archive));
#else
  const auto data = getFileDataFromZip(archive, __ARCHIVE_ROOT__ + file, &t, genZipPassword(archive));
#endif

  // Create data object
  Data d;
  d.fastSet(data, t);
  return d;
}

ValueMap ArchiveUtils::loadValueMap(const string& file, const string& archive) {
  auto data = loadData(file, archive);

  // Return data from file
  return FileUtils::getInstance()->getValueMapFromData(reinterpret_cast<const char*>(data.getBytes()),
                                                       static_cast<int>(data.getSize()));
}

ValueVector ArchiveUtils::loadValueVector(const string& file, const string& archive) {
  auto data = loadData(file, archive);

  // Return data from file

  auto map = FileUtils::getInstance()->getValueMapFromData(reinterpret_cast<const char*>(data.getBytes()),
                                                           static_cast<int>(data.getSize()));
  return map.at(__DATA__).asValueVector();
}

string ArchiveUtils::loadString(const string& file, const string& archive) {
  auto data = loadData(file, archive);

  // Return the data in string format
  string s(reinterpret_cast<char const*>(data.getBytes()), data.getSize());
  return s;
}

string ArchiveUtils::genZipPassword(const string& salt) {
  // Creating temp locals
  const auto a = string(to_string(__ARCHIVE_PART1__));
  const auto b = string(to_string(__ARCHIVE_PART2__));
  const auto c = string(to_string(__ARCHIVE_PART3__));
  const auto d = string(to_string(__ARCHIVE_PART4__));

  // Generate the hash password to the zip archive
  return to_string(XXH32((a + d + salt + b + c + a).c_str(),
                         static_cast<int>((a + d + salt + b + c + a).length()), 86719205)) +
         to_string(XXH32((d + c + a + salt + b + c).c_str(),
                         static_cast<int>((d + c + a + salt + b + c).length()), 88572036));
}

unsigned char* ArchiveUtils::getFileDataFromZip(const string& archive, const string& name, ssize_t* size,
                                                const string& password) {
  // Validate path is not empty
  CCASSERT(!archive.empty(), "No archive passed...");

// Opening the zip file
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS or CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
  auto location = FileUtils::getInstance()->fullPathForFilename(archive);
#else
  auto location = FileUtils::getInstance()->getSuitableFOpen(archive);
#endif
  const auto file = unzOpen(location.c_str());
  CCASSERT(file, "Unable to open file");

  // Tries to locate the szFileName in the zip file.
  auto r = unzLocateFile(file, name.c_str(), 1);
  CCASSERT(UNZ_OK == r, "Unable to find szFileName in the zip file");

  // Retrieve the file info
  char file_path_a[260];
  unz_file_info file_info{};
  r = unzGetCurrentFileInfo(file, &file_info, file_path_a, sizeof file_path_a, nullptr, 0, nullptr, 0);
  CCASSERT(UNZ_OK == r, "Failed to receive file info");

  // Open the file. If the password field isn't empty, We will try to open it with a password
  // if (!password.empty()) r = unzOpenCurrentFilePassword(file, password.c_str());
  if (!password.empty())
    r = unzOpenCurrentFilePassword(file, password.c_str());
  else
    r = unzOpenCurrentFile(file);
  CCASSERT(UNZ_OK == r, "Failed to open zip");

  // allocate date to a buffer and attempt to read the file
  const auto buffer = static_cast<unsigned char*>(malloc(file_info.uncompressed_size));
  r = unzReadCurrentFile(file, buffer, static_cast<unsigned>(file_info.uncompressed_size));
  CCASSERT(r == 0 or r == (int)file_info.uncompressed_size, "File size is wrong");

  *size = 0;
  *size = file_info.uncompressed_size;
  unzCloseCurrentFile(file);

  if (file not_eq nullptr) unzClose(file);
  return buffer;
}
