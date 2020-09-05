#ifndef SRC_CREDENTIALS_H_
#define SRC_CREDENTIALS_H_

#include <string>
#include <tuple>

namespace keytar {

typedef std::tuple<std::string, std::string, std::string> Credentials;

}

#endif  // SRC_CREDENTIALS_H_
