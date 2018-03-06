
#include <Rcpp.h>
#include <RcppParallel.h>
using namespace RcppParallel;
// [[Rcpp::depends(RcppParallel)]]


struct getElement : public Worker {
   // input vector to read from
   const std::vector< std::string > myVector;
   
   // output vector to write to
   std::vector< std::string > retVector;
   std::string myString;

   // initialize input and output vectors
   getElement(const std::vector< std::string > myVector, std::vector< std::string > &retVector)
      : myVector(myVector), retVector(retVector) {}
   
   // function call operator that work for the specified range (begin/end)
   void operator()(std::size_t begin, std::size_t end) {
//     Rcpp::Rcout << "Value: " << begin << "\n";
     for (std::size_t i = begin; i < end; i++) {
//       Rcpp::checkUserInterrupt();
       retVector[i] = "blah";
//       retVector[i] = myVector[i];
//       myString = myVector[i];
//       retVector[i] = myString;
//       retVector[i] = i;
//       Rcpp::Rcout << "Value: " << myString << "\n";
//       Rcpp::Rcout << "Value: " << retVector[i] << "\n";
     }
   }
};



// [[Rcpp::export]]
Rcpp::StringVector rcpp_parallel_delimitString(Rcpp::StringVector myVector) {
  
  // Because Rcpp data structures are not thread safe
  // we'll use std::vector for input and output.
  std::vector< std::string > tmpVector1(myVector.size());
  std::vector< std::string > tmpVector2(myVector.size());

  // Convert Rcpp::StringVector to std::vector.
  unsigned int i;
  std::string tmpString;

  for(i=0; i<myVector.size(); i++){
//    Rcpp::checkUserInterrupt();
    tmpString = myVector[i];
    tmpVector1[i] = tmpString;
  }
  
  // Create the worker
  getElement getElement(tmpVector1, tmpVector2);

  // Call it with parallelFor
  parallelFor(0, tmpVector1.size() - 1, getElement);

  // allocate the string we will return 
  Rcpp::StringVector retVector(tmpVector2.size());
  
  // Copy to Rcpp container to send back to R.
  retVector = tmpVector2;
  
  return retVector;
}


