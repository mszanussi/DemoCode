# Program : GCD Speedup
# Author : Milan Zanussi
# Purpose : Implement GCD on the integers by instead using the absolute value
# Euclidean valuation on the integers instead of the basic integer ordering.
# (NOTE : This code was intended to compare the speed of GCD using standard valuation
#  to GCD using the absolute value valuation. The printouts are meant to compare the
#  numbers of iterations needed to compute GCD(a,b) using either valuation).

# GCD : Implements GCD using absolute value for the Euclidean valuation
def GCD(a,b):

    if a == 0:
        return b

    if b == 0:
        return a
    
    r = a%b

    if int(abs(r - b)) < int(abs(r)) :
        print(abs(r-b))
        return GCD(b, int(abs(r - b)))
    else:
        print(r)
        return GCD(b,r)

# altGCD : Implements GCD with standard valuation
def altGCD(a,b):

    if a == 0:
        return b

    if b == 0:
        return a
    
    r = a%b
    print(r)
    return GCD(b, r)
