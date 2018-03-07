


// [[Rcpp::export]]
void RcppParallel_delimitString(Rcpp::StringMatrix myVector) {

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





