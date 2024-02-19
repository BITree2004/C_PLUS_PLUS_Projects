# Optional

В этом задании вам необходимо
написать [`optional`](https://en.cppreference.com/w/cpp/utility/optional/optional).

Интерфейс вы найдете в репозитории, в файле `optional.h`. Поведение должно
соответствовать `std::optional`.

Обратите внимание, что:

- `optional` должен стремиться повторять наличия и тривиальности special
  member'ов (формальные требования описаны на cppreference);
- `optional` может использоваться в constexpr контексте, см. тесты со
  `static_assert`.
