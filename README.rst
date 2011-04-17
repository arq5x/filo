:Project: filo
:Version: 1.1.0
:Authors: - Aaron Quinlan, University of Virginia
          - Assaf Gordon, Cold Spring Harbor Laboratories
:Contact: arq5x@virginia.edu

=========================================
filo - Useful FILe and stream Operations
=========================================

The following tools are available as part of the **filo** package.  More to come...

.. contents::


groupBy
-------
**groupBy** is a useful tool that mimics the "groupBy" clause in database systems. Given a file or stream that is sorted by the appropriate "grouping columns", **groupBy** will compute summary statistics on another column in the file or stream. This will work with output from all BEDTools as well as any other tab-delimited file or stream.

You specify a list of columns that should be "grouped" with the -g parameter (e.g., -g 2,3,4 will group on the second through fourth columns).  You then specify column(s) that should be summarized or "operated upon" for each group with the -c parameter (e.g., -c 2 or -c 2,3 or -c 2,2,2,5).  Finally, you specify what operations should be applied to the list of columns in -c.  

Here is the current list of the available operations.

1. *sum* - numeric only
2. *count* - numeric or text 
3. *min* - numeric only 
4. *max* - numeric only 
5. *mean* - numeric only 
6. *stdev* - numeric only 
7. *median* - numeric only 
8. *mode* - numeric or text 
9. *antimode* - numeric or text collapse (i.e., print a comma separated list) - numeric or text 
10. *freqasc* - print a comma separated list of values observed and the number of times they were observed. Reported in **ascending** order of frequency.
11. *freqdesc* - print a comma separated list of values observed and the number of times they were observed. Reported in descending order of frequency.
12. *collapse* - print a comma separated list of each value in the grouped column.
13. *concat* - concattenate each value in the grouped column into a single string.

And here are some usage examples.  I hope you find this utility to be of use in your work.  I have found it to be a huge time saver.

::

  $ cat ex1.out
  chr1 10  20  A   chr1    15  25  B.1 1000
  chr1 10  20  A   chr1    25  35  B.2 10000
  
  $ groupBy -i ex1.out -g 1,2,3,4 -c 9 -o sum
  chr1 10  20  A   11000
  
  $ groupBy -i ex1.out -grp 1,2,3,4 -opCols 9,9 -ops sum,max
  chr1 10  20  A   11000   10000
  
  $ groupBy -i ex1.out -g 1,2,3,4 -c 8,9 -o collapse,mean
  chr1 10  20  A   B.1,B.2,    5500
  
  $ cat ex1.out | groupBy -g 1,2,3,4 -c 8,9 -o collapse,mean
  chr1 10  20  A   B.1,B.2,    5500


shuffle
-------
**shuffle** will randomize the order of lines in a file.  In other words, if you have a sorted file, **shuffle** will undo the sort.

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

  $ shuffle test
  8	9
  0	1
  4	5
  7	8
  9	10
  5	6
  3	4
  2	3
  1	2
  6	7
  10	11

  $ cat test | shuffle
  4	5
  6	7
  0	1
  9	10
  3	4
  7	8
  5	6
  10	11
  8	9
  2	3
  1	2



stats
-----
**stats** is a small utility for computing descriptive statistic on a given column of a tab-delimited file or stream.  By default, it will assume you want to gather stats on the first column in your file/stream and compute *all* of the following statistics:

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
  StdDev:		3.16227766016838

  #######################################################################
  # Let's work with the second (1-based) column.
  #######################################################################
  $ stats test -c 2
  Total lines:		11
  Sum of lines:		66
  Ari. Mean:		6
  Geo. Mean:		4.9092387795844
  Median:		6
  Mode:			1 (N=1)
  Anti-Mode:		1 (N=1)
  Minimum:		1
  Maximum:		11
  Variance:		10
  StdDev:		3.16227766016838

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
  Median:		6

  #######################################################################
  # You get the idea.  Other options are available with -h
  #######################################################################





