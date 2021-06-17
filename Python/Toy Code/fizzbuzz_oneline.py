# Author : Milan Zanussi
# Date : Summer 2018
# Description : Computes the classical "Fizzbuzz" interview question in one line (however hideous)
# Heavily utilizes Python's "generator functions" feature. Since fizzbuzz is a trivial test to determine
# whether an applicant has at least a cursory understanding of a language, as well as to test to see
# how applicants try to approach a simple problem (the approach taken here is obviously not the simplest solution).

for x in (( "fizz"[: (0 if y%3 != 0 else 4) ] + "buzz"[: (0 if y%5 != 0 else 4)] if y%3 == 0 or y%5 == 0 else y) for y in range(1,101)): print(x)

# HOW THE FOR-LOOP IS USED : For-loops in Python are allowed to iterate over any generating expression,
# so my goal was to turn the problem into one where I could generate the entire "Fizzbuzz" sequence using
# a single generating expression and then write a for-loop which iterates over that generator and prints
# each result.

# HOW THE GENERATOR WAS DESIGNED : The generator itself uses nested ternary operators (expressions of the form "x if COND else y")
# to decide what's generated on each iteration. The outer ternary operator decides whether the number
# should be printed, or a string saying either "fizz", "buzz", or "fizzbuzz". The inner ternary operator
# decides whether which of these strings should be printed. If the number fails to be divisible by 3,
# then the "fizz" prefix is omitted using the substring operator to replace it with the empty string.
# The case with divisibility by 5 to decide the "buzz" suffix is handled analogously. If a number
# is divisible by both 3 and 5, then consequently "fizzbuzz" is printed since neither the prefix nor the
# suffix will be omitted. If both suffixes were to be omitted, then the outer ternary operator would have
# printed the number instead. This verifies the design of the generator.

# IS THERE ANY ADVANTAGES TO THIS DESIGN? : As far as I'm familiar, the same effect can be accomplished using
# the ordinary fizzbuzz strategy. The only potential areas the principles used to tackle this problem are is the following
#   1.) Python generators can allow us to encode the entire list of fizzbuzz numbers using a specification
#       as in the iterator used for the for-loop. This can allow for fizzbuzz sequence terms only on demand.
#       Furthermore, the space complexity of such a method of storage is essentially constant at the cost of
#       needing to perform a linear-time calculation to generate the whole list (linear in the number of desired terms)
#   2.) This generator approach can be slightly modifed to write a generating expression for "fizzbuzz" that
#       could generate arbitarily high fizz-buzz terms on demand. This would only require replacing "range(1,101)"
#       in the generating expression with some generating expression that iteratively generates arbitrarily
#       large natural numbers.
