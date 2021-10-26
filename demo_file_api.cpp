#include <zim/file.h>
#include <zim/fileiterator.h>
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
    zim::File f(filename);
    return f.getCountArticles();
}

// Get ArticleContent by URL (with libzim)
std::string getArticleContentByUrl(std::string filename, std::string url) {
    zim::File f(filename);    
    zim::Article article = f.getArticleByUrl(url);
    return article.getData(0).data();
}

// Binding code
EMSCRIPTEN_BINDINGS(kiwix_module) {
    emscripten::function("getArticleContentByUrl", &getArticleContentByUrl);
    emscripten::function("getArticleCount", &getArticleCount);
}
