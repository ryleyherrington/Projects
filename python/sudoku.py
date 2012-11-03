#!/usr/bin/python
import sys

def same_row(i,j): return (i/9 == j/9)
def same_col(i,j): return (i-j) % 9 == 0
#Divide it into thirds and then figure out if they are in the same
#box column/row (that's the %9/3 part).
def same_square(i,j): return (i/27 == j/27 and i%9/3 == j%9/3)

def r(a):
  for k in range (0, 81, 9):
    print a[k+0:k+3] + '|' + a[k+3:k+6] + '|' + a[k+6:k+9] 
    if k is 18 or k is 45:
      print '---+---+---'
  print ''      
 
  #  print a[:3] + ' ' + a[3:6] + ' ' + a[6:9] 
  #  print a[9:12] + ' ' + a[12:15] + ' ' + a[15:18] 
  #  print a[18:20] + ' ' + a[21:23] + ' ' + a[24:26] 
  #  print a[27:29] + ' ' + a[30:32] + ' ' + a[33:35] 
  #  print a[36:38] + ' ' + a[39:41] + ' ' + a[42:44] 
  #  print a[45:47] + ' ' + a[48:50] + ' ' + a[51:53] 
  #  print a[54:56] + ' ' + a[57:59] + ' ' + a[60:62] 
  #  print a[63:65] + ' ' + a[66:68] + ' ' + a[69:71] 
  #  print a[72:74] + ' ' + a[75:77] + ' ' + a[78:80] 

  #print a[:8] + ' ' + a[9:17] + ' '+ a[18:26]
  #print a[27:35] + ' ' + a[36:44] + ' '+ a[45:53]
  #print a[54:62] + ' ' + a[63:71] + ' '+ a[72:80]
  #print ''
  
  i = a.find('0')
  if i==-1:
    sys.exit(a)

  excluded = set()
  for j in range(81):
    if same_row(i,j) or same_col(i,j) or same_square(i,j):
      excluded.add(a[j])

  for n in '123456789':
    if n not in excluded:
      #By now, n is not excluded by any row, column, or square, so place it and start on the next 0
      #print a
      r(a[:i]+n+a[i+1:])

if __name__ == '__main__':
  if len(sys.argv) == 2 and len(sys.argv[1]) == 81:
    r(sys.argv[1])
  else:
    print 'Usage:sudoku.py numbers'
    print ' where numbers means an 81 character string. Left-->right, top-->bottom, and 0 means unknown'
