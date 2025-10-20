/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <iostream>
#include "system.h"

using namespace std;

int main(int argc, char *argv[]) {

    int nconf = 1;
    System SYS;
    SYS.initialize();
    SYS.initialize_properties();
    SYS.block_reset(0);

    for (int i = 0; i < SYS.get_nbl(); i++) { // loop over blocks
        for (int j = 0; j < SYS.get_nsteps(); j++) { // loop over steps in a block
            SYS.step();
            SYS.measure();
            if (j % 50 == 0) {
                //        SYS.write_XYZ(nconf); //Write actual configuration in XYZ format //Commented to avoid "filesystem full"!
                nconf++;
            }
        }
        if (i%100 == 0 && SYS.get_nsteps() == 1){
            SYS.averages(i + 1, true, true);
        } else if (SYS.get_nsteps() == 1){
            SYS.averages(i + 1, false, false);
        } else {
            SYS.averages(i + 1, true, true);
        }
        SYS.block_reset(i + 1);
    }
    SYS.finalize();

    return 0;
}

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
