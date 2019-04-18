#include <zim/file.h>
#include <zim/fileiterator.h>
#include <reader.h>
#include <iostream>
#include <chrono>
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>

using namespace emscripten;

// This wrapper is a workaround for the fact that the Reader class does not have any default constructor with no parameter
class KiwixReaderWrapper
{
  public:
    KiwixReaderWrapper()
    {
    }

    kiwix::Reader *_reader;
};

// This singleton instance is initialized once, and kept for subsequent function calls
KiwixReaderWrapper readerWrapper;

int main(int argc, char* argv[])
{
    std::cout << "wasm initialized" << std::endl;
    return 0;
}

// Get ArticleCount by URL (with libzim)
std::string getArticleContentByUrl(std::string filename, std::string url) {
    zim::File f(filename);    
    zim::Article article = f.getArticleByUrl(url);
    return article.getData(0).data();
}

// Get article count of a ZIM file (with kiwix-lib)
unsigned int getArticleCount(std::string filename) {
    kiwix::Reader reader(filename);
    unsigned int articleCount = reader.getArticleCount();
    return articleCount;
}

// Get article count of a ZIM file (with kiwix-lib, using the ReaderWrapper singleton)
unsigned int getArticleCountFromReader() {
    return readerWrapper._reader->getArticleCount();
}

// Initialize the ReaderWrapper singleton
void initReader(std::string filename) {
    kiwix::Reader reader(filename);
    readerWrapper._reader = &reader;
}

// Get a Kiwix Entry from its URL (with kiwix-lib, using the ReaderWrapper singleton)
kiwix::Entry getEntryFromPath(std::string url){
    return readerWrapper._reader->getEntryFromPath(url);
}

// Binding code
EMSCRIPTEN_BINDINGS(kiwix_module) {
    emscripten::function("getArticleContentByUrl", &getArticleContentByUrl);
    emscripten::function("getArticleCount", &getArticleCount);
    emscripten::function("initReader", &initReader);
    emscripten::function("getArticleCountFromReader", &getArticleCountFromReader);
    emscripten::function("getEntryFromPath", &getEntryFromPath);
}
