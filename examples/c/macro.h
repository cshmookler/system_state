#ifndef SYST_MACRO_H
#define SYST_MACRO_H

#define ASSERT_SUCCESS(call)                                                   \
    error = call;                                                              \
    if (error != NULL) {                                                       \
        int print_result = fprintf(stderr, "%s", error);                       \
        syst_string_free(error);                                               \
        if (print_result < 0) {                                                \
            return 2;                                                          \
        }                                                                      \
        return 1;                                                              \
    }                                                                          \
    syst_string_free(error)

#endif // SYST_MACRO_H
