import sys
import re
import optparse
import logging

__description__ = 'reads, compares two csv files and outputs a list of a different SWSs'
def late_binding_avoid():
   "to avoid a late binding use the with default arg evaluating"
   def create_multipliers():
      return [lambda x, i=i : i * x for i in range(5)]
   for multiplier in create_multipliers():
      print(multiplier(2))
def main():
   late_binding_avoid()

if __name__  == "__main__":
   main()
