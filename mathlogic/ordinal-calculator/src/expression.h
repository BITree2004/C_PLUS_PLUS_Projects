//
// Created by bitree on 08.01.24.
//
#ifndef CPP_SOLUTION_EXPRESSION_H
#define CPP_SOLUTION_EXPRESSION_H


#include <cstdint>
#include <cassert>
#include <limits>
#include <ostream>
#include <vector>

using namespace std;

namespace expression {
    using _type = uint64_t;

    class expression;

    struct list {
        vector<pair<list, _type>> arr;
        bool is_end{false}; // means just 0, in exp(...^)

        list() : is_end(true) {}

        list(bool is_end) : is_end(is_end) {}

        list(list &&other) : arr() {
            arr.swap(other.arr);
            swap(is_end, other.is_end);
        };

        list(list const &other) : arr(other.arr), is_end(other.is_end) {};

        list &operator=(list const &other) = default;

        list(initializer_list<pair<list, _type>> x) : arr(x), is_end(false) {}

        int compare(list const &other) {
            // -1 <
            // 0 =
            // 1 >
            if (other.is_it_final()) {
                return (is_it_final() ? 0 : 1);
            } else if (is_it_final()) {
                return -1;
            }
            for (size_t i = 0; i < arr.size(); ++i) {
                if (i == other.arr.size()) {
                    return 1;
                }
                int res = arr[i].first.compare(other.arr[i].first);
                if (res != 0) {
                    return res;
                } else if (arr[i].second < other.arr[i].second) {
                    return -1;
                } else if (arr[i].second > other.arr[i].second) {
                    return 1;
                }
            }
            if (other.arr.size() > arr.size()) {
                return -1;
            }
            return 0;
        }

        bool is_it_final() const {
            return is_end;
        }

        bool is_zero() const {
            return is_it_final() || (arr.size() == 1 && arr[0].first.is_it_final() && arr[0].second == 0);
        }

        bool is_it_limited() {
            return arr.size() > 1 || !arr[0].first.is_it_final();
        }

        expression *to_expression();
    };

    class expression {
    public:
        virtual list to_normal() = 0;

        virtual void print(ostream &os) = 0;

        virtual ~expression() {};
    };

    class number : public expression {
    public:
        _type value;

        number(const string &value) : value(stoull(value)) {}

        number(const _type &value) : value(value) {}

        list to_normal() override {
            return list{make_pair(list(), value)};
        }

        void print(ostream &os) override {
            os << value;
        }

        static bool is_number(pair<list, _type> const &obj) {
            return obj.first.is_it_final();
        }
    };


    class limit_ordinal : public expression {
    public:
        limit_ordinal() = default;

        list to_normal() override {
            return list{make_pair(list{make_pair(list(), 1)}, 1)};
        }

        void print(ostream &os) override {
            os << 'w';
        }
    };


    class operation : public expression {
    protected:
        expression *left;
        expression *right;

        virtual char get_char() = 0;

    public:
        operation(expression *left, expression *right) : left(left), right(right) {}

        void print(ostream &os) {
            os << '(';
            left->print(os);
            os << ')';
            os << get_char();
            os << "(";
            right->print(os);
            os << ')';
        }
    };

    class add : public operation {
    public:
        using operation::operation;

        list to_normal() override {
            return to_simplify(left->to_normal(), right->to_normal());
        }

        static list to_simplify(list left_list, list right_list) {
            if (left_list.is_it_final()) { // 0 + x = x
                return right_list;
            } else if (right_list.is_it_final()) { // x + 0 = x
                return left_list;
            }
            while (!left_list.arr.empty()) {
                list cmp1{left_list.arr.back().first};
                int res = cmp1.compare(right_list.arr[0].first);
                if (res == -1) {
                    left_list.arr.pop_back();
                } else if (res == 1) {
                    break;
                } else {
                    right_list.arr[0].second += left_list.arr.back().second;
                    left_list.arr.pop_back();
                }
            }
            for (auto &x: right_list.arr) {
                left_list.arr.push_back(x);
            }
            return left_list;
        }

        char get_char() override {
            return '+';
        }

        ~add() {
            delete left;
            delete right;
        }
    };

    class multiply : public operation {
    public:
        using operation::operation;

        list to_normal() override {
            return to_simplify(left->to_normal(), right->to_normal());
        }

        static list to_simplify(list left_list, list right_list) {
            if (left_list.is_zero() || right_list.is_zero()) {
                return list{make_pair(list(), 0)};
            }
            if (!right_list.is_it_limited()) {
                for (auto &[x, y]: left_list.arr) {
                    y *= right_list.arr[0].second;
                }
                return left_list;
            }
            list res(false);
            for (auto &x: right_list.arr) {
                if (!number::is_number(x)) {
                    res.arr.push_back(make_pair(add::to_simplify(left_list.arr[0].first, x.first), x.second));
                } else {
                    list res2 = to_simplify(left_list, list{right_list.arr.back()});
                    res.arr.insert(res.arr.end(), res2.arr.begin(), res2.arr.end());
                }
            }
            return res;
        }

        char get_char() override {
            return '*';
        }

        ~multiply() {
            delete left;
            delete right;
        }
    };

    class pow : public operation {
    public:
        using operation::operation;

        list to_normal() override {
            return to_simplify(left->to_normal(), right->to_normal());
        }

        static list exp_simplify_helper(list right_list) {
            if (right_list.arr.size() > 1) {
                list res = exp_simplify_helper(list{right_list.arr[0]});
                for (size_t i = 1; i < right_list.arr.size(); ++i) {
                    to_simplify(res, pow::to_simplify(list{make_pair(list{make_pair(list(), 1)}, 1)},
                                                      list{right_list.arr[i]}));
                }
                return res;
            }
            if (right_list.arr[0].first.is_it_final() && right_list.arr[0].second == 1) {
                return list{make_pair(list{make_pair(list(), 1)}, 1)};
            }
            list res{exp_simplify_helper(list{make_pair(right_list.arr[0].first, 1)})};
            for (size_t i = 0; i < right_list.arr[0].second - 1; ++i) {
                res = pow::to_simplify(
                        res,
                        list{make_pair(list{make_pair(list(), 1)}, 1)}
                );
            }
            return res;
        }

        static list to_simplify(list left_list, list right_list) {
            if (right_list.arr.size() > 1) {
                list res{make_pair(list(), 1)};
                for (size_t i = 0; i < right_list.arr.size(); ++i) {
                    res = multiply::to_simplify(res, pow::to_simplify(left_list, list{right_list.arr[i]}));
                    //distributivity
                }
                return res;
            } else if (!left_list.arr[0].first.is_it_final() && right_list.is_it_limited()) {
                return list{make_pair(multiply::to_simplify(left_list.arr[0].first, right_list), 1)};
            } else if (right_list.is_it_limited()) {
                if (left_list.arr[0].second <= 1) {
                    return list{make_pair(list(), left_list.arr[0].second)};
                } else {
                    return pow::to_simplify(
                            exp_simplify_helper(right_list.arr[0].first),
                            list{make_pair(list(), right_list.arr[0].second)}
                    ); // 4 axiom
                }
            }
            list res{make_pair(list(), 1)};
            for (size_t i = 0; i < right_list.arr[0].second; ++i) {
                res = multiply::to_simplify(res, left_list);
            }
            return res;
        }

        char get_char()
        override {
            return '^';
        }

        ~pow() {
            delete left;
            delete right;
        }
    };

}
#endif //CPP_SOLUTION_EXPRESSION_H