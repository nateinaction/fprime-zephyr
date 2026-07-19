// ======================================================================
// \title fprime-zephyr/Os/Directory.cpp
// \brief Zephyr implementation for Os::Directory
// ======================================================================
#include <sys/stat.h>
#include <cerrno>
#include <cstring>

#include <Fw/Types/Assert.hpp>
#include <Fw/Types/StringUtils.hpp>
#include <fprime-zephyr/Os/Directory.hpp>
#include <Os/Posix/error.hpp>
#include <sys/types.h>
#include <dirent.h>
#include <zephyr/sys/printk.h>

namespace Os {
namespace Zephyr {
namespace Directory {

DirectoryHandle* ZephyrDirectory::getHandle() {
    return &this->m_handle;
}

ZephyrDirectory::Status ZephyrDirectory::open(const char* path, OpenMode mode) {
    Status status = Status::OP_OK;

    // The path must be stored for rewind(), so reject paths that would truncate
    if (Fw::StringUtils::string_length(path, ZephyrDirectoryHandle::PATH_MAX_LENGTH) ==
        ZephyrDirectoryHandle::PATH_MAX_LENGTH) {
        return Status::OTHER_ERROR;
    }

    // If one of the CREATE mode, attempt to create the directory
    if (mode == OpenMode::CREATE_EXCLUSIVE || mode == OpenMode::CREATE_IF_MISSING) {
        if (::mkdir(path, S_IRWXU) == -1) {
            status = Os::Posix::errno_to_directory_status(errno);
            // If error is not ALREADY_EXISTS, return the error
            // If any error and mode CREATE_EXCLUSIVE, return the error
            // Else, we keep going with OP_OK
            if (status != Status::ALREADY_EXISTS || mode == OpenMode::CREATE_EXCLUSIVE) {
                return status;
            } else {
                status = Status::OP_OK;
            }
        }
    }

    DIR* dir = ::opendir(path);

    if (dir == nullptr) {
        // errno is otherwise collapsed to a coarse Status (e.g. EMFILE -> OTHER_ERROR)
        printk("opendir(%s) failed: errno %d\n", path, errno);
        status = Os::Posix::errno_to_directory_status(errno);
    } else {
        (void)Fw::StringUtils::string_copy(this->m_handle.m_path, path, ZephyrDirectoryHandle::PATH_MAX_LENGTH);
    }

    this->m_handle.m_dir_descriptor = dir;
    return status;
}

ZephyrDirectory::Status ZephyrDirectory::rewind() {
    // Zephyr declares but does not implement rewinddir(), so emulate a rewind
    // by reopening the directory. Open the new stream before closing the old
    // one so the handle stays valid if the reopen fails.
    DIR* dir = ::opendir(this->m_handle.m_path);
    if (dir == nullptr) {
        printk("opendir(%s) failed: errno %d\n", this->m_handle.m_path, errno);
        return Os::Posix::errno_to_directory_status(errno);
    }
    if (this->m_handle.m_dir_descriptor != nullptr) {
        (void)::closedir(this->m_handle.m_dir_descriptor);
    }
    this->m_handle.m_dir_descriptor = dir;
    return Status::OP_OK;
}

ZephyrDirectory::Status ZephyrDirectory::read(char* fileNameBuffer, FwSizeType bufSize) {
    FW_ASSERT(fileNameBuffer);

    Status status = Status::OP_OK;

    // Set errno to 0 so we know why we exited readdir
    // This is recommended by the manual pages (man 3 readdir)
    errno = 0;

    struct dirent* direntData = nullptr;
    while ((direntData = ::readdir(this->m_handle.m_dir_descriptor)) != nullptr) {
        // Skip . and .. directory entries
        if ((direntData->d_name[0] == '.' and direntData->d_name[1] == '\0') or
            (direntData->d_name[0] == '.' and direntData->d_name[1] == '.' and direntData->d_name[2] == '\0')) {
            continue;
        } else {
            (void)Fw::StringUtils::string_copy(fileNameBuffer, direntData->d_name, bufSize);
            break;
        }
    }
    if (direntData == nullptr) {
        // loop ended because readdir failed, did it error or did we run out of files?
        if (errno != 0) {
            // Only error from readdir is EBADF
            status = Status::BAD_DESCRIPTOR;
        } else {
            status = Status::NO_MORE_FILES;
        }
    }
    return status;
}

void ZephyrDirectory::close() {
    // ::closedir errors if dir descriptor is nullptr
    if (this->m_handle.m_dir_descriptor != nullptr) {
        (void)::closedir(this->m_handle.m_dir_descriptor);
    }
    this->m_handle.m_dir_descriptor = nullptr;
}

}  // namespace Directory
}  // namespace Zephyr
}  // namespace Os
