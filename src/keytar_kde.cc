#include "keytar.h"
#include "keytar_posix.h"

#include <QString>
#include <kwindowsystem.h>
#include <kwallet.h>


namespace keytar {

/**
 *  Add a new password
 *
 *  @param  service   The folder the account will be added to.
 *  @param  account   The account name.
 *  @param  password  Password for the account.
 *  @return           True or false indicating whether the function was successful.
 */
bool KAddPassword(const std::string& service,
                  const std::string& account,
                  const std::string& password) {
  KWallet::Wallet *wallet;

  if ((wallet = KWallet::Wallet::openWallet(KWallet::Wallet::LocalWallet(),
                                            KWindowSystem::activeWindow()))
      && wallet->createFolder(QString(service.c_str()))
      && wallet->setFolder(QString(service.c_str()))
      && !wallet->writePassword(QString(account.c_str()),
                                QString(password.c_str()))) {
      // Takes advantage of short-circuit evaluation
      // writePassword returns 0 on success therefore needs ! to convert to true
    return true;
  }

  return false;
}


/**
 *  Get a stored password
 *
 *  @param  service   The folder the account will be added to.
 *  @param  account   The account name.
 *  @param  password  Password for the account (this will be written into).
 *  @return           True or false indicating whether the function was successful.
 */
bool KGetPassword(const std::string& service,
                  const std::string& account,
                  std::string* password) {
  KWallet::Wallet *wallet;
  QString raw_password = NULL;

  if ((wallet = KWallet::Wallet::openWallet(KWallet::Wallet::LocalWallet(),
                                            KWindowSystem::activeWindow()))
      && wallet->hasFolder(QString(service.c_str()))
      && wallet->setFolder(QString(service.c_str()))
      && !wallet->readPassword(QString(account.c_str()), raw_password)) {
      // Takes advantage of short-circuit evaluation
      // readPassword returns 0 on success therefore needs ! to convert to true
    if (!raw_password.isNull()) {
      *password = raw_password.toStdString();
      raw_password.clear();
      return true;
    }
  }

  return false;
}


/**
 *  Delete a stored password
 *
 *  @param  service   The folder the account will be added to.
 *  @param  account   The account name.
 *  @return           True or false indicating whether the function was successful.
 */
bool KDeletePassword(const std::string& service, const std::string& account) {
  KWallet::Wallet *wallet;

  if ((wallet = KWallet::Wallet::openWallet(KWallet::Wallet::LocalWallet(),
                                            KWindowSystem::activeWindow()))
      && wallet->hasFolder(QString(service.c_str()))
      && wallet->setFolder(QString(service.c_str()))
      && !wallet->removeEntry(QString(account.c_str()))) {
      // Takes advantage of short-circuit evaluation
      // removeEntry returns 0 on success therefore needs ! to convert to true
    if ((wallet->entryList()).isEmpty()) {
      // If we deleted the last entry in the folder, remove the folder entirely
      wallet->removeFolder(QString(service.c_str()));
    }
    return true;
  }

  return false;
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
bool KFindPassword(const std::string& service, std::string* password) {
  KWallet::Wallet *wallet;
  QString first_entry;
  QString raw_password;
  QStringList entry_list;

  if ((wallet = KWallet::Wallet::openWallet(KWallet::Wallet::LocalWallet(),
                                            KWindowSystem::activeWindow()))
      && wallet->hasFolder(QString(service.c_str()))
      && wallet->setFolder(QString(service.c_str()))) {
      // Takes advantage of short-circuit evaluation
    entry_list = wallet->entryList();
    if (!entry_list.isEmpty()) {
      entry_list.sort();
      first_entry = entry_list.first();
      if (!wallet->readPassword(first_entry, raw_password)) {
        // readPassword returns 0 on success therefore needs ! to convert to
        // true
        *password = raw_password.toStdString();
        raw_password.clear();
        return true;
      }
    }
  }

  return false;
}

}  // namespace keytar
