#include <cstdio>
#include <cstdlib>
#include <cstring>

static size_t prefix_function(const char* pattern, const char symbol, const size_t lastP, const size_t* p) {
  size_t res = lastP;
  while (res > 0 && symbol != pattern[res]) {
    res = p[res - 1];
  }
  if (symbol == pattern[res]) {
    res++;
  }
  return res;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Invalid number of arguments\nEnter name file and pattern(string)\n");
    return EXIT_FAILURE;
  }
  const char* pattern = argv[2];
  const size_t len_pattern = strlen(pattern);
  FILE* input = fopen(argv[1], "rb");
  if (input == nullptr) {
    perror("Opening file error, reason: ");
    return EXIT_FAILURE;
  }

  size_t* res_prefixfun = static_cast<size_t*>(malloc((len_pattern + 1) * sizeof(size_t)));
  if (res_prefixfun == nullptr) {
    fprintf(stderr, "insufficient memory\n");
    fclose(input);
    return EXIT_FAILURE;
  }
  res_prefixfun[0] = 0;
  for (size_t i = 1; i < len_pattern; i++) {
    res_prefixfun[i] = prefix_function(pattern, pattern[i], res_prefixfun[i - 1], res_prefixfun);
  }

  size_t last_result = 0;
  char readed_ch;
  size_t res_preffun_in_char;

  while ((readed_ch = fgetc(input)) != EOF) {
    res_preffun_in_char = prefix_function(pattern, readed_ch, last_result, res_prefixfun);
    if (res_preffun_in_char == len_pattern) {
      puts("Yes");
      free(res_prefixfun);
      fclose(input);
      return EXIT_SUCCESS;
    }
    last_result = res_preffun_in_char;
  }
  free(res_prefixfun);
  if (ferror(input)) {
    perror("Reading file error, reason: ");
    fclose(input);
    return EXIT_FAILURE;
  }
  fclose(input);
  puts("No");
  return EXIT_SUCCESS;
}
