:Project: filo
:Version: 1.1.0
:Authors: - Aaron Quinlan, University of Virginia
          - Assaf Gordon, Cold Spring Harbor Laboratories
:Contact: arq5x@virginia.edu

=========================================
filo - Useful FILe and stream Operations
=========================================

groupBy
-------


shuffle
-------


stats
-----
*stats* is a small utility for computing descriptive statistic on a given column of a tab-delimited file or stream.  By default, it will assume you want to gather stats on the first column in your file/stream and compute **all** of the following statistics:

1. total number of lines
2. the sum of all the values in the column
3. the arithmetic mean (i.e., the "average") of the values in the column
4. the geometric mean (if possible)
5. the median
6. the mode
7. the anti-mode (i.e., the least frequent value)
8. the minimum
9. the maximum
10. the variance
11. the standard deviation.

Here are some examples of stats in action.

::
  
  $ cat test
  0	1
  1	2
  2	3
  3	4
  4	5
  5	6
  6	7
  7	8
  8	9
  9	10
  10	11

  #######################################################################
  # Default is to compute statistics on the first (tab-delimited) column.
  #######################################################################
  $ stats test
  Total lines:		11
  Sum of lines:		55
  Ari. Mean:		5
  Geo. Mean:		undef (zero found in data)
  Median:			5
  Mode:			0 (N=1)
  Anti-Mode:		0 (N=1)
  Minimum:		0
  Maximum:		10
  Variance:		10
  StdDev:			3.16227766016838

  #######################################################################
  # Let's work with the second (1-based) column.
  #######################################################################
  $ stats test -c 2
  Total lines:		11
  Sum of lines:		66
  Ari. Mean:		6
  Geo. Mean:		4.9092387795844
  Median:			6
  Mode:			1 (N=1)
  Anti-Mode:		1 (N=1)
  Minimum:		1
  Maximum:		11
  Variance:		10
  StdDev:			3.16227766016838

  #######################################################################
  # Let's just get the mean of the second (1-based) column.
  #######################################################################
  $ stats test -c 2 -mu
  Total lines:		11
  Sum of lines:		66
  Ari. Mean:		6

  #######################################################################
  # It works on stdin as well.
  #######################################################################
  $ stats -c 2 -max < test
  Total lines:		11
  Sum of lines:		66
  Maximum:		11

  $ cat test | stats -c 2 -med
  Total lines:		11
  Sum of lines:		66
  Median:			6

  #######################################################################
  # You get the idea.  Other options are available with -h
  #######################################################################





