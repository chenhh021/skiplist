# stress test result

Test with a skiplist with 16 levels, run in 4 processes. Run on a Lenovo laptop with AMD Ryzen 7 5800H 3.20 GHz.

## insert test
| insert size (k) | elapsed time (s) | 
|-----------------|------------------| 
| 100             |0.0834205| 
| 1,000           |1.09726| 
| 1,0000          |9.53863| 

QPS: 1.048m

## search test
| search size (k) | elapsed time (s) | 
|-----------------|------------------| 
| 100             |0.0066056| 
| 1,000           |0.05648| 
| 1,0000          |0.457254| 

QPS: 21.872m