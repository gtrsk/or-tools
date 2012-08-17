// Copyright 2010-2012 Google
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Integer programming example that shows how to use the API.

#include "base/commandlineflags.h"
#include "base/logging.h"
#include "linear_solver/linear_solver.h"

namespace operations_research {
void RunIntegerProgrammingExample(
    MPSolver::OptimizationProblemType optimization_problem_type) {
  MPSolver solver("IntegerProgrammingExample", optimization_problem_type);
  const double infinity = solver.infinity();
  // x1 and x2 are integer non-negative variables.
  MPVariable* const x1 = solver.MakeIntVar(0.0, infinity, "x1");
  MPVariable* const x2 = solver.MakeIntVar(0.0, infinity, "x2");

  // Minimize x1 + 2 * x2.
  solver.SetObjectiveCoefficient(x1, 1);
  solver.SetObjectiveCoefficient(x2, 2);

  // 2 * x2 + 3 * x1 >= 17.
  MPConstraint* const c0 = solver.MakeRowConstraint(17, infinity);
  c0->SetCoefficient(x1, 3);
  c0->SetCoefficient(x2, 2);

  const MPSolver::ResultStatus result_status = solver.Solve();

  // Check that the problem has an optimal solution.
  if (result_status != MPSolver::OPTIMAL) {
    LOG(FATAL) << "The problem does not have an optimal solution!";
  }

  LOG(INFO) << "Problem solved in " << solver.wall_time() << " milliseconds";

  // The objective value of the solution.
  LOG(INFO) << "Optimal objective value = " <<  solver.objective_value();

  // The value of each variable in the solution.
  LOG(INFO) << "x1 = " << x1->solution_value();
  LOG(INFO) << "x2 = " << x2->solution_value();

  LOG(INFO) << "Advanced usage:";
  LOG(INFO) << "Problem solved in " << solver.nodes()
            << " branch-and-bound nodes";
}


void RunAllExamples() {
  #if defined(USE_GLPK)
  LOG(INFO) << "---- Integer programming example with GLPK ----";
  RunIntegerProgrammingExample(MPSolver::GLPK_MIXED_INTEGER_PROGRAMMING);
  #endif
  #if defined(USE_CBC)
  LOG(INFO) << "---- Integer programming example with CBC ----";
  RunIntegerProgrammingExample(MPSolver::CBC_MIXED_INTEGER_PROGRAMMING);
  #endif
  #if defined(USE_SCIP)
  LOG(INFO) << "---- Integer programming example with SCIP ----";
  RunIntegerProgrammingExample(MPSolver::SCIP_MIXED_INTEGER_PROGRAMMING);
  #endif
}
}  // namespace operations_research

int main(int argc, char **argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  operations_research::RunAllExamples();
  return 0;
}