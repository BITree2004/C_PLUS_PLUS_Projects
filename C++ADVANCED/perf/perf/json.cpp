#include <benchmark/benchmark.h>

#include <string>

// DO NOT change this struct
struct User {
  int64_t id;
  std::string username;
};

static const std::size_t USER_SIZE = 40;
static const std::size_t MESSAGE_SIZE = 180;

std::string to_json(const User& user) {
  std::string s;
  s.reserve(USER_SIZE);
  s += "{\"id\":";
  s += std::to_string(user.id);
  s += ",\"username\":\"";
  s += user.username;
  s += "\"}";
  return s;
}

// DO NOT change this struct
struct Message {
  int64_t id;
  std::string subject;
  std::string body;

  User from;
  User to;
};

// DO NOT change this function's signature
std::string to_json(const Message& msg) {
  std::string s;
  s.reserve(MESSAGE_SIZE);
  s += "{\"id\":";
  s += std::to_string(msg.id);
  s += ",\"subject\":\"";
  s += msg.subject;
  s += "\",\"body\":\"";
  s += msg.body;
  s += "\",\"from\":";
  s += to_json(msg.from);
  s += ",\"to\":";
  s += to_json(msg.to);
  s += "}";
  return s;
}

namespace {

void bm_message_to_json(benchmark::State& state) {
  Message msg{
      .id = 1000,
      .subject = "About modules",
      .body = "So, when is that 'modules' proposal coming?",
      .from = User{.id = 12345,      .username = "Herb Sutter"},
      .to = User{    .id = 1, .username = "Biern Stroustrup"}
  };

  for (auto _ : state) {
    benchmark::DoNotOptimize(to_json(msg));
  }

  auto json = to_json(msg);
  std::string expected =
      R"({"id":1000,"subject":"About modules","body":"So, when is that 'modules' proposal coming?","from":{"id":12345,"username":"Herb Sutter"},"to":{"id":1,"username":"Biern Stroustrup"}})";
  if (json != expected) {
    state.SkipWithError("Wrong output");
  }
}

} // namespace

BENCHMARK(bm_message_to_json);

BENCHMARK_MAIN();
