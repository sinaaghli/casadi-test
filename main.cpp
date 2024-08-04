#include <casadi/casadi.hpp>
#include <chrono>
#include <dlfcn.h> // Include for dynamic loading
#include <fstream>
#include <iostream>

using namespace casadi;
using namespace std::chrono;

// Define casadi_real and casadi_int to match the definitions in the generated
// code
typedef double casadi_real;
typedef long long int casadi_int;

// Define the function pointer type based on the generated code's signature
typedef int (*func_t)(const casadi_real **arg, casadi_real **res,
                      casadi_int *iw, casadi_real *w, int mem);

// Function to generate and compile CasADi functions
void generateAndCompileFunctions(const std::string &lib_name,
                                 const Function &original_function,
                                 const Function &derivative_function) {
  // Generate C code using CodeGenerator
  CodeGenerator gen(lib_name);
  gen.add(original_function);
  gen.add(derivative_function);

  // Save the generated C code to a file
  std::ofstream c_code_file(lib_name + ".c");
  gen.dump(c_code_file);
  c_code_file.close();

  // Compile the C code into a shared library
  std::string compile_command =
      "gcc -fPIC -shared -o " + lib_name + ".so " + lib_name + ".c -lm";
  int result = system(compile_command.c_str());
  if (result != 0) {
    std::cerr << "Compilation failed" << std::endl;
    exit(1);
  }
}

// Function to load the shared library and get the function pointer
func_t loadFunction(const std::string &lib_name,
                    const std::string &function_name, void **handle) {
  // Load the compiled shared library
  *handle = dlopen(("./" + lib_name + ".so").c_str(), RTLD_LAZY);
  if (!*handle) {
    std::cerr << "Failed to load the shared library: " << dlerror()
              << std::endl;
    exit(1);
  }

  // Locate the compiled function
  func_t func_eval = (func_t)dlsym(*handle, function_name.c_str());
  if (!func_eval) {
    std::cerr << "Failed to locate the function: " << dlerror() << std::endl;
    dlclose(*handle);
    exit(1);
  }

  return func_eval;
}

// Function to evaluate the loaded function
double evaluateFunction(func_t func_eval, double x_value) {
  // Prepare the input and output arguments
  casadi_real x_val = x_value;
  const casadi_real *arg[] = {&x_val};
  casadi_real result_val;
  casadi_real *res[] = {&result_val};

  // Call the function
  func_eval(arg, res, nullptr, nullptr, 0);

  // Print the result

  return result_val;
}

int main() {
  // Step 1: Define the symbolic expression
  SX x = SX::sym("x");
  SX f = x * x + x + 1;

  // Step 2: Differentiate the function
  SX f_prime = jacobian(f, x);

  // Step 3: Create CasADi functions
  Function original_function = Function("original_function", {x}, {f});
  Function derivative_function =
      Function("derivative_function", {x}, {f_prime});

  // Print the functions
  std::cout << "Original Function: " << f << std::endl;
  std::cout << "Derivative Function: " << f_prime << std::endl;

  // Generate and compile the functions
  generateAndCompileFunctions("generated_code", original_function,
                              derivative_function);

  // Load the compiled function
  void *handle = nullptr;
  func_t original_function_eval =
      loadFunction("generated_code", "original_function", &handle);

  // Evaluate the function at different values
  double result;
  auto start = high_resolution_clock::now();
  for (uint i = 0; i < 100000; i++) {
    result += evaluateFunction(original_function_eval, i * 0.5);
  }
  auto end = high_resolution_clock::now();
  duration<double, std::micro> duration = end - start;
  std::cout << "time was: " << duration.count() << std::endl;

  start = high_resolution_clock::now();
  double res;
  for (int ii = 0; ii < 1000; ii++) {
    double val = ii * 0.5;
    res = val * val + val + 1;
  }
  end = high_resolution_clock::now();
  duration = end - start;
  std::cout << "time2: " << duration.count() << std::endl;
  // Close the shared library
  dlclose(handle);

  return 0;
}
