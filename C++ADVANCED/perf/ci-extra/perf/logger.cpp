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
public:
  explicit Logger(std::string_view path) {
    const int MODE_BITS = O_CREAT | O_APPEND | O_WRONLY | O_TRUNC;
    const int PROTECTION_BITS = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    fd = ::open(path.data(), MODE_BITS, PROTECTION_BITS);
    if (fd == -1) {
      throw std::system_error(errno, std::system_category(), "open");
    }
  }

  ~Logger() {
    ::close(fd);
  }

  void write(const std::string& msg) {
    if (::write(fd, msg.data(), msg.size()) != static_cast<ssize_t>(msg.size())) {
      throw std::system_error(errno, std::system_category(), "write");
    }
  }

private:
  int fd = -1;
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
