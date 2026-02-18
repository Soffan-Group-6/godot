#include "branch_coverage.h"

const int NUM_FUNCTIONS = 3;
const int MAX_BRANCHES_FUNC1 = 22;
const int MAX_BRANCHES_FUNC2 = 35;
const int MAX_BRANCHES_FUNC3 = 29;
BranchCoverageInfo func1_branches[MAX_BRANCHES_FUNC1];
BranchCoverageInfo func2_branches[MAX_BRANCHES_FUNC2];
BranchCoverageInfo func3_branches[MAX_BRANCHES_FUNC3];

FunctionCoverage functions[] = {
	{"ChainIK3DGizmoPlugin::get_joints_mesh()", MAX_BRANCHES_FUNC1, func1_branches},
	{"String::append_utf16()", MAX_BRANCHES_FUNC2, func2_branches},
	{"String::simplify_path()", MAX_BRANCHES_FUNC3, func3_branches}
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
	functions[function_index - 1].branches[branch_id - 1].taken = true;
}

void print_coverage()
{
	for (int f = 0; f < NUM_FUNCTIONS; f++)
	{
		std::cout << "\nFunction: " << functions[f].function_name << "\n";
		int branches_taken = 0;
		for (int i = 0; i < functions[f].num_branches; i++)
		{
			if (functions[f].branches[i].taken) branches_taken++;
			std::cout << "\tBranch " << functions[f].branches[i].id
					  << "\t  taken: " << functions[f].branches[i].taken << "\n";
		}
		std::cout << "\t" << branches_taken << "/" << functions[f].num_branches << " Branches Taken\n";
	}
}