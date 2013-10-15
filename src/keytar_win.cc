#include "keytar.h"

namespace keytar {

bool AddPassword(const std::string& service,
                 const std::string& account,
                 const std::string& password) {
  return false;
}

bool GetPassword(const std::string& service,
                 const std::string& account,
                 std::string* password) {
  return false;
}

bool DeletePassword(const std::string& service,
                    const std::string& account) {
  return false;
}

}  // namespace keytar

