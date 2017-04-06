#include "keytar.h"

#include <libsecret/secret.h>
#include <stdio.h>

namespace keytar {

namespace {

static const SecretSchema schema = {
  "org.freedesktop.Secret.Generic", SECRET_SCHEMA_NONE, {
    { "service", SECRET_SCHEMA_ATTRIBUTE_STRING },
    { "account", SECRET_SCHEMA_ATTRIBUTE_STRING }
  }
};

}  // namespace

bool AddPassword(const std::string& service,
                 const std::string& account,
                 const std::string& password,
                 std::string* error) {
  GError* error = NULL;

  gboolean result = secret_password_store_sync(
    &schema,                            // The schema.
    SECRET_COLLECTION_DEFAULT,          // Default collection.
    (service + "/" + account).c_str(),  // The label.
    password.c_str(),                   // The password.
    NULL,                               // Cancellable. (unneeded)
    &error,                             // Reference to the error.
    "service", service.c_str(),
    "account", account.c_str(),
    NULL);                              // End of arguments.

  if (error != NULL) {
    g_error_free(error);
    return false;
  }

  return result;
}

bool GetPassword(const std::string& service,
                 const std::string& account,
                 std::string* password,
                 std::string* error) {
  GError* error = NULL;

  gchar* raw_password = secret_password_lookup_sync(
    &schema,                            // The schema.
    NULL,                               // Cancellable. (unneeded)
    &error,                             // Reference to the error.
    "service", service.c_str(),
    "account", account.c_str(),
    NULL);                              // End of arguments.

  if (error != NULL) {
    g_error_free(error);
    return false;
  }

  if (raw_password == NULL)
    return false;

  *password = raw_password;
  secret_password_free(raw_password);
  return true;
}

bool DeletePassword(const std::string& service,
                    const std::string& account,
                    std::string* error) {
  GError* error = NULL;

  gboolean result = secret_password_clear_sync(
    &schema,                            // The schema.
    NULL,                               // Cancellable. (unneeded)
    &error,                             // Reference to the error.
    "service", service.c_str(),
    "account", account.c_str(),
    NULL);                              // End of arguments.

  if (error != NULL) {
    g_error_free(error);
    return false;
  }

  return result;
}

bool FindPassword(const std::string& service,
                  std::string* password,
                  std::string* error) {
  GError* error = NULL;

  gchar* raw_password = secret_password_lookup_sync(
    &schema,                            // The schema.
    NULL,                               // Cancellable. (unneeded)
    &error,                             // Reference to the error.
    "service", service.c_str(),
    NULL);                              // End of arguments.

  if (error != NULL) {
    g_error_free(error);
    return false;
  }

  if (raw_password == NULL)
    return false;

  *password = raw_password;
  secret_password_free(raw_password);
  return true;
}

}  // namespace keytar
