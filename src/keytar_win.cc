#include "keytar.h"

#include <windows.h>
#include <Shlobj.h>

#include <algorithm>
#include <vector>

namespace keytar {

namespace {

bool FileExists(const std::string& path) {
  DWORD attribute = ::GetFileAttributes(path.c_str());

  return (attribute != INVALID_FILE_ATTRIBUTES &&
         !(attribute & FILE_ATTRIBUTE_DIRECTORY));
}

bool WriteToFile(const std::string& path, const std::vector<char>& data) {
  HANDLE file = ::CreateFile(path.c_str(),
                             GENERIC_WRITE,
                             0,
                             NULL,
                             CREATE_ALWAYS,
                             FILE_ATTRIBUTE_NORMAL,
                             NULL);
  if (file == INVALID_HANDLE_VALUE)
    return false;

  DWORD written;
  bool r = ::WriteFile(file, data.data(), data.size(), &written, NULL) == TRUE;
  if (written != data.size())
    r = false;

  ::CloseHandle(file);
  return r;
}

bool ReadFromFile(const std::string& path, std::vector<char>* data) {
  HANDLE file = ::CreateFile(path.c_str(),
                             GENERIC_READ,
                             FILE_SHARE_READ,
                             NULL,
                             OPEN_ALWAYS,
                             FILE_ATTRIBUTE_NORMAL,
                             NULL);
  if (file == INVALID_HANDLE_VALUE)
    return false;

  DWORD size = ::GetFileSize(file, NULL);
  if (size == INVALID_FILE_SIZE) {
    ::CloseHandle(file);
    return false;
  }

  data->resize(size);
  DWORD read;
  bool r = ::ReadFile(file, data->data(), data->size(), &read, NULL) == TRUE;
  if (read != data->size())
    r = false;

  ::CloseHandle(file);
  return r;
}

bool CreatePath(const std::string& path) {
  if (!::CreateDirectory(path.c_str(), NULL) &&
      ::GetLastError() != ERROR_ALREADY_EXISTS)
    return false;

  return true;
}

bool GetPasswordPath(const std::string& service,
                     const std::string& account,
                     std::string* password_path) {
  char path[MAX_PATH] = { 0 };
  HRESULT r = ::SHGetFolderPath(NULL,
                                CSIDL_APPDATA | CSIDL_FLAG_CREATE,
                                NULL,
                                SHGFP_TYPE_CURRENT,
                                path);
  if (FAILED(r))
    return false;

  std::string keytar_path = std::string(path) + "\\node-keytar";
  if (!CreatePath(keytar_path))
    return false;

  std::string service_path = keytar_path + '\\' + service;
  if (!CreatePath(service_path))
    return false;

  *password_path = service_path + '\\' + account;
  return true;
}

}  // namespace

bool AddPassword(const std::string& service,
                 const std::string& account,
                 const std::string& password) {
  std::string password_path;
  if (!GetPasswordPath(service, account, &password_path))
    return false;

  if (FileExists(password_path))
    return false;

  DATA_BLOB data_in, data_out;
  data_in.pbData = (BYTE*)(password.c_str());  // NOLINT
  data_in.cbData = password.length();
  if (!::CryptProtectData(&data_in,
                          L"Password stored by node-keytar.",
                          NULL,
                          NULL,
                          NULL,
                          0,
                          &data_out))
    return false;

  std::vector<char> encrypted(data_out.cbData);
  std::copy(data_out.pbData, data_out.pbData + encrypted.size(),
            encrypted.begin());
  ::LocalFree(data_out.pbData);

  WriteToFile(password_path, encrypted);
  return true;
}

bool GetPassword(const std::string& service,
                 const std::string& account,
                 std::string* password) {
  std::string password_path;
  if (!GetPasswordPath(service, account, &password_path))
    return false;

  if (!FileExists(password_path))
    return false;

  std::vector<char> encrypted;
  if (!ReadFromFile(password_path, &encrypted))
    return false;

  DATA_BLOB data_in, data_out;
  data_in.pbData = reinterpret_cast<BYTE*>(encrypted.data());
  data_in.cbData = encrypted.size();
  if (!::CryptUnprotectData(&data_in,
                            NULL,
                            NULL,
                            NULL,
                            NULL,
                            0,
                            &data_out))
    return false;

  *password = std::string(reinterpret_cast<char*>(data_out.pbData),
                          data_out.cbData);
  ::LocalFree(data_out.pbData);
  return true;
}

bool DeletePassword(const std::string& service,
                    const std::string& account) {
  std::string password_path;
  if (!GetPasswordPath(service, account, &password_path))
    return false;

  if (!FileExists(password_path))
    return false;

  return ::DeleteFile(password_path.c_str()) == TRUE;
}

}  // namespace keytar
