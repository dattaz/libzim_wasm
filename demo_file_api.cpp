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

// Get content by URL
std::vector<char> getContentByUrl(std::string url) {
    //try {
        zim::Entry entry = g_archive->getEntryByPath(url);
        zim::Item item = entry.getItem(true);
        auto blob = item.getData();
        std::cout << "size of extracted content : " << blob.size() << std::endl;
        return std::vector<char>(blob.data(), blob.data()+blob.size());
    //} catch(zim::EntryNotFound& e) {
    //    return "Entry not found";
    //} catch(std::exception& e) {
    //    return std::string("Other exception : ") + e.what();
    //}
}

// Get MIME type by URL
std::string getMimetypeByUrl(std::string url) {
    try {
        zim::Entry entry = g_archive->getEntryByPath(url);
        zim::Item item = entry.getItem(true);
        std::cout << "size declared by ZIM file : " << item.getSize() << std::endl;
        return item.getMimetype();
    } catch(zim::EntryNotFound& e) {
        return "Entry not found";
    } catch(std::exception& e) {
        return std::string("Other exception : ") + e.what();
    }
}

// Binding code
EMSCRIPTEN_BINDINGS(libzim_module) {
    emscripten::function("loadArchive", &loadArchive);
    emscripten::function("getContentByUrl", &getContentByUrl);
    emscripten::function("getMimetypeByUrl", &getMimetypeByUrl);
    emscripten::function("getArticleCount", &getArticleCount);
    emscripten::register_vector<char>("vector<char>");
}
