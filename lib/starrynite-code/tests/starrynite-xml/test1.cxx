#include <iostream>
#include <memory>

#include "frame.hxx"

//using namespace std;
//using namespace starrynite;
//using namespace xml_schema;
//using namespace xercesc;

int
main(int argc, char* argv[])
{
  try
    {
      starrynite::frame f(1, 2, 3, 4);
      xml_schema::namespace_infomap map;
      map[""].name = "http://starrynite.sf.net/ns/1";
      // map[""].schema = "frame.xsd";
      starrynite::frame_(std::cout, f, map);
    }
  catch (const xml_schema::exception& e)
    {
      std::cerr << e << std::endl;
      return 1;
    }
  return 0;
}
