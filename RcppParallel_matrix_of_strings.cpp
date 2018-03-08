
#include <Rcpp.h>
#include <RcppParallel.h>
using namespace RcppParallel;
// [[Rcpp::depends(RcppParallel)]]


struct getElement : public Worker {
   // input vector to read from
//   const std::vector< std::string > myVector;
    std::vector< std::vector< std::string > > &CppMatrix;
   

   // initialize input and output vectors
   getElement(std::vector< std::vector< std::string > > &CppMatrix)
     : CppMatrix(CppMatrix) {}
   
   // function call operator that work for the specified range (begin/end)
   void operator()(std::size_t begin, std::size_t end) {
//     Rcpp::Rcout << "Value: " << begin << "\n";
     for (std::size_t i = begin; i < end; i++) {
//       retVector[i] = myVector[i];
//       myString = myVector[i];
       char myDelim = ':';
       int element = 2;
//       myString = getElement(myString, myDelim, element);
//       retVector[i] = myString;
//       Don't do the following/
//       Rcpp::Rcout << "Value: " << myString << "\n";
//       Or this.
//       Rcpp::checkUserInterrupt();
     }
   }
};




// [[Rcpp::export]]
void stringExtract(Rcpp::StringMatrix RcppMatrix) {
  // Because Rcpp data structures are not thread safe
  // we'll use std::vector for input and output.
  std::vector< std::vector< std::string > > CppMatrix(RcppMatrix.nrow(), std::vector< std::string >(RcppMatrix.ncol()));
  
  // Load C++ data structure from Rcpp.
  int i = 0;
  int j = 0;
  std::string tmpString;
  for(i=0; i<CppMatrix.size(); i++){
    for(j=0; j<CppMatrix[i].size(); j++){
      CppMatrix[i][j] = RcppMatrix(i,j);
    }
  }
  
  // Create the worker
  struct getElement getElement(CppMatrix);
  
  // Call it with parallelFor
  unsigned int grainSize = 100;
  parallelFor(0, CppMatrix.size(), getElement, grainSize);
  
  
  
}




/**
 * ### Fabricate example data
 * 
 *
 */



/*** R
n <- 1e3
myGT <- paste(sample(0:1, size = n, replace = TRUE),
              sample(0:1, size = n, replace = TRUE),
              sep = "/")
myGT <- paste(myGT, LETTERS, sample(1:20, size = n, replace = TRUE), sep = ":")
myGT <- paste(myGT, LETTERS, 
              paste(sample(1:200, size = n, replace = TRUE), 
                    sample(1:200, size = n, replace = TRUE), sep=","),
              sep = ":")
myGT <- matrix(myGT, nrow=100, ncol = 100)
myGT[1:4,1:5]
*/


/**
 * ### Benchmark
 * 
 *
 */



/*** R
library(microbenchmark)
nTimes <- 20
#microbenchmark(gt1 <- delimitString(myGT, ":", 3), times = nTimes)
#head(gt1)
*/


/*** R
#
RcppParallel::setThreadOptions(numThreads = 4)
#microbenchmark(gt2 <- rcpp_parallel_delimitString(myGT), times = nTimes)
#head(gt2)
*/





