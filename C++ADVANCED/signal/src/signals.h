#pragma once
#include "intrusive-list.h"

#include <functional>

namespace signals {

template <typename T>
struct signal;

template <typename... Args>
struct signal<void(Args...)> {
public:
  struct connection;

private:
  struct connection_tag;
  struct smart_iterator_tag;
  struct smart_iterator;
  using slot = std::function<void(Args...)>;
  using slots = intrusive::list<connection, connection_tag>;
  using stack = intrusive::list<smart_iterator, smart_iterator_tag>;
  using iterator = typename slots::const_iterator;

public:
  struct connection : public intrusive::list_element<connection_tag> {
  public:
    connection() = default;

    connection(const connection& con) = delete;

    connection(connection&& con) {
      move_ctor(std::move(con));
    }

    connection& operator=(const connection& con) = delete;

    connection& operator=(connection&& con) {
      if (this != &con) {
        disconnect();
        move_ctor(std::move(con));
      }
      return *this;
    }

    void disconnect() noexcept {
      if (sig) {
        for (auto& v : sig->current) {
          if (&*v.it == this) {
            ++v.it;
          }
        }
        this->unlink();
        func = slot();
        sig = nullptr;
      }
    }

    ~connection() {
      disconnect();
    }

  private:
    connection(signal* sig, slot func) noexcept : sig(sig), func(std::move(func)) {
      sig->connections.push_back(*this);
    }

    void move_ctor(connection&& con) {
      sig = con.sig;
      func = std::move(con.func);
      if (sig) {
        sig->connections.insert(++sig->connections.get_iterator(con), *this);
        con.disconnect();
      }
    }

    friend struct signal;
    signal* sig{nullptr};
    slot func;
  };

  signal() = default;

  signal(const signal&) = delete;
  signal& operator=(const signal&) = delete;

  ~signal() {
    while (!connections.empty()) {
      connections.front().disconnect();
    }
  }

  connection connect(std::function<void(Args...)> func) noexcept {
    return connection(this, std::move(func));
  }

  void operator()(Args... args) const {
    smart_iterator cur(connections.begin());
    iterator end = connections.end();
    current.push_back(cur);
    while (cur.it != end) {
      iterator copy = cur.it++;
      copy->func(std::forward<Args>(args)...);
    }
  }

private:
  struct smart_iterator : public intrusive::list_element<smart_iterator_tag> {
  public:
    explicit smart_iterator(iterator it) : it(it) {}

    using intrusive::list_element<smart_iterator_tag>::is_linked;

    ~smart_iterator() {
      this->unlink();
    }

    iterator it;
  };

  mutable stack current;
  slots connections;
};

} // namespace signals
