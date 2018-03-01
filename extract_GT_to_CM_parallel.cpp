#include <Rcpp.h>
// [[Rcpp::depends(RcppParallel)]]
#include <RcppParallel.h>
using namespace RcppParallel;


struct GT_to_SM_parallel : public Worker {
  
  // input matrix to read from
  const std::vector< std::vector< std::string > > mat;
  
  // output matrix to write to
  std::vector< std::vector< std::string > > rmat;
  
  // initialize from Rcpp input and output matrixes (the RMatrix class
  // can be automatically converted to from the Rcpp matrix type)
  GT_to_SM_parallel(const std::vector< std::vector< std::string > > mat, 
                    std::vector< std::vector< std::string > > rmat) :
    mat(mat), rmat(rmat) {}
  
  
     // function call operator that work for the specified range (begin/end)
   void operator()(std::size_t begin, std::size_t end) {
      for (std::size_t i = begin; i < end; i++) {
         for (std::size_t j = 0; j < i; j++) {
           
         }
      }
   }
  
};




//' @export
// [[Rcpp::export(name=".extract_GT_to_CM_parallel")]]
Rcpp::StringMatrix extract_GT_to_CM_parallel( Rcpp::StringMatrix fix,
                                         Rcpp::StringMatrix gt,
                                         std::string element="DP",
                                         int alleles = 0,
                                         int extract = 1,
                                         int convertNA = 1) {
  
  // Rcpp data structures are not thread safe.
  // So we'll convert it to std::string.
  std::vector< std::vector< std::string > > gt2;
  
  
  
  // Initialize output data structure.
  Rcpp::CharacterMatrix rmat(gt.nrow(), gt.ncol() - 1);
  
   // create the worker
   GT_to_SM_parallel GT_to_SM_parallel(gt2, rmat);
     
   // call it with parallelFor
   parallelFor(0, gt2.nrow(), GT_to_SM_parallel);
  
  return rmat;
}


