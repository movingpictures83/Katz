#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include "PluginManager.h"
#include "KatzPlugin.h"

void KatzPlugin::input(std::string file) {

                const char field_terminator = ',';
                const char line_terminator  = '\n';
                const char enclosure_char   = '"';
   // File is in CSV format
  csv_parser file_parser;
                file_parser.set_skip_lines(1);
                file_parser.init(file.c_str());
                file_parser.set_enclosed_char(enclosure_char, ENCLOSURE_OPTIONAL);
                file_parser.set_field_term_char(field_terminator);
                file_parser.set_line_term_char(line_terminator);

                GSIZE = 0;
                while (file_parser.has_more_rows()) {
                   file_parser.get_row();
                   GSIZE++;
                }
                const int NumBytes=(GSIZE)*(GSIZE)*sizeof(float);
                //host allocations to create Adjancency matrix and result matrices with path matrices
                OrigGraph=(float *)malloc(NumBytes);//will be original Adjancency matrix, will NOT be changed
                bacteria = new std::string[GSIZE];
                file_parser.init(file.c_str());

                unsigned int row_count = 0;
                while(file_parser.has_more_rows())
                {
                        unsigned int i = 0;

                        csv_row row = file_parser.get_row();

                        bacteria[row_count] = row[0];

                        for (i = 1; i < row.size(); i++) {
                              int bac1 = row_count;
                              int bac2 = i-1;
                              float weight = atof(row[i].c_str());
                              if (bac1 != bac2) {
                                 if (weight != 0) {
                                    OrigGraph[bac1*GSIZE+bac2] = weight;
                                    OrigGraph[bac2*GSIZE+bac1] = weight;
                                 }
                                else {
                                    OrigGraph[bac1*GSIZE+bac2] = 0;
                                    OrigGraph[bac2*GSIZE+bac1] = 0;
                                }
                              }
                              else {
                                    OrigGraph[bac1*GSIZE+bac2] = 0;
                                    OrigGraph[bac2*GSIZE+bac1] = 0;
                              }
                        }
                        row_count++;
                }
  

}

void KatzPlugin::run() {
   U.resize(GSIZE, 0.0);

                for (int i = 0; i < GSIZE; i++)
                {
                   float pay = 0;
                   float totalpay = 0;
                   for (int j = 0; j < GSIZE; j++) {
                      pay +=  OrigGraph[i*GSIZE+j];
                      totalpay += fabs(OrigGraph[i*GSIZE+j]);
                   }
                   if (totalpay != 0)
                      U[i] = pay / totalpay;
                   else
                      U[i] = 0;
                }
}




void KatzPlugin::output(std::string file) {
for (int i = GSIZE-1; i >= 0; i--)
           for (int j = 0; j < i; j++) {
              if (fabs(U[j]) < fabs(U[j+1])) {
                 float tmp = U[j];
                 U[j] = U[j+1];
                 U[j+1] = tmp;
                 string tmp2 = bacteria[j];
                 bacteria[j] = bacteria[j+1];
                 bacteria[j+1] = tmp2;
              }
           }

        std::ofstream noafile(file.c_str(), std::ios::out);
        noafile << "Name\tCentrality\tRank" << endl;
        float min = 0;
        float max = 0;
        for (int i = 0; i < GSIZE; i++) {
           U[i] = fabs(U[i]);
           if (fabs(U[i]) > max)
              max = fabs(U[i]);
           if (fabs(U[i]) < min)
              min = fabs(U[i]);
           std::cout << bacteria[i] << ": " << U[i] << std::endl;
           noafile << bacteria[i] << "\t" << U[i] << "\t\t" << GSIZE-i << endl;
        }

}

PluginProxy<KatzPlugin> KatzPluginProxy = PluginProxy<KatzPlugin>("Katz", PluginManager::getInstance());

