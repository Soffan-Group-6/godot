#include "branch_coverage.h"

const int NUM_FUNCTIONS = 1;
const int MAX_BRANCHES_FUNC1 = 22;
BranchCoverageInfo func1_branches[MAX_BRANCHES_FUNC1];

FunctionCoverage functions[] = {
	{"ChainIK3DGizmoPlugin::get_joints_mesh()", MAX_BRANCHES_FUNC1, func1_branches}
};

void init_coverage() {
	for (int f = 0; f < NUM_FUNCTIONS; f++) {
		for (int i = 0; i < functions[f].num_branches; i++) {
			functions[f].branches[i].id = i + 1;
			functions[f].branches[i].taken = false;
		}
	}
}

void hit(int function_index, int branch_id)
{
	functions[function_index].branches[branch_id - 1].taken = true;
}

void print_coverage()
{
	for (int f = 0; f < NUM_FUNCTIONS; f++)
	{
		std::cout << "\nFunction: " << functions[f].function_name << "\n";
		for (int i = 0; i < functions[f].num_branches; i++)
		{
			std::cout << "\tBranch " << functions[f].branches[i].id
					  << "\t  taken: " << functions[f].branches[i].taken << "\n";
		}
	}
}
