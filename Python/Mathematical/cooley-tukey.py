# Author : Milan Zanussi
# Date : Some time between Fall 2018 or Spring 2019
# Project : Fast Fourier Transform
# Description : This program implements the Cooley-Tukey algorithm for the
# Fast (discrete) Fourier transform with radix 2 -- in other words,  the program
# assumes that the number of data points used to compute the transform is a power
# of two.

import math

# Implements Cooley-Tukey Fast Fourier Transform algorithm
# using radix 2 only. As a predcondition, data must have
# a size that is a power of 2. K is a positive integer
# which is the maximum
def FastFourierRad2(data):

    # Base case
    if(len(data) == 1):
        return data

    # Nontrivial case
    else:

        # Initialize DFT to data so DFT has the same number of elements as data
        DFT = data

        # Partition data into even and odd entries
        even = []
        odd = []
        
		# Generates even-indexed partition
        for i in range(len(data)//2): even.append(data[2*i])

		# Generates odd-indexed partition
        for i in range(len(data)//2): odd.append(data[2*i + 1])

        # Recursively calculate FFT on even and odd entries
        even = FastFourierRad2(even)
        odd = FastFourierRad2(odd)
        
        # Combine corresponding sums between partitions
        for k in range(len(data)//2):
            twiddle = math.e**(((-2*(math.pi))*1j)*(k/len(data)))
            DFT[k] = even[k] + twiddle * odd[k]
            DFT[k + len(data)//2] = even[k] - twiddle * odd[k]

        # Return Fourier transform
        return DFT

# Used in inverse transform function
def ComplexSwap(z):
    return complex(z.imag, z.real)

# Computes inverse transform using swapping method
def InvFastFourierRad2(data):

    # Swap components of complex number
    # (real coefficient becomes imaginary coefficient and vice versa)
    IDFT = [ ComplexSwap(z) for z in data ]

    # Compute the discrete fourier transform of the swapped data
    IDFT = FastFourierRad2(IDFT)

    # Swap complex components and normalize
    for i in range(len(IDFT)):
        IDFT[i] = ComplexSwap(IDFT[i])/len(IDFT)

    # Return results
    return IDFT
