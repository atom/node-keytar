#include "keytar.h"

#include <windows.h>
#include <wincred.h>

namespace keytar {

std::string getErrorMessage(DWORD errorCode) {
  LPVOID errBuffer;
  ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL, errorCode, 0, (LPTSTR) &errBuffer, 0, NULL);
  std::string errMsg = std::string(reinterpret_cast<char*>(errBuffer));
  LocalFree(errBuffer);
  return errMsg;
}

KEYTAR_OP_RESULT SetPassword(const std::string& service,
                 const std::string& account,
                 const std::string& password,
                 std::string* errStr) {
  std::string target_name = service + '/' + account;

  CREDENTIAL cred = { 0 };
  cred.Type = CRED_TYPE_GENERIC;
  cred.TargetName = const_cast<char*>(target_name.c_str());
  cred.CredentialBlobSize = password.size();
  cred.CredentialBlob = (LPBYTE)(password.data());
  cred.Persist = CRED_PERSIST_LOCAL_MACHINE;

  bool result = ::CredWrite(&cred, 0);
  if (!result) {
    *errStr = getErrorMessage(::GetLastError());
    return FAIL_ERROR;
  } else {
    return SUCCESS;
  }
}

KEYTAR_OP_RESULT GetPassword(const std::string& service,
                 const std::string& account,
                 std::string* password,
                 std::string* errStr) {
  std::string target_name = service + '/' + account;

  CREDENTIAL* cred;
  bool result = ::CredRead(target_name.c_str(), CRED_TYPE_GENERIC, 0, &cred);
  if (!result) {
    DWORD code = ::GetLastError();
    if (code == ERROR_NOT_FOUND) {
      return FAIL_NONFATAL;
    } else {
      *errStr = getErrorMessage(code);
      return FAIL_ERROR;
    }
  }

  *password = std::string(reinterpret_cast<char*>(cred->CredentialBlob),
                          cred->CredentialBlobSize);
  ::CredFree(cred);
  return SUCCESS;
}

KEYTAR_OP_RESULT DeletePassword(const std::string& service,
                    const std::string& account,
                    std::string* errStr) {
  std::string target_name = service + '/' + account;

  bool result = ::CredDelete(target_name.c_str(), CRED_TYPE_GENERIC, 0);
  if (!result) {
    DWORD code = ::GetLastError();
    if (code == ERROR_NOT_FOUND) {
      return FAIL_NONFATAL;
    } else {
      *errStr = getErrorMessage(code);
      return FAIL_ERROR;
    }
  }

  return SUCCESS;
}

KEYTAR_OP_RESULT FindPassword(const std::string& service,
                  std::string* password,
                  std::string* errStr) {
  std::string filter = service + "*";

  DWORD count;
  CREDENTIAL** creds;
  bool result = ::CredEnumerate(filter.c_str(), 0, &count, &creds);
  if (!result) {
    DWORD code = ::GetLastError();
    if (code == ERROR_NOT_FOUND) {
      return FAIL_NONFATAL;
    } else {
      *errStr = getErrorMessage(code);
      return FAIL_ERROR;
    }
  }

  *password = std::string(reinterpret_cast<char*>(creds[0]->CredentialBlob),
                          creds[0]->CredentialBlobSize);
  ::CredFree(creds);
  return SUCCESS;
}

}  // namespace keytar
