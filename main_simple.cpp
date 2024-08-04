#include <casadi/casadi.hpp>
#include <chrono>
#include <fstream>
#include <iostream>

using namespace std::chrono;

int main() {
  using namespace casadi;

  // Step 1: Define the symbolic expression
  SX x = SX::sym("x");
  SX f = x * x + x + 1;

  // Step 2: Differentiate the function
  SX f_prime = jacobian(f, x);

  // Step 3: Generate C++ code
  Function original_function = Function("f", {x}, {f});
  Function derivative_function = Function("f_prime", {x}, {f_prime});

  // Print the functions
  std::cout << "Original Function: " << f << std::endl;
  std::cout << "Derivative Function: " << f_prime << std::endl;

  // Save the generated C++ code to files
  std::ofstream original_code("original_function.c");
  original_code << original_function.generate();
  original_code.close();

  std::ofstream derivative_code("derivative_function.c");
  derivative_code << derivative_function.generate();
  derivative_code.close();

  // Step 4: Evaluate the original_function at a given x value
  auto start = high_resolution_clock::now();
  double x_value = 2.0; // Example value for x
  std::vector<DM> result;
  for (int ii = 0; ii < 1000; ii++) {
    DM x_eval = DM(ii * 0.5);
    std::vector<DM> arg = {x_eval};
    result = derivative_function(arg);
  }
  auto end = high_resolution_clock::now();
  // Print the result
  std::cout << "The value of the derivative function at x = " << x_value
            << " is " << result[0] << std::endl;
  duration<double, std::micro> duration = end - start;
  std::cout << "time: " << duration.count() << std::endl;

  start = high_resolution_clock::now();
  double res;
  for (int ii = 0; ii < 1000; ii++) {
    double val = ii * 0.5;
    res = val + val + 1;
  }
  end = high_resolution_clock::now();
  duration = end - start;
  std::cout << "result2: " << res << std::endl;
  std::cout << "time2: " << duration.count() << std::endl;

  return 0;
}
