#include <benchmark/benchmark.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <string_view>
#include <system_error>

namespace {

// you must use the provided syscalls for IO
class Logger {
private:
  std::string _buffer;

  void _write(const std::string& buffer, bool is_check = true) {
    if (::write(fd, buffer.data(), buffer.size()) != static_cast<ssize_t>(buffer.size()) &&
        is_check) {
      throw std::system_error(errno, std::system_category(), "write");
    }
  }

  bool _is_heavy_text(const std::string& buffer) {
    return buffer.size() > MAX_BUFFER_SIZE;
  }

public:
  explicit Logger(std::string_view path) : _buffer("") {
    const int MODE_BITS = O_CREAT | O_APPEND | O_WRONLY | O_TRUNC;
    const int PROTECTION_BITS = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    fd = ::open(path.data(), MODE_BITS, PROTECTION_BITS);
    if (fd == -1) {
      throw std::system_error(errno, std::system_category(), "open");
    }
  }

  ~Logger() {
    _write(_buffer, false);
    ::close(fd);
  }

  void write(const std::string& msg) {
    if (_is_heavy_text(msg)) {
      _write(msg);
    } else {
      _buffer += msg;
      if (_is_heavy_text(_buffer)) {
        _write(_buffer);
        _buffer = "";
      }
    }
  }

private:
  int fd = -1;
  static constexpr size_t MAX_BUFFER_SIZE = 1'000'000;
};

void bm_logger(benchmark::State& state) {
  static Logger logger("/tmp/benchmark_logger");

  for (auto _ : state) {
    logger.write("test message\n");
  }
}

} // namespace

BENCHMARK(bm_logger)->Threads(1);

BENCHMARK_MAIN();
