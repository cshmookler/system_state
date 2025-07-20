#pragma once

#define ASSERT_NOT_NULL(arg)                                                   \
    if (arg == NULL) {                                                         \
        return strdup(                                                         \
          RES_NEW_ERROR("A required argument is null.").string().c_str());     \
    }

#define ASSERT_SUCCESS(call)                                                   \
    auto result = call;                                                        \
    if (result.failure()) {                                                    \
        return strdup(result.error().string().c_str());                        \
    }

#define ASSERT_HAS_VALUE(result)                                               \
    if (result.has_error()) {                                                  \
        return strdup(result.error().string().c_str());                        \
    }

#define ASSERT_HAS_INDEX(list, index)                                          \
    if (index >= list->size()) {                                               \
        return strdup(RES_NEW_ERROR("Index out of range.").string().c_str());  \
    }
