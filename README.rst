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
Test.
..
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

  # Default to compute statistics on the first (tab-delimited) column.
  #
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
