#pragma once

#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)

#define OK 0
#define USAGE "./ft_vox <seed>"

#define ERROR "Error: "

#define WRONG_NUMBER_ARGUMENT_ID 1
#define WRONG_NUMBER_ARGUMENT_STR "Wrong number of arguments"

#define SEED_MIN_CHAR 6
#define SEED_MAX_CHAR 12
#define BASE_SEED_ERROR "Its length must be between " STRINGIFY(SEED_MIN_CHAR) " and " STRINGIFY(SEED_MAX_CHAR) " characters"
#define SEED_TO_SMALL_ID 2
#define SEED_TO_SMALL_STR "The seed is too small. " BASE_SEED_ERROR
#define SEED_TO_BIG_ID 3
#define SEED_TO_BIG_STR "The seed is too long. " BASE_SEED_ERROR