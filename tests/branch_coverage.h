#include <iostream>

struct BranchCoverageInfo
{
	int id;
	bool taken;
};

struct FunctionCoverage
{
	const char *function_name;
	int num_branches;
	BranchCoverageInfo *branches;
};

extern const int NUM_FUNCTIONS;
extern FunctionCoverage functions[];

void init_coverage();
void hit(int function_index, int branch_id);
void print_coverage();
