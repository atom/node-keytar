#include "keytar.h"

#define UNICODE

#include <windows.h>
#include <wincred.h>

#include "credentials.h"

namespace keytar {

std::vector<wchar_t> utf8ToWideChar(const std::string& utf8) {
  int size = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), (int)utf8.size(), NULL, 0);
  std::vector<wchar_t> ret;
  ret.resize(size);
  MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), (int)utf8.size(), &ret[0], size);
  return ret;
}

std::string wideCharToAnsi(LPWSTR wide_char, int len) {
  //assert(wide_char == NULL);
  int size = WideCharToMultiByte(CP_ACP, 0, wide_char, len, NULL, 0, NULL, NULL);
  std::string ret;
  ret.resize(size);
  WideCharToMultiByte(CP_ACP, 0, wide_char, len, &ret[0], size, NULL, NULL);
  return ret;
}

std::string getErrorMessage(DWORD errorCode) {
  LPWSTR errBuffer;
  DWORD ret = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL, errorCode, 0, (LPWSTR) &errBuffer, 0, NULL);
  if (ret) {
    std::string errMsg = wideCharToAnsi(errBuffer, ret);
    LocalFree(errBuffer);
    return errMsg;
  }
  return "Unknown error";
}

KEYTAR_OP_RESULT SetPassword(const std::string& service,
                 const std::string& account,
                 const std::string& password,
                 std::string* errStr) {
  std::vector<wchar_t> target_uni = utf8ToWideChar(service + '/' + account);
  if (target_uni.empty()) {
    return FAIL_ERROR;
  }

  std::vector<wchar_t> account_uni = utf8ToWideChar(account);
  if (account_uni.empty()) {
    return FAIL_ERROR;
  }

  // might be empty
  std::vector<wchar_t> password_uni = utf8ToWideChar(password);
  DWORD password_bytes = password_uni.size() * sizeof(wchar_t); // password size in bytes

  CREDENTIAL cred = { 0 };
  cred.Type = CRED_TYPE_GENERIC;
  cred.TargetName = target_uni.data();
  cred.UserName = account_uni.data();
  cred.CredentialBlobSize = password_bytes;
  cred.CredentialBlob = (LPBYTE)(password_uni.data());
  cred.Persist = CRED_PERSIST_LOCAL_MACHINE;

  bool result = ::CredWrite(&cred, 0);
  if (!result) {
    *errStr = getErrorMessage(::GetLastError());
    return FAIL_ERROR;
  }
  return SUCCESS;
}

KEYTAR_OP_RESULT GetPassword(const std::string& service,
                 const std::string& account,
                 std::string* password,
                 std::string* errStr) {
  std::vector<wchar_t> target_uni = utf8ToWideChar(service + '/' + account);
  if (target_uni.empty()) {
    return FAIL_ERROR;
  }

  CREDENTIAL* cred;
  bool result = ::CredRead(target_uni, CRED_TYPE_GENERIC, 0, &cred);
  if (!result) {
    DWORD code = ::GetLastError();
    if (code == ERROR_NOT_FOUND) {
      return FAIL_NONFATAL;
    } else {
      *errStr = getErrorMessage(code);
      return FAIL_ERROR;
    }
  }

  *password = wideCharToAnsi((wchar_t*)cred->CredentialBlob, cred->CredentialBlobSize / sizeof(wchar_t));
  ::CredFree(cred);
  return SUCCESS;
}

KEYTAR_OP_RESULT DeletePassword(const std::string& service,
                    const std::string& account,
                    std::string* errStr) {
  std::vector<wchar_t> target_uni = utf8ToWideChar(service + '/' + account);
  if (target_uni.empty()) {
    return FAIL_ERROR;
  }

  bool result = ::CredDelete(target_uni, CRED_TYPE_GENERIC, 0);
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
  std::vector<wchar_t> filter_uni = utf8ToWideChar(service + "*");
  if (filter_uni.empty()) {
    return FAIL_ERROR;
  }

  DWORD count;
  CREDENTIAL** creds;
  bool result = ::CredEnumerate(filter_uni, 0, &count, &creds);
  if (!result) {
    DWORD code = ::GetLastError();
    if (code == ERROR_NOT_FOUND) {
      return FAIL_NONFATAL;
    } else {
      *errStr = getErrorMessage(code);
      return FAIL_ERROR;
    }
  }

  *password = wideCharToAnsi((wchar_t*)cred->CredentialBlob, cred->CredentialBlobSize / sizeof(wchar_t));
  ::CredFree(creds);
  return SUCCESS;
}

KEYTAR_OP_RESULT FindCredentials(const std::string& service,
                                 std::vector<Credentials>* credentials,
                                 std::string* errStr) {
  std::vector<wchar_t> filter_uni = utf8ToWideChar(service + "*");
  if (filter_uni.empty()) {
    return FAIL_ERROR;
  }

  DWORD count;
  CREDENTIAL **creds;
  bool result = ::CredEnumerate(filter_uni, 0, &count, &creds);
  if (!result) {
    DWORD code = ::GetLastError();
    if (code == ERROR_NOT_FOUND) {
      return FAIL_NONFATAL;
    } else {
      *errStr = getErrorMessage(code);
      return FAIL_ERROR;
    }
  }

  for (unsigned int i = 0; i < count; ++i) {
    CREDENTIAL* cred = creds[i];
    if (cred->UserName == NULL || cred->CredentialBlob == NULL) {
      continue;
    }

    std::string login = wideCharToAnsi(cred->UserName, -1);
    std::string password = wideCharToAnsi((wchar_t*)cred->CredentialBlob, cred->CredentialBlobSize / sizeof(wchar_t));
    credentials->emplace_back(std::make_pair(login, password));
  }

  CredFree(creds);
  return SUCCESS;
}

}  // namespace keytar
