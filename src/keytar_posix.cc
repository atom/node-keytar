#include "keytar.h"
#include "keytar_posix.h"

#include <gnome-keyring.h>

#ifdef KEYTAR_KWALLET_BACKEND
#include <QString>


namespace keytar_posix {

// Keyring detection algorithm derived from code in:
//  https://github.com/frankosterfeld/qtkeychain/blob/master/keychain_unix.cpp
//
// This is in turn derived from the chromium detection algorithm:
//  see base/nix/xdg_util.cc

enum keyring_backend_t {
  NO_BACKEND,
  KWALLET_BACKEND,
  GNOME_KEYRING_BACKEND
};

enum desktop_environment_t {
  OTHER_DESKTOP,
  PLASMA5_DESKTOP,
  KDE4_DESKTOP,
  GNOME_DESKTOP,
  UNITY_DESKTOP,
  XFCE_DESKTOP,
};


static desktop_environment_t getKdeVersion() {
  QString s = qgetenv("KDE_SESSION_VERSION");
  if ( s == "5" ) {
    return PLASMA5_DESKTOP;
  } else if ( s == "4" ) {
    return KDE4_DESKTOP;
  } else {
    // most likely KDE3
    return OTHER_DESKTOP;
  }
}


static desktop_environment_t getDesktopEnvironment() {
  QByteArray s = qgetenv("XDG_CURRENT_DESKTOP");
  if ( s == "GNOME" ) {
    return GNOME_DESKTOP;
  } else if ( s == "Unity" ) {
    return UNITY_DESKTOP;
  } else if ( s == "KDE" ) {
    return getKdeVersion();
  }

  s = qgetenv("DESKTOP_SESSION");
  if ( s == "gnome" ) {
    return GNOME_DESKTOP;
  } else if ( s == "kde" ) {
    return getKdeVersion();
  } else if ( s == "kde4" ) {
    return KDE4_DESKTOP;
  } else if ( s.contains("xfce") || s == "xubuntu" ) {
    return XFCE_DESKTOP;
  }

  if ( !qgetenv("GNOME_DESKTOP_SESSION_ID").isEmpty() ) {
    return GNOME_DESKTOP;
  } else if ( !qgetenv("KDE_FULL_SESSION").isEmpty() ) {
    return getKdeVersion();
  }

  return OTHER_DESKTOP;
}


static keyring_backend_t getKeyringBackend() {
  switch (getDesktopEnvironment()) {
    case KDE4_DESKTOP:
      return NO_BACKEND;
      break;
    case PLASMA5_DESKTOP:
      return KWALLET_BACKEND;
      break;
    case GNOME_DESKTOP:
    case UNITY_DESKTOP:
    case XFCE_DESKTOP:
    case OTHER_DESKTOP:
    default:
      if ( gnome_keyring_is_available() ) {
        return GNOME_KEYRING_BACKEND;
      } else {
        return NO_BACKEND;
      }
      break;
  }
}
}


namespace keytar {

// Identify the most appropriate backend to use
keytar_posix::keyring_backend_t backend = keytar_posix::getKeyringBackend();


/**
 *  Add a new password
 *
 *  @param  service   The folder the account will be added to.
 *  @param  account   The account name.
 *  @param  password  Password for the account.
 *  @return           True or false indicating whether the function was successful.
 */
bool AddPassword(const std::string& service,
                 const std::string& account,
                 const std::string& password) {
  switch (backend) {
    case keytar_posix::KWALLET_BACKEND:
      return KAddPassword(service, account, password);
    case keytar_posix::GNOME_KEYRING_BACKEND:
      return GAddPassword(service, account, password);
    case keytar_posix::NO_BACKEND:
    default:
      return false;
  }
}


/**
 *  Get a stored password
 *
 *  @param  service   The folder the account will be added to.
 *  @param  account   The account name.
 *  @param  password  Password for the account (this will be written into).
 *  @return           True or false indicating whether the function was successful.
 */
bool GetPassword(const std::string& service,
                 const std::string& account,
                 std::string* password) {
  switch (backend) {
  case keytar_posix::KWALLET_BACKEND:
    return KGetPassword(service, account, password);
  case keytar_posix::GNOME_KEYRING_BACKEND:
    return GGetPassword(service, account, password);
  case keytar_posix::NO_BACKEND:
  default:
    return false;
  }
}


/**
 *  Delete a stored password
 *
 *  @param  service   The folder the account will be added to.
 *  @param  account   The account name.
 *  @return           True or false indicating whether the function was successful.
 */
bool DeletePassword(const std::string& service, const std::string& account) {
  switch (backend) {
  case keytar_posix::KWALLET_BACKEND:
    return KDeletePassword(service, account);
  case keytar_posix::GNOME_KEYRING_BACKEND:
    return GDeletePassword(service, account);
  case keytar_posix::NO_BACKEND:
  default:
    return false;
  }
}


/**
 *  Get a stored password
 *
 *  @param  service   The folder the account will be added to.
 *  @param  password  Password for the account (this will be written into).
 *  @return           True or false indicating whether the function was successful.
 *
 *  This function will return the password associated with the alphabetically sorted
 *  first account in the folder.
 */
bool FindPassword(const std::string& service, std::string* password) {
  switch (backend) {
  case keytar_posix::KWALLET_BACKEND:
    return KFindPassword(service, password);
  case keytar_posix::GNOME_KEYRING_BACKEND:
    return GFindPassword(service, password);
  case keytar_posix::NO_BACKEND:
  default:
    return false;
  }
}

}  // namespace keytar

#else
// If KEYTAR_KWALLET_BACKEND not defined, just pass through to gnome-keyring
// functions
namespace keytar {

bool AddPassword(const std::string& service,
                 const std::string& account,
                 const std::string& password) {
  return GAddPassword(service, account, password);
}

bool GetPassword(const std::string& service,
                 const std::string& account,
                 std::string* password) {
  return GGetPassword(service, account, password);
}

bool DeletePassword(const std::string& service, const std::string& account) {
  return GDeletePassword(service, account);
}

bool FindPassword(const std::string& service, std::string* password) {
  return GFindPassword(service, password);
}

}  // namespace keytar

#endif  // KEYTAR_KWALLET_BACKEND
