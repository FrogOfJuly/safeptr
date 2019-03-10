//
// Created by Kirill on 2019-02-28.
//

#ifndef UTILS_DEFER_H
#define UTILS_DEFER_H

#include <functional>
#define CANCAT_IMP(x, y) x##y
#define MACRO_CANCAT(x, y) CANCAT_IMP(x, y)


namespace utils {
    class defer {
        std::function<void(void)> responsibility;
    public:
        defer();

        defer(const defer &) = delete;

        defer(defer &&) noexcept;

        defer &operator=(const defer &) = delete;

        ~defer();

        explicit defer(std::function<void(void)> instruction);
    };
}

#define DEFER(lambda_body) utils::defer MACRO_CANCAT(derfer_, __COUNTER__)([&]()->void {lambda_body;});
#define L_DEFER(lambda) utils::defer MACRO_CANCAT(derfer_, __COUNTER__)(lambda);


#endif //UTILS_DEFER_H
