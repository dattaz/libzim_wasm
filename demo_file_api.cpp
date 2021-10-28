#include <zim/archive.h>
#include <zim/item.h>
#include <zim/error.h>
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

class ContentWithMimetype{
public:
    ContentWithMimetype(std::vector<char> _content, std::string _mimetype) {
        content = _content;
        mimetype = _mimetype;
    }

    std::vector<char> getContent() const { return content; }
    std::string getMimetype() const { return mimetype; }

private:
    std::vector<char> content;
    std::string mimetype;
};

// Get content (and MIME type) by URL
ContentWithMimetype getContentByUrl(std::string url) {
    //try {
        zim::Entry entry = g_archive->getEntryByPath(url);
        zim::Item item = entry.getItem(true);
        auto blob = item.getData();
        std::cout << "size of extracted content : " << blob.size() << std::endl;
        std::vector<char> content = std::vector<char>(blob.data(), blob.data()+blob.size());
        std::string mimetype = item.getMimetype();
        return ContentWithMimetype(content, mimetype);
    //} catch(zim::EntryNotFound& e) {
    //    return "Entry not found";
    //} catch(std::exception& e) {
    //    return std::string("Other exception : ") + e.what();
    //}
}

// Binding code
EMSCRIPTEN_BINDINGS(libzim_module) {
    emscripten::function("loadArchive", &loadArchive);
    emscripten::function("getContentByUrl", &getContentByUrl);
    emscripten::function("getArticleCount", &getArticleCount);
    emscripten::register_vector<char>("vector<char>");
    class_<ContentWithMimetype>("ContentWithMimetype")
      .constructor<std::vector<char>, std::string>()
      .property("content", &ContentWithMimetype::getContent)
      .property("mimetype", &ContentWithMimetype::getMimetype)
      ;
}
