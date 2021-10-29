#include <zim/archive.h>
#include <zim/item.h>
#include <zim/error.h>
#include <zim/search.h>
#include <iostream>
#include <chrono>
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>

using namespace emscripten;

std::shared_ptr<zim::Archive> g_archive;

int main(int argc, char* argv[])
{
    std::cout << "wasm initialized" << std::endl;
    return 0;
}

void loadArchive(std::string filename) {
    g_archive.reset(new zim::Archive(filename));
    std::cout << "archive loaded" << std::endl;
}

// Get article count of a ZIM file
unsigned int getArticleCount() {
    return g_archive->getArticleCount();
}

class ItemWrapper{
public:
    ItemWrapper(zim::Item item):
        m_item(item)
    { }

    std::vector<char> getContent() const {
      auto blob = m_item.getData();
      std::vector<char> content = std::vector<char>(blob.data(), blob.data()+blob.size());
      return content;
    }
    std::string getMimetype() const { return m_item.getMimetype(); }

private:
    zim::Item m_item;
};

class EntryWrapper{
public:
    EntryWrapper(zim::Entry entry):
        m_entry(entry)
    { }

    ItemWrapper getItem(bool follow) {
        return ItemWrapper(m_entry.getItem(follow));
    }
    std::string getPath() {
        return m_entry.getPath();
    }
    uint32_t getIndex() {
        return m_entry.getIndex();
    }
    bool isRedirect() {
        return m_entry.isRedirect();
    }
    EntryWrapper getRedirectEntry() {
        return EntryWrapper(m_entry.getRedirectEntry());
    }

private:
    zim::Entry m_entry;
};

// Get an entry by its path
EntryWrapper getEntryByPath(std::string url) {
    //try {
        zim::Entry entry = g_archive->getEntryByPath(url);
        return EntryWrapper(entry);
    //} catch(zim::EntryNotFound& e) {
    //    return "Entry not found";
    //} catch(std::exception& e) {
    //    return std::string("Other exception : ") + e.what();
    //}
}

// Get an entry by its title index
EntryWrapper getEntryByTitleIndex(uint32_t idx) {
    //try {
        zim::Entry entry = g_archive->getEntryByTitle(idx);
        return EntryWrapper(entry);
    //} catch(zim::EntryNotFound& e) {
    //    return "Entry not found";
    //} catch(std::exception& e) {
    //    return std::string("Other exception : ") + e.what();
    //}
}

// Search for a text, and returns the path of the first result
std::vector<EntryWrapper> search(std::string text) {
    auto searcher = zim::Searcher(*g_archive);
    auto query = zim::Query(text);
    auto search = searcher.search(query);
    auto searchResultSet = search.getResults(0,50);
    std::vector<EntryWrapper> ret;
    for(auto entry:searchResultSet) {
        ret.push_back(EntryWrapper(entry));
    }
    return ret;
}

// Binding code
EMSCRIPTEN_BINDINGS(libzim_module) {
    emscripten::function("loadArchive", &loadArchive);
    emscripten::function("getEntryByPath", &getEntryByPath);
    emscripten::function("getEntryByTitleIndex", &getEntryByTitleIndex);
    emscripten::function("getArticleCount", &getArticleCount);
    emscripten::function("search", &search);
    emscripten::register_vector<char>("vector<char>");
    emscripten::register_vector<EntryWrapper>("vector(EntryWrapper)");
    class_<EntryWrapper>("EntryWrapper")
      .function("getItem", &EntryWrapper::getItem)
      .function("getPath", &EntryWrapper::getPath)
      .function("isRedirect", &EntryWrapper::isRedirect)
      .function("getRedirectEntry", &EntryWrapper::getRedirectEntry)
      ;
    class_<ItemWrapper>("ItemWrapper")
      .function("getContent", &ItemWrapper::getContent)
      .function("getMimetype", &ItemWrapper::getMimetype)
      ;
}
