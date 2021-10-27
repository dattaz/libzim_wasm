#include <zim/archive.h>
#include <zim/item.h>
#include <iostream>
#include <chrono>
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>

using namespace emscripten;

int main(int argc, char* argv[])
{
    std::cout << "wasm initialized" << std::endl;
    return 0;
}

// Get article count of a ZIM file (with kiwix-lib)
unsigned int getArticleCount(std::string filename) {
    zim::Archive a(filename);
    return a.getArticleCount();
}

// Get ArticleContent by URL (with libzim)
std::string getArticleContentByUrl(std::string filename, std::string url) {
    zim::Archive a(filename);
    zim::Entry entry = a.getEntryByPath(url);
    zim::Item item = entry.getItem(true);
    return item.getData();
}

// Binding code
EMSCRIPTEN_BINDINGS(kiwix_module) {
    emscripten::function("getArticleContentByUrl", &getArticleContentByUrl);
    emscripten::function("getArticleCount", &getArticleCount);
}
