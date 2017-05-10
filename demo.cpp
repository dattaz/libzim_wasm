#include <zim/file.h>
#include <zim/fileiterator.h>
#include <iostream>
int main(int argc, char* argv[]) 
{
  try
  {
    std::string filename = "meta.esperanto.stackexchange.com_eng_all_2017-05.zim";
    zim::File f(filename); 
    std::cout << "will print first 100 url/title of " << filename << std::endl;
    int i=0;
    for (zim::File::const_iterator it = f.begin(); it != f.end() && i<100; ++it)
    {
      std::cout << "url: " << it->getUrl() << " title: " << it->getTitle() << '\n';
      i++;
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}
