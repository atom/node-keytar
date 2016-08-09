#ifndef SRC_KEYTAR_POSIX_H_
#define SRC_KEYTAR_POSIX_H_

#include <string>

namespace keytar {

// Using gnome-keyring
bool GAddPassword(const std::string& service,
                  const std::string& account,
                  const std::string& password);

bool GGetPassword(const std::string& service,
                  const std::string& account,
                  std::string* password);

bool GDeletePassword(const std::string& service, const std::string& account);

bool GFindPassword(const std::string& service, std::string* password);

#ifdef KEYTAR_KWALLET_BACKEND
// Using kwallet
bool KAddPassword(const std::string& service,
                  const std::string& account,
                  const std::string& password);

bool KGetPassword(const std::string& service,
                  const std::string& account,
                  std::string* password);

bool KDeletePassword(const std::string& service, const std::string& account);

bool KFindPassword(const std::string& service, std::string* password);

#endif  // KEYTAR_KWALLET_BACKEND

}  // namespace keytar

#endif  // SRC_KEYTAR_POSIX_H_
