#pragma once

#if defined(__GNUC__) || defined(__clang__)
#   define NODISCARD __attribute__((warn_unused_result))
#elif (__STDC_VERSION__ >= 202311L)
#   define NODISCARD [[nodiscard]]
#else
#   define NODISCARD
#endif

