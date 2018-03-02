// [[Rcpp::depends(RcppParallel)]]
#include <Rcpp.h>
#include <RcppParallel.h>
using namespace RcppParallel;




struct getElement : public Worker {
   // input vector to read from
   const std::vector< std::string > myVector;
   
   // output vector to write to
   std::vector< std::string > retVector;

   // initialize input and output matrixes
   getElement(const std::vector< std::string > myVector, std::vector< std::string > retVector)
      : myVector(myVector), retVector(retVector) {}
   
   // function call operator that work for the specified range (begin/end)
   void operator()(std::size_t begin, std::size_t end) {
     for (std::size_t i = begin; i < end; i++) {
       retVector[i] = "blah";
     }
     
//     retString = myString;
   }
   
};



// [[Rcpp::export]]
Rcpp::StringVector rcpp_parallel_delimitString(Rcpp::StringVector myVector) {
  
  // Because Rcpp data structures are not thread safe
  // we'll use std::vector for input and output.
  std::vector< std::string > tmpVector1;
  std::vector< std::string > tmpVector2;

  
  // Create the worker
  getElement getElement(tmpVector1, tmpVector2);

  // Call it with parallelFor
  parallelFor(0, tmpVector1.size(), getElement);

  // allocate the string we will return 
  Rcpp::StringVector retVector;
  
  return retVector;
}


