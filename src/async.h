#ifndef SRC_ASYNC_H_
#define SRC_ASYNC_H_

#include <string>

class AddPasswordWorker : public Nan::AsyncWorker {
  public:
    AddPasswordWorker(const std::string& service, const std::string& account, const std::string& password,
                      Nan::Callback* callback);

    ~AddPasswordWorker();

    void Execute();
    void HandleOKCallback();

  private:
    const std::string service;
    const std::string account;
    const std::string password;
    bool success;
};

class GetPasswordWorker : public Nan::AsyncWorker {
  public:
    GetPasswordWorker(const std::string& service, const std::string& account, Nan::Callback* callback);

    ~GetPasswordWorker();

    void Execute();
    void HandleOKCallback();

  private:
    const std::string service;
    const std::string account;
    std::string password;
    bool success;
};

class DeletePasswordWorker : public Nan::AsyncWorker {
  public:
    DeletePasswordWorker(const std::string& service, const std::string& account, Nan::Callback* callback);

    ~DeletePasswordWorker();

    void Execute();
    void HandleOKCallback();

  private:
    const std::string service;
    const std::string account;
    bool success;
};

class FindPasswordWorker : public Nan::AsyncWorker {
  public:
    FindPasswordWorker(const std::string& service, Nan::Callback* callback);

    ~FindPasswordWorker();

    void Execute();
    void HandleOKCallback();

  private:
    const std::string service;
    std::string password;
    bool success;
};

#endif  // SRC_ASYNC_H_
