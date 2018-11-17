#include "starrynite_main.h"
#include "run_starrynite.h"

int main(int argc, char **argv)
{
  run_starrynite_t callback = run_starrynite;
  starrynite_main(argc, argv, callback);
}
