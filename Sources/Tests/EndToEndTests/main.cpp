/**
 * @file Tests/EndToEndTests/main.cpp
 * Contains the end-to-end test's main program.
 *
 * @copyright Copyright (C) 2014 Rafid Khalid Abdullah
 *
 * @license This file is released under Alusus Public License, Version 1.0.
 * For details on usage and copying conditions read the full license in the
 * accompanying license file or at <http://alusus.net/alusus_license_1_0>.
 */
//==============================================================================

#include <stdlib.h>
// Alusus header files
#include <core.h>

// System headers
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using Core::Data::Notice;
using Core::Data::Ast::List;
using Core::Standard::RootManager;

namespace Tests { namespace EndToEndTests
{

Str resultFilename;

/**
 * @brief Print the provided notices to the console.
 *
 * Printed notice includes severity, msg code, location, as well as
 * description.
 */
void printNotice(const SharedPtr<Notice> &msg)
{
  // Print severity.
  switch (msg->getSeverity()) {
  case 0:
    std::cout << "BLOCKER "; break;
  case 1:
    std::cout << "ERROR "; break;
  case 2:
  case 3:
    std::cout << "WARNING "; break;
  case 4:
    std::cout << "ATTN "; break;
  }
  // Print msg code.
  std::cout << msg->getCode() << " @ ";
  // Print location.
  std::cout << "(" << msg->getSourceLocation().line << "," << msg->getSourceLocation().column << "): ";
  // Print description.
  std::cout << msg->getDescription() << NEW_LINE;
}


Bool isDirectory(Char const *path)
{
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISDIR(path_stat.st_mode);
}


Bool compareStringEnd(Str const &str, Char const *end)
{
  Int len = getStrLen(end);
  if (len >= str.size()) return false;
  return compareStr(str.c_str()+str.size()-len, end) == 0;
}


/**
 * Executes the given Alusus source code and compare the result against
 * an output file (having the same name with .output at the end).
 *
 * @param[in] fileName  The name of Alusus source file name.
 *
 * @return Returns @c true if the function succeeds, otherwise @c false.
 */
Bool RunSourceFile(Str const &fileName)
{
  // Redirect stdout to a file.
  fpos_t pos;
  fgetpos(stdout, &pos);
  int fd = dup(fileno(stdout));
  freopen(resultFilename.c_str(),"w", stdout);

  try
  {
    // Prepare the root object;
    RootManager root;
    Slot<void, SharedPtr<Core::Data::Notice> const&> noticeSlot(printNotice);
    root.noticeSignal.connect(noticeSlot);

    // Parse the provided filename.
    auto ptr = root.processFile(fileName.c_str());

    // Restore stdout.
    fflush(stdout);
    dup2(fd, fileno(stdout));
    close(fd);
    clearerr(stdout);
    fsetpos(stdout, &pos);

    if (ptr == 0)
      return false;

    return true;
  }
  catch (Core::Basic::Exception &e)
  {
    // Restore stdout.
    fflush(stdout);
    dup2(fd, fileno(stdout));
    close(fd);
    clearerr(stdout);
    fsetpos(stdout, &pos);

    std::cout << "Failed to run source file " << fileName << "." << std::endl;
    std::cout << "The following error were thrown: " << std::endl;
    std::cout << e.getVerboseErrorMessage() << std::endl;
    return false;
  }
  catch (...)
  {
    // Restore stdout.
    fflush(stdout);
    dup2(fd, fileno(stdout));
    close(fd);
    clearerr(stdout);
    fsetpos(stdout, &pos);

    std::cout << "Failed to run source file " << fileName << "." << std::endl;
    throw;
  }
}


/**
 * Checks whether the run result of a source file matches the expected result.
 * The expected result is stored in a file having the same name followed by
 * ".output" post-fix.
 * @param[in] fileName  The name of Alusus source file name.
 *
 * @return Returns @c true if the run result matches the expected result,
 * otherwise @c false.
 */
Bool CheckRunResult(Str const &fileName)
{
  std::ifstream runResult(resultFilename);
  std::string runResultContent((std::istreambuf_iterator<char>(runResult)),
      std::istreambuf_iterator<char>());

  std::ifstream expectedResult(fileName + ".output");
  std::string expectedResultContent((std::istreambuf_iterator<char>(expectedResult)),
      std::istreambuf_iterator<char>());

  // Remove one character from the expectedResultContent because, for some
  // reason, editors seem to append 0A at the end of the file!
  auto ret =  runResultContent.compare(expectedResultContent) == 0;
  if (ret == true)
    std::cout << ">>> Test successful: " << fileName << std::endl;
  else
  {
    std::cout << ">>> Test failed: " << fileName << std::endl;
    std::cout << "Expected Result (Length = " << expectedResultContent.size() << "): " << std::endl;
    std::cout << expectedResultContent << std::endl;
    std::cout << "Received Result (Length = " << runResultContent.size() << "): " << std::endl;
    std::cout << runResultContent << std::endl;
  }
  return ret;
}


/**
 * Runs the given Alusus source file and checks the result against an expected
 * content file. The expected result is stored in a file having the same name
 * of the source file followed by ".output" post-fix.
 *
 * @param[in] fileName  The name of Alusus source file name.
 *
 * @return Returns @c true if the run result matches the expected result,
 * otherwise @c false.
 */
Bool RunAndCheckSourceFile(Str const &fileName)
{
  if (!RunSourceFile(fileName))
    return false;
  return CheckRunResult(fileName);
}


/**
 * Runs all the end-to-end tests under the folder EndToEndTests.
 *
 * @return Returns @c true if all test succeeds, otherwise @c false.
 */
Bool RunEndToEndTests(Str const &dirPath)
{
  DIR *dir;
  dirent *ent;
  if ((dir = opendir(dirPath.c_str())) != nullptr)
  {
    auto ret = true;
    while ((ent = readdir(dir)) != nullptr)
    {
      Str fileName(ent->d_name);
      Str filePath = dirPath + "/" + fileName;
      if (isDirectory(filePath.c_str()) && fileName != "." && fileName != "..") {
        if (!RunEndToEndTests(filePath)) ret = false;
      } else if (
        fileName.compare("common.alusus") != 0 && fileName.find("ignore.alusus") == std::string::npos &&
        (compareStringEnd(fileName, ".alusus") || compareStringEnd(fileName, ".أسس"))
      ) {
        if (!RunAndCheckSourceFile(filePath)) ret = false;
      }
    }
    closedir(dir);
    return ret;
  }
  else
  {
    std::cout << "Could not open end-to-end tests directory: " << dirPath << " !" << std::endl;
    return false;
  }
}

} } // namespace


using namespace Tests::EndToEndTests;

int main(int argc, char **argv)
{
  Char alususReleaseYear[5];
  copyStr(ALUSUS_RELEASE_DATE, alususReleaseYear, 4);
  alususReleaseYear[4] = 0;
  std::cout << "Alusus End-to-End Tests\n"
               "Version " ALUSUS_VERSION ALUSUS_REVISION " (" ALUSUS_RELEASE_DATE ")\n"
               "Copyright (C) " << alususReleaseYear << " Rafid Khalid Abdullah\n\n";

  // Prepare a temporary filename.
  Char const * tempPath = getenv("TMPDIR");
  if (tempPath == 0) tempPath = getenv("TMP");
  if (tempPath == 0) tempPath = getenv("TEMP");
  if (tempPath == 0) tempPath = getenv("TEMPDIR");
  if (tempPath == 0) tempPath = "/tmp/";
  resultFilename = tempPath;
  if (resultFilename.back() != '/') resultFilename += "/";
  resultFilename += "AlususEndToEndTest.txt";

  auto ret = EXIT_SUCCESS;
  //  if (!RunEndToEndTests("./Tests/General"))
  //    ret = EXIT_FAILURE;
  //  if (!RunEndToEndTests("./Tests/Arabic"))
  //    ret = EXIT_FAILURE;
  //  if (!RunEndToEndTests("./Tests/Ported_C_Examples"))
  //    ret = EXIT_FAILURE;
  if (!RunEndToEndTests("./Tests/Core"))
    ret = EXIT_FAILURE;
  if (!RunEndToEndTests("./Tests/Spp"))
    ret = EXIT_FAILURE;

  std::remove(resultFilename.c_str());

  return ret;
}
