/** Copyright (c) 2018 Mozart Alexander Louis. All rights reserved. */

#ifndef __ARCHIVE_UTILS_HXX__
#define __ARCHIVE_UTILS_HXX__

/**
 * Archive Attributes
 */
#define __ARCHIVE_NAME__ "evz.archive"
#define __ARCHIVE_ROOT__ "xScripts/"

/**
 * Archive Password Parts
 */
#define __ARCHIVE_PART1__ 0x51277094
#define __ARCHIVE_PART2__ 0x88257473
#define __ARCHIVE_PART3__ 0x57138953
#define __ARCHIVE_PART4__ 0x06917385

/**
 * Includes
 */
#include "globals.hxx"

class ArchiveUtils {
 public:
  /**
   * Loads a file from the password protected archive into the cocos2d::Data format.
   *
   * @param file Name of the file in the password protected archive
   * @param archive The Name of the password protected archive
   */
  static Data loadData(const string& file, const string& archive = __ARCHIVE_NAME__);

  /**
   * Loads a file from the password protected archive into the cocos2d::ValueMap format.
   *
   * @param file    ~ Name of the file in the password protected archive.
   * @param archive ~ The Name of the password protected archive.
   *
   * @returns A ValueMap from file
   */
  static ValueMap loadValueMap(const string& file, const string& archive = __ARCHIVE_NAME__);

  /**
   * Loads a file from the password protected archive into the cocos2d::ValueVector format.
   *
   * @param file    ~ Name of the file in the password protected archive.
   * @param archive ~ The Name of the password protected archive.
   *
   * @returns A ValueVector from file
   *
   * @note ~ The root must be a ValueMap with one key `data` continaing the array.
   */
  static ValueVector loadValueVector(const string& file, const string& archive = __ARCHIVE_NAME__);

  /**
   * Loads a file from the password protected archive into a string
   *
   * @param file    ~ Name of the file in the password protected archive.
   * @param archive ~ The Name of the password protected archive.
   *
   * @returns String version of the file. Nice for loading Json
   */
  static string loadString(const string& file, const string& archive = __ARCHIVE_NAME__);

 private:
  /**
   * @brief Generates the hash password for the zip on the fly.
   *
   * @param salt ~ The salt added to generate the specific password only for that archive.
   *               Usually this is just the name of the archive.
   *
   * @returns Hashed password.
   */
  static string genZipPassword(const string& salt);

  /**
   * @brief Retrieves data from a encrypted archive.
   *
   * @param archive  ~ 7z store archive where data is stored.
   * @param name     ~ Name of the file located inside of the archive.
   * @param size     ~ Pointer for storing the file size.
   * @param password ~ Optional password 7z store archive.
   *
   * @return Data of the file in zip.
   *
   * @note If the zip file is password protected, the archive must be packaged using the `store` option in an
   * implementation of 7zip. No compression should be applied or this will return garbage data.
   */
  static unsigned char* getFileDataFromZip(const string& archive, const string& name, ssize_t* size,
                                           const string& password = "");

  /**
   * __DISALLOW_IMPLICIT_CONSTRUCTORS__
   */
  __DISALLOW_IMPLICIT_CONSTRUCTORS__(ArchiveUtils)
};

#endif  // __ARCHIVE_UTILS_HXX__
