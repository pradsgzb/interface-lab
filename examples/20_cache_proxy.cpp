#include "studydesk/example.hpp"
#include <map>

namespace ex20 {
// A proxy and its source satisfy the same read contract.
struct ICourseCatalog {
    virtual ~ICourseCatalog() = default;
    virtual std::string title(int id) = 0;
};
class RemoteCatalogDemo final : public ICourseCatalog {
public:
    std::string title(int id) override {
        ++calls;
        return id == 101 ? "Data structures" : "Unknown course";
    }
    int calls = 0; // Simulates a costly call; no network request is made.
};
class CachedCatalog final : public ICourseCatalog {
public:
    explicit CachedCatalog(ICourseCatalog& source) : source_(source) {}
    std::string title(int id) override {
        const auto found = cache_.find(id);
        if (found != cache_.end()) {
            return found->second;
        }
        const auto value = source_.title(id);
        cache_[id] = value;
        return value;
    }
    void clear() {
        cache_.clear();
    } // Deliberately explicit invalidation for this lesson.
private:
    ICourseCatalog& source_;
    std::map<int, std::string> cache_;
};
void run(const lab::Context&) {
    RemoteCatalogDemo remote;
    CachedCatalog cache(remote);
    ICourseCatalog& catalog = cache;
    lab::check(catalog.title(101) == "Data structures", "First read fetches the course");
    lab::check(catalog.title(101) == "Data structures" && remote.calls == 1,
               "Second read uses the cache");
    cache.clear();
    catalog.title(101);
    lab::check(remote.calls == 2, "Invalidation permits a fresh fetch");
    lab::lesson("Proxy pattern: mediate access behind the same interface; consider stale data.");
}
} // namespace ex20
