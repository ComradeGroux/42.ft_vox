#include "utils.hpp"

/**
 * FNV-1a algorythm
 *
 * I choose this algorythm because it is know to be efficient
 * and particularly simple
 *
 */
uint64_t	hashSeed(const std::string& seed)
{
	uint64_t hash = 14695981039346656037ULL;	// FNV offset basis

	for (char c : seed)
	{
		hash ^= static_cast<uint64_t>(c);
		hash *= 1099511628211ULL;				// FNV prime
	}

	return hash;
}
