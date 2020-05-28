#include "main_cache.hpp"
#include <math.h>
#include <vector>

MainCache::MainCache(const CacheProperty &setting) : BaseCache(setting) {
    property._num_block = (setting._cache_size << 10) / setting._block_size;

    switch (property.associativity) {
    case full_associative:
        /* For fully associative, remaining bits are used for TAG*/
        property._bit_index = 0;
        property._bit_set = 0;
        property._bit_tag = 32 - property._bit_offset;
        break;
    case direct_mapped:
        property._bit_index = log2l(property._num_block);
        property._bit_set = 0;
        property._bit_tag = 32 - property._bit_offset - property._bit_index;
        break;
    case set_associative:
        property._bit_index = 0;
        property._num_way = setting._num_way;
        property._num_set = property._num_block / property._num_way;
        property._bit_set = log2l(property._num_set);
        property._bit_tag = 32 - property._bit_offset - property._bit_set;
        break;
    }
}

MainCache::~MainCache() = default;

bool MainCache::Get(const addr_t &addr) {
    bool res = IsHit(addr);
    if (res) {
        _HitHandle(addr);
    }
    return res;
}

bool MainCache::Set(const addr_t &addr) {
    _Replace(addr);
    _HitHandle(addr);
    return true;
}

bool MainCache::IsHit(const addr_t &addr) {
    ulint idx(0);
    bool identical(true);

    auto check_ident = [](const ulint &idx, const uint &_bit_tag,
                          const addr_t &lhs, const addr_t rhs[]) -> bool {
        for (uint j = 31, k = 28; j > (31 - _bit_tag); j--, k--) {
            if (lhs[j] != rhs[idx][k]) {
                return false;
            }
        }
        return true;
    };

    switch (property.associativity) {
    case full_associative:
        // Fully-Associative: search all cache block
        for (idx = 0; idx < property._num_block; idx++) {
            if (_cache[idx][30]) {
                identical = check_ident(idx, property._bit_tag, addr, _cache);
                if (identical == true) {
                    return true;
                }
            }
        }
        break;

    case direct_mapped:
        // Directed Mapped: check the only one corresponding block
        idx = (addr.to_ulong() >> property._bit_offset) % property._num_block;
        if (_cache[idx][30]) {
            return check_ident(idx, property._bit_tag, addr, _cache);
        }
        break;

    case set_associative:
        // Set Associative: Get set, then check blocks among the set
        ulint _set_num = _GetSetNumber(addr);
        for (idx = _set_num * property._num_way;
             idx < (_set_num + 1) * property._num_way; idx++) {
            if (_cache[idx][30]) {
                identical = check_ident(idx, property._bit_tag, addr, _cache);
                if (identical == true) {
                    return true;
                }
            }
        }
        break;
    }

    return false;
}

void MainCache::_Replace(const addr_t &addr) {
    ulint idx = _GetCacheBlockIndex(addr);

    for (uint j = 31, k = 28; j > (31 - property._bit_tag); j--, k--) {
        _cache[idx][k] = addr[j];
    }
    _cache[idx][30] = true;
}

// active whenever read
void MainCache::_HitHandle([[maybe_unused]] const addr_t &addr) {
    // # TODO
    // ulint cache_tag=0;
    // ulint addr_tag=0;

    //
    ulint idx(0);
    bool identical(true);

    auto check_ident = [](const ulint &idx, const uint &_bit_tag,
                          const addr_t &lhs, const addr_t rhs[]) -> bool {
        for (uint j = 31, k = 28; j > (31 - _bit_tag); j--, k--) {
            if (lhs[j] != rhs[idx][k]) {
                return false;
            }
        }
        return true;
    };
    //

    ulint cache_set(0);

    switch (property.associativity) {
    case full_associative:

        for (idx = 0; idx < property._num_block; idx++) {
            if (_cache[idx][30]) {
                identical = check_ident(idx, property._bit_tag, addr, _cache);
                if (identical == true) {
                    break;
                }
            }
        }

        for (std::vector<ulint>::iterator it = _LRU_priority.begin();
             it != _LRU_priority.end(); it++) {
            if (*it == idx) {
                _LRU_priority.erase(it);
                break;
            }
        }
        _LRU_priority.insert(_LRU_priority.begin(), idx);

        break;

    case set_associative:
        cache_set = _GetSetNumber(addr);
        for (idx = 0; idx < property._num_way; idx++) {
            if (_cache[property._num_way * cache_set + idx][30]) {
                identical = check_ident(property._num_way * cache_set + idx,
                                        property._bit_tag, addr, _cache);
                if (identical == true) {
                    break;
                }
            }
        }

        // for(std::vector<ulint>::iterator it_2 = _LRU_priority.begin() +
        // 4*cache_set ; it_2 != _LRU_priority.begin()+ 4 * cache_set + 4;
        // it_2++){
        //     if(*it_2==4*cache_set+idx) {
        //         _LRU_priority.erase(it_2);
        //         break;
        //     }
        // }

        for (std::vector<ulint>::iterator it_2 = _LRU_priority.begin();
             it_2 != _LRU_priority.end(); it_2++) {
            if (*it_2 == property._num_way * cache_set + idx) {
                _LRU_priority.erase(it_2);
                break;
            }
        }
        _LRU_priority.insert(_LRU_priority.begin(),
                             property._num_way * cache_set + idx);

        break;
    default:
        break;
    }
}

ulint MainCache::_GetCacheBlockIndex(const addr_t &addr) {
    ulint res(0);
    if (property.associativity == direct_mapped)
        res = (addr.to_ulong() >> property._bit_offset) % property._num_block;
    else {
        switch (property.replacement_policy) {
        case RANDOM:
            res = _GetIndexByRandom(addr);
            break;
        case LRU:
            res = _GetIndexByLRU(addr);
            break;
        default:
            std::cerr << "Invalid replacement policy" << std::endl;
            break;
        }
    }

    return res;
}

ulint MainCache::_GetIndexByRandom(const addr_t &addr) {
    std::random_device rd;
    std::mt19937_64 generator(rd());
    std::uniform_int_distribution<int> unif(0, INT32_MAX);
    ulint res(0);
    switch (property.associativity) {
    case direct_mapped:
        // nothing to do
        break;

    case full_associative:
        res = static_cast<ulint>(unif(generator) /
                                 (INT32_MAX / property._num_block + 1));
        break;

    case set_associative:
        ulint temp = static_cast<ulint>(unif(generator) /
                                        (INT32_MAX / property._num_way + 1));
        ulint _set_num = _GetSetNumber(addr);
        res = _set_num * property._num_way + temp;
        break;
    }

    return res;
}

// output the position of cache to be replace by addr(new data)
ulint MainCache::_GetIndexByLRU([[maybe_unused]] const addr_t &addr) {
    ulint res(0);
    ulint cache_set;
    int set_empty = 0;
    ulint temp_set_data;

    // # TODO

    switch (property.associativity) {
    case full_associative:

        if (_LRU_priority.size() != property._num_block) {
            for (ulint k = 0; k < property._num_block; k++) {
                if (_cache[k][30] == false) {
                    res = k;

                    break;
                }
            }
        } else
            res = _LRU_priority.back();
        break;

    case set_associative:
        cache_set = _GetSetNumber(addr);

        // empty or not
        for (ulint i = 0; i < property._num_way; i++) {
            if (_cache[property._num_way * cache_set + i][30] == 0) {
                set_empty = 1;
                res = property._num_way * cache_set + i;
                break;
            }
            set_empty = 0;
        }

        if (set_empty == 0) {
            for (std::vector<ulint>::iterator it = _LRU_priority.begin();
                 it != _LRU_priority.end(); it++) {
                if ((*it / property._num_way) == cache_set) {
                    for (uint j = 0; j < 4; j++) {
                        if (*it % property._num_way == j) {
                            temp_set_data = cache_set * property._num_way + j;
                            break;
                        }
                    }
                }
            }
            res = temp_set_data;
        }

        break;

    default:
        break;
    }
    return res;
}

ulint MainCache::_GetSetNumber(const addr_t &addr) {
    std::bitset<28> _set_num;
    for (ulint i = (property._bit_offset), j = 0;
         i < (property._bit_offset + property._bit_set); ++i, ++j) {
        _set_num[j] = addr[i];
    }
    return _set_num.to_ulong();
}
