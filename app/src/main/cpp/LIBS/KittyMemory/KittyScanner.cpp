#include "KittyScanner.h"

#include "KittyMemory.h"
#include "KittyUtils.h"

using KittyMemory::ProcMap;

// refs
// https://github.com/learn-more/findpattern-bench

namespace KittyScanner
{

    bool compare(const char *data, const char *pattern, const char *mask)
    {
        for (; *mask; ++mask, ++data, ++pattern)
        {
            if (*mask == 'x' && *data != *pattern)
                return false;
        }
        return !*mask;
    }

    uintptr_t find(const uintptr_t start, uintptr_t end,
                   const char *pattern, const char *mask)
    {
        if (start >= end)
            return 0;

        const size_t mask_size = strlen(mask);
        const size_t size = end - start;

        for (size_t i = 0; i < size; ++i)
        {
            const uintptr_t current_end = start + i + mask_size;
            if (current_end > end)
                break;

            if (!compare(reinterpret_cast<const char *>(start + i), pattern, mask))
                continue;

            return start + i;
        }
        return 0;
    }

    std::vector<uintptr_t> findBytesAll(const KittyMemory::ProcMap &map,
                                        const char *pattern, const char *mask)
    {
        std::vector<uintptr_t> list;

        if (!map.isValid())
            return list;

        uintptr_t curr_search_address = map.startAddress;
        size_t size = strlen(mask);
        do {
            if (!list.empty()) curr_search_address = list.back() + size;
            
            uintptr_t found = find(curr_search_address, map.endAddress, pattern, mask);
            if (!found) break;

            list.push_back(found);
        } while (true);

        return list;
    }

    uintptr_t findBytesFirst(const KittyMemory::ProcMap &map, const char *pattern, const char *mask)
    {
        if (!map.isValid() || !pattern || !mask || map.length < strlen(mask))
            return 0;

        return find(map.startAddress, map.endAddress, pattern, mask);
    }


    std::vector<uintptr_t> findHexAll(const KittyMemory::ProcMap& map, std::string hex, const char *mask)
	{
        std::vector<uintptr_t> list;
        
        if (!map.isValid() || !mask || !KittyUtils::validateHexString(hex)) return list;

        size_t size = strlen(mask);
        if((hex.length() / 2) != size) return list;
        
        std::vector<char> pattern(size);
        KittyUtils::fromHex(hex, &pattern[0]);

        list = findBytesAll(map, pattern.data(), mask);
        return list;
    }

    uintptr_t findHexFirst(const KittyMemory::ProcMap& map, std::string hex, const char *mask)
	{        
        if (!map.isValid() || !mask || !KittyUtils::validateHexString(hex)) return 0;

        size_t size = strlen(mask);
        if((hex.length() / 2) != size) return 0;
        
        std::vector<char> pattern(size);
        KittyUtils::fromHex(hex, &pattern[0]);

        return findBytesFirst(map, pattern.data(), mask);
    }


    std::vector<uintptr_t> findDataAll(const KittyMemory::ProcMap &map, const void *data, size_t size)
    {
        std::vector<uintptr_t> list;

        if (!map.isValid())
            return list;

        std::string mask(size, 'x');
        uintptr_t curr_search_address = map.startAddress;
        do {
            if (!list.empty()) curr_search_address = list.back() + size;

            uintptr_t found = find(curr_search_address, map.endAddress, (const char *)data, mask.c_str());
            if (!found) break;

            list.push_back(found);
        } while (true);

        return list;
    }

    uintptr_t findDataFirst(const KittyMemory::ProcMap &map, const void *data, size_t size)
    {
        if (!map.isValid())
            return 0;

        std::string mask(size, 'x');

        return find(map.startAddress, map.endAddress, (const char *)data, mask.c_str());
    }

    RegisterNativeFn findRegisterNativeFn(const std::vector<KittyMemory::ProcMap> &maps, const std::string &name)
    {
        uintptr_t string_loc = 0, string_xref = 0, fn_loc = 0;
        RegisterNativeFn fn = { nullptr, nullptr, nullptr };

        if (name.empty() || maps.empty())
            return fn;
        
        for (auto &it : maps)  {
            if (it.is_rx) {
                string_loc = KittyScanner::findDataFirst(it, name.data(), name.length());
                if (string_loc) break;
            }
        }

        if (!string_loc) {
            KITTY_LOGE("couldn't find string (%s) in selected maps", name.c_str());
            return fn;
        }

        KITTY_LOGI("string (%s) at %p", name.c_str(), (void*)string_loc);

        for (auto &it : maps) {
            if (it.is_rw) {
                string_xref = KittyScanner::findDataFirst(it, &string_loc, sizeof(uintptr_t));
                if (!string_xref) continue;

                KITTY_LOGI("string at (%p) referenced at %p", (void *)string_loc, (void *)string_xref);
                
                fn_loc = string_xref;
            }
        }

        if(!fn_loc) return fn;

        memcpy(&fn, (void *)fn_loc, sizeof(RegisterNativeFn));
        return fn;
    }

}