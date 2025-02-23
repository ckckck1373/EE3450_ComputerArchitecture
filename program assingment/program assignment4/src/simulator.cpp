#include "simulator.hpp"

Simulator::Simulator(std::vector<CacheProperty> &cache_cfg_list,
                     const std::string &program_trace,
                     const bool &multi_level_mode = false)
    : _multi_level(multi_level_mode), trace_file(program_trace) {

    _SetupCache(cache_cfg_list);

    inst_loader = std::make_unique<InstructionLoader>(trace_file);
}

Simulator::~Simulator() = default;

void Simulator::_SetupCache(const std::vector<CacheProperty> &_cfg_list) {
    if (_multi_level) {
        for (uint i = 0; i < _cfg_list.size(); i++) {
            _cache_hierarchy_list.push_back(MainCache(_cfg_list[i]));
        }
    } else {

        _cache_hierarchy_list.push_back(MainCache(_cfg_list[0]));
    }
}

void Simulator::RunSimulation() {
    while (inst_loader->IfAvailable()) {
        try {
            bool is_success = _CacheHandler(inst_loader->GetNextInst());
            if (!is_success) {
                throw std::logic_error("Cache Handler failed");
            }
        } catch (const std::exception &ex) {
            std::cerr << ex.what() << std::endl;
            exit(-1);
        }
    }
    _CalHitRate();
}

bool Simulator::_CacheHandler(const inst_t &inst) {
    addr_t next_addr = Cvt2AddrBits(inst.addr_raw);

    // Determine what kind of the instruction
    switch (inst.op) {
    case I_LOAD:
        _Load(next_addr);
        break;
    case I_STORE:
        _Store(next_addr);
        break;
    case I_NONE:
        ++_counter.space;
        break;
    default:
        std::cerr << "Unexpected error in _CacheHandler()" << std::endl;
        std::cerr << "ERROR line: " << inst.addr_raw << std::endl;
        return false;
    }
    return true;
}

bool Simulator::_Access(const addr_t &addr) {
    bool is_hit(false);
    if (_multi_level) {
        // TODO
        is_hit = _cache_hierarchy_list[0].Get(addr);
        if (is_hit)
            return is_hit;
        else {
            _cache_hierarchy_list[0].Set(addr);
            is_hit = _cache_hierarchy_list[1].Get(addr);
            if (is_hit)
                return is_hit;
            else
                _cache_hierarchy_list[1].Set(addr);
        }

    } else {

        is_hit = _cache_hierarchy_list[0].Get(addr);

        if (!is_hit) {
            _cache_hierarchy_list[0].Set(addr);
        }
    }

    return is_hit;
}

void Simulator::_Load(const addr_t &addr) {
    ++_counter.access;
    ++_counter.load;

    if (_Access(addr)) {
        ++_counter.load_hit;
    }
}

void Simulator::_Store(const addr_t &addr) {
    ++_counter.access;
    ++_counter.store;

    if (_Access(addr)) {
        ++_counter.store_hit;
    }
}

void Simulator::DumpResult(const bool &oneline) {
    if (oneline) {
        std::cout << std::setprecision(6) << _counter.avg_hit_rate << std::endl;
    } else {
        std::cout << "========================================" << std::endl;
        std::cout << "Test file: " << this->trace_file << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        _ShowSettingInfo();
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Number of cache access: " << _counter.access << std::endl;
        std::cout << "Number of cache load: " << _counter.load << std::endl;
        std::cout << "Number of cache store: " << _counter.store << std::endl;
        std::cout << "Number of total cache hit: " << _counter.hit << std::endl;
        std::cout << "Cache hit rate: " << std::setprecision(6)
                  << _counter.avg_hit_rate << std::endl;
        std::cout << "Average Memory Access Time: " << std::setprecision(4)
                  << _counter.amat << " cycles" << std::endl;
        std::cout << "========================================" << std::endl;
    }
}

void Simulator::_ShowSettingInfo() {
    for (std::size_t i = 0; i < _cache_hierarchy_list.size(); i++) {
        std::cout << "# L" << i + 1 << " Cache" << std::endl;
        _ShowSettingInfo(_cache_hierarchy_list[i]);
        if (i != _cache_hierarchy_list.size() - 1)
            std::cout << "---------------------------------------" << std::endl;
    }
}

void Simulator::_ShowSettingInfo(MainCache &_cache) {
    CacheProperty _property = _cache.GetProperty();

    std::cout << "Cache size: " << _property._cache_size << "KB" << std::endl;

    std::cout << "Cache block size: " << _property._block_size << "B"
              << std::endl;
    switch (_property.associativity) {
    case direct_mapped:
        std::cout << "Associativity: direct-mapped" << std::endl;
        break;
    case set_associative:
        std::cout << "Associativity: " << _property._num_way
                  << "-way set_associative" << std::endl;
        break;
    case full_associative:
        std::cout << "Associativity: full-associative" << std::endl;
        break;
    default:
        std::cerr << "Error associtivity setting" << std::endl;
        exit(-1);
    }
    switch (_property.replacement_policy) {
    case NONE:
        std::cout << "Replacement policy: None" << std::endl;
        break;
    case RANDOM:
        std::cout << "Replacement policy: Random" << std::endl;
        break;
    case LRU:
        std::cout << "Replacement policy: LRU" << std::endl;
        break;
    default:
        std::cerr << "Error replacement setting" << std::endl;
        exit(-1);
    }
}

void Simulator::_CalHitRate() {
    const int miss_penalty(100);
    assert(_counter.access != 0);
    assert(_counter.load != 0);
    assert(_counter.store != 0);
    _counter.hit = _counter.store_hit + _counter.load_hit;
    _counter.avg_hit_rate = static_cast<double>(_counter.hit) / _counter.access;
    _counter.load_hit_rate =
        static_cast<double>(_counter.load_hit) / _counter.load;
    _counter.store_hit_rate =
        static_cast<double>(_counter.store_hit) / _counter.store;
    _counter.amat = 1 + (1.0 - _counter.avg_hit_rate) * miss_penalty;
}
