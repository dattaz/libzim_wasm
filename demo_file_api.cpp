#include <zim/file.h>
#include <zim/fileiterator.h>
#include <iostream>
int main(int argc, char* argv[])
{
  try
  {
    //std::string filename = "tmp.zim"; //"meta.esperanto.stackexchange.com_eng_all_2017-05.zim";
    std::string filename = argv[1];
    std::cout << "will print first 10 url/title of " << filename << std::endl;
    zim::File f(filename);
    std::cout << "file size : " << f.getFilesize() << std::endl;
    int i=0;
    for (zim::File::const_iterator it = f.begin(); it != f.end() && i<10; ++it)
    {
      std::cout << "url: " << it->getUrl() << " title: " << it->getTitle() << '\n';
      std::string articleUrl = it->getUrl();
      if (it->getNamespace() == 'A' && !it->isRedirect()) {
          zim::Article article = f.getArticleByUrl("A/" + articleUrl);
          std::cout << "article " << articleUrl << " size : " << article.getArticleSize() << '\n';
          std::cout << "beginning of article " << articleUrl << " : " << '\n';
          printf("%.*s\n", 30, article.getData(0).data());
          i++;
      }
    }
    std::string articleUrl = "A/Baby_Grand.html";
    
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}
