#include "keytar.h"

#include <libsecret/secret.h>
#include <stdio.h>

namespace keytar {

const SecretSchema schema = {
    "org.gnome.keyring.NodeKeyTar",
    SECRET_SCHEMA_NONE,
    {   { "service", SECRET_SCHEMA_ATTRIBUTE_STRING },
        { "account", SECRET_SCHEMA_ATTRIBUTE_STRING },
        { "NULL", SECRET_SCHEMA_ATTRIBUTE_STRING }
    }
};

bool AddPassword(const std::string& service,
                 const std::string& account,
                 const std::string& password) {
    GError* error = NULL;

    gboolean result = secret_password_store_sync(
                          &schema,
                          NULL,  // Default keyring.
                          (service + "/" + account).c_str(),  // Display name.
                          password.c_str(),
                          NULL,
                          &error,
                          "service", service.c_str(),
                          "account", account.c_str(),
                          NULL);

    if (error != NULL) {
        g_error_free(error);
        return false;
    }

    return result == TRUE;
}

bool GetPassword(const std::string& service,
                 const std::string& account,
                 std::string* password) {
    GError* error = NULL;

    gchar* raw_password = secret_password_lookup_sync(
                              &schema,
                              NULL,
                              &error,
                              "service", service.c_str(),
                              "account", account.c_str(),
                              NULL);

    if (error != NULL) {
        g_error_free(error);
        return false;
    }

    if (raw_password != NULL) {
        password->assign(raw_password);
        secret_password_free(raw_password);
        return true;
    }
    return false;
}

bool DeletePassword(const std::string& service, const std::string& account) {
    GError* error = NULL;

    gboolean result = secret_password_clear_sync(
                          &schema,
                          NULL,
                          &error,
                          "service", service.c_str(),
                          "account", account.c_str(),
                          NULL);

    if (error != NULL) {
        g_error_free(error);
        return false;
    }

    return result == TRUE;
}

bool FindPassword(const std::string& service, std::string* password) {
    GError* error = NULL;

    gchar* raw_password = secret_password_lookup_sync(
                              &schema,
                              NULL,
                              &error,
                              "service", service.c_str(),
                              NULL);

    if (error != NULL) {
        g_error_free(error);
        return false;
    }

    if (raw_password != NULL) {
        password->assign(raw_password);
        secret_password_free(raw_password);
        return true;
    }
    return false;
}

}  // namespace keytar
