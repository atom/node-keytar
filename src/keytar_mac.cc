#include "keytar.h"

#include <Security/Security.h>

namespace keytar {

const std::string errorStatusToString(OSStatus status) {
  std::string errorStr;
  CFStringRef errorMessageString = SecCopyErrorMessageString(status, NULL);

  const char* errorCStringPtr = CFStringGetCStringPtr(errorMessageString,
                                                      kCFStringEncodingUTF8);
  if (errorCStringPtr) {
    errorStr = std::string(errorCStringPtr);
  } else {
    errorStr = std::string("An unknown error occurred.");
  }

  CFRelease(errorMessageString);
  return errorStr;
}

KEYTAR_OP_RESULT SetPassword(const std::string& service,
                             const std::string& account,
                             const std::string& password,
                             std::string* error) {
  OSStatus status = SecKeychainAddGenericPassword(NULL,
                                                  service.length(),
                                                  service.data(),
                                                  account.length(),
                                                  account.data(),
                                                  password.length(),
                                                  password.data(),
                                                  NULL);
  if (status == errSecDuplicateItem) {
    // This password already exists.
    return FAIL_NONFATAL;
  } else if (status != errSecSuccess) {
    *error = errorStatusToString(status);
    return FAIL_ERROR;
  }

  return SUCCESS;
}

KEYTAR_OP_RESULT GetPassword(const std::string& service,
                             const std::string& account,
                             std::string* password,
                             std::string* error) {
  void *data;
  UInt32 length;
  OSStatus status = SecKeychainFindGenericPassword(NULL,
                                                   service.length(),
                                                   service.data(),
                                                   account.length(),
                                                   account.data(),
                                                   &length,
                                                   &data,
                                                   NULL);

  if (status == errSecItemNotFound) {
    return FAIL_NONFATAL;
  } else if (status != errSecSuccess) {
    *error = errorStatusToString(status);
    return FAIL_ERROR;
  }

  *password = std::string(reinterpret_cast<const char*>(data), length);
  SecKeychainItemFreeContent(NULL, data);
  return SUCCESS;
}

KEYTAR_OP_RESULT DeletePassword(const std::string& service,
                                const std::string& account,
                                std::string* error) {
  SecKeychainItemRef item;
  OSStatus status = SecKeychainFindGenericPassword(NULL,
                                                   service.length(),
                                                   service.data(),
                                                   account.length(),
                                                   account.data(),
                                                   NULL,
                                                   NULL,
                                                   &item);
  if (status == errSecItemNotFound) {
    // Item could not be found, so already deleted.
    return FAIL_NONFATAL;
  } else if (status != errSecSuccess) {
    *error = errorStatusToString(status);
    return FAIL_ERROR;
  }

  status = SecKeychainItemDelete(item);
  CFRelease(item);
  if (status != errSecSuccess) {
    *error = errorStatusToString(status);
    return FAIL_ERROR;
  }

  return SUCCESS;
}

KEYTAR_OP_RESULT FindPassword(const std::string& service,
                              std::string* password,
                              std::string* error) {
  SecKeychainItemRef item;
  void *data;
  UInt32 length;

  OSStatus status = SecKeychainFindGenericPassword(NULL,
                                                   service.length(),
                                                   service.data(),
                                                   0,
                                                   NULL,
                                                   &length,
                                                   &data,
                                                   &item);
  if (status == errSecItemNotFound) {
    return FAIL_NONFATAL;
  } else if (status != errSecSuccess) {
    *error = errorStatusToString(status);
    return FAIL_ERROR;
  }

  *password = std::string(reinterpret_cast<const char*>(data), length);
  SecKeychainItemFreeContent(NULL, data);
  CFRelease(item);
  return SUCCESS;
}

}  // namespace keytar
