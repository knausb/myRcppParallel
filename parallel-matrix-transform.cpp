
// [[Rcpp::depends(RcppParallel)]]
#include <RcppParallel.h>
using namespace RcppParallel;

struct SquareRoot : public Worker
{
  // source matrix
  const RMatrix<double> input;
  
  // destination matrix
  RMatrix<double> output;
  
  // initialize with source and destination
  SquareRoot(const NumericMatrix input, NumericMatrix output) : input(input), output(output) {}
  
  // take the square root of the range of elements requested
     void operator()(std::size_t begin, std::size_t end) {
        std::transform(input.begin() + begin, 
                       input.begin() + end, 
                       output.begin() + begin, 
                       ::sqrt);
     }
};
