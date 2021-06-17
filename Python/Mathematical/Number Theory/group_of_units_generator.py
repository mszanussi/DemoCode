#   Author: Milan Zanussi
#   Program: Group of Units Generator
#   Purpose: Contains code necessary to generate the group of units for a modular
#   ring of order n. This group of units will be denoted U(n). This program was written
# 	to solve some homework problems in my Symmetric Key Cryptography class (MATH 408 at Boise State)

from math import sqrt

# GCD(n,m) : Euclidean algorithm for computing GCD
def GCD(n,m):

    # Undefined cases (negatives undefined for simplicity)
    if(n < 0 or m < 0) or (n == 0 and m == 0):
        return None

    if (n < m):
        n,m = m,n

    if (n != 0 and m == 0):
        return n

    if(m == 1):
        return 1

    else:
        return GCD(m, n%m)

# Checks if a number is prime
def IsPrime(n):

    if n%2 == 0 & n != 2:
        return False

    no_prime_divisors = True
    for k in range(3, int(sqrt(n)) + 1, 2):
        if(n % k == 0):
            no_prime_divisors = False
            break

    return no_prime_divisors
        

# U(n) : Algorithm which finds the group of units for a given modular number n.
# If n is prime, then we simply return the set of non-zero integers below n.
def U(n) :
    if IsPrime(n):
        return [x for x in range(1,n)]

    else:
        units = []
        for k in range(1,n):
            if GCD(n,k) == 1:
                units.append(k)
                
        return units

