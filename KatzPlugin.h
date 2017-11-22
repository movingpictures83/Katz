#ifndef KATZPLUGIN_H
#define KATZPLUGIN_H

#include "Plugin.h"
#include "PluginProxy.h"
#include "csv_parser/csv_parser.cpp"
#include <string>
#include <vector>

class KatzPlugin : public Plugin 
{
  public:    
  std::string toString(){return "Katz";}
  void input(std::string file);
  void run();
  void output(std::string file);

  private:
     float* OrigGraph;
     std::string* bacteria;//[GSIZE];
     int GSIZE;
     std::vector<float> U;   

};


#endif
