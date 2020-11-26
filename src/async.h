#ifndef SRC_ASYNC_H_
#define SRC_ASYNC_H_

#include <string>
#include "napi.h"

#include "credentials.h"

class SetPasswordWorker : public Napi::AsyncWorker {
  public:
    SetPasswordWorker(const std::string& service, const std::string& account, const std::string& password,
                      const Napi::Env &env);

    ~SetPasswordWorker();

    void Execute() override;
    void OnOK() override;
    void OnError(Napi::Error const &error) override;
    Napi::Promise Promise();

  private:
    const std::string service;
    const std::string account;
    const std::string password;
    Napi::Promise::Deferred deferred;
};

class GetPasswordWorker : public Napi::AsyncWorker {
  public:
    GetPasswordWorker(const std::string& service, const std::string& account,
                      const Napi::Env &env);

    ~GetPasswordWorker();

    void Execute() override;
    void OnOK() override;
    void OnError(Napi::Error const &error) override;
    Napi::Promise Promise();

  private:
    const std::string service;
    const std::string account;
    std::string password;
    bool success = false;
    const Napi::Promise::Deferred deferred;
};

class DeletePasswordWorker : public Napi::AsyncWorker {
  public:
    DeletePasswordWorker(const std::string& service, const std::string& account,
                         const Napi::Env &env);

    ~DeletePasswordWorker();

    void Execute() override;
    void OnOK() override;
    void OnError(Napi::Error const &error) override;
    Napi::Promise Promise();

  private:
    const std::string service;
    const std::string account;
    bool success = false;
    Napi::Promise::Deferred deferred;
};

class FindPasswordWorker : public Napi::AsyncWorker {
  public:
    FindPasswordWorker(const std::string& service, const Napi::Env &env);

    ~FindPasswordWorker();

    void Execute() override;
    void OnOK() override;
    void OnError(Napi::Error const &error) override;
    Napi::Promise Promise();

  private:
    const std::string service;
    std::string password;
    bool success = false;
    const Napi::Promise::Deferred deferred;
};

class FindCredentialsWorker : public Napi::AsyncWorker {
  public:
    FindCredentialsWorker(const std::string& service, const Napi::Env &env);

    ~FindCredentialsWorker();

    void Execute() override;
    void OnOK() override;
    void OnError(Napi::Error const &error) override;
    Napi::Promise Promise();

  private:
    const std::string service;
    std::vector<keytar::Credentials> credentials;
    bool success = false;
    const Napi::Promise::Deferred deferred;
};

#endif  // SRC_ASYNC_H_
