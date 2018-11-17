/*
 * starrynite-dump.cxx
 *
 *  Created on: Apr 16, 2009
 *      Author: blick
 */

#include "run_starrynite_dump.h"
#include "starrynite_main.h"
#include "run_starrynite.h"

int main(int argc, char **argv)
{
  run_starrynite_t callback = run_starrynite_dump;
  starrynite_main(argc, argv, callback);
}
